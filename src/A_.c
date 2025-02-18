/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]
t_argb	return_px_img(t_img *img, double x, double y);
t_vect	return_vect_img(t_img *img, double x, double y);
int		return_alpha_img(t_img *img, double x, double y);
///////////////////////////////////////////////////////////////////////////////]
t_argb	return_px_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_argb	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));

	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));

	rtrn = (t_argb){
		(pixel >> 24) & 0xFF,
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return (rtrn);
}

t_vect	return_vect_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_vect	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));

	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;

	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));

	rtrn = (t_vect){
		((pixel >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((pixel >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(pixel & 0xFF) / 255.0 * 2.0 - 1.0
	};
	ft_normalize_vect(&rtrn);
	// rtrn.dy *= -1;// ???
	rtrn.dx *= -1;// ???
	// rtrn.dz *= -1; // Flip depth axis if needed (opengl map)
	return (rtrn);
}

int		return_alpha_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_rgb	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));

	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;

	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));

	rtrn = (t_rgb){
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return ((rtrn.r + rtrn.g + rtrn.b) / 3);
}
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
int	ft_diffuse_sublim(t_data *data, t_calcul_px *c, t_light *lights)
{
	double adjusted_intensity;
	
	c->dist_light = dist_two_points(&c->inter, &lights->xyz);
	c->v_light = vect_ab_norm(&c->inter, &lights->xyz);
	c->cos_angle = ft_dot_product(&c->v_light, &c->v_normal);
	if (c->cos_angle < EPSILON || light_tracing_sublim(data, c, lights))
		return (0);
	adjusted_intensity = c->transp_light.ratio * c->cos_angle;
	adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (1 + c->dist_light * c->dist_light);

	c->diffuse.x += c->argb.r * c->transp_light.color.r / 255.0 * adjusted_intensity;
	c->diffuse.y += c->argb.g * c->transp_light.color.g / 255.0 * adjusted_intensity;
	c->diffuse.z += c->argb.b * c->transp_light.color.b / 255.0 * adjusted_intensity;
	return (1);
}

int light_tracing_sublim(t_data *data, t_calcul_px *calcul, t_light *light)
{
	t_calcul_px	c;
	c.c0 = light->xyz;
	c.v = (t_vect){-calcul->v_light.dx, -calcul->v_light.dy, -calcul->v_light.dz};
	
	double	transp;
	t_light	rtrn;
	t_coor	rgb = (t_coor){light->color.r, light->color.g, light->color.b};
	
	rtrn.ratio = light->ratio;
	double dist_l = calcul->dist_light - EPSILON;
	c.dist = dist_l;

	c.print = calcul->print * 2;

	while (c.dist > EPSILON)
	{
		if (ft_find_pixel_colision_sublim(data, &c))
		{

			c.eff_trans = fmin(1.0, c.argb.a / 255.0 + ((t_obj2*)c.object)->param.transparence);
			transp = ((t_obj2*)c.object)->param.transparence;
			if (c.eff_trans < EPSILON)
				return (1);
			rtrn.ratio *= c.eff_trans;
			transp = 1.0 - c.eff_trans;
			rgb.x *= c.argb.r / 255.0 * c.eff_trans;
			rgb.y *= c.argb.g / 255.0 * c.eff_trans;
			rgb.z *= c.argb.b / 255.0 * c.eff_trans;
			c.c0 = c.inter;
			dist_l -=c.dist; 
			c.dist = dist_l;
		}
		else
			break ;
	}
	rtrn.color.r = (int)min(255.0, max(0.0, floor(rgb.x)));
	rtrn.color.g = (int)min(255.0, max(0.0, floor(rgb.y)));
	rtrn.color.b = (int)min(255.0, max(0.0, floor(rgb.z)));
	if (calcul->print)
		printf("light= [%d,%d,%d; %.3f]\n", rtrn.color.r, rtrn.color.g, rtrn.color.b, rtrn.ratio);
	calcul->transp_light = rtrn;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_refracted_sublim(t_data *data, t_calcul_px *c, int sublim)
{

	t_argb	behind;

	c->eff_trans = fmin(1.0, c->argb.a / 255.0 + ((t_obj2*)c->object)->param.transparence);
	if (c->eff_trans > EPSILON)
	{
		behind = what_is_behind_sublim(data, c, sublim);
		c->behind.x = behind.r * c->eff_trans;
		c->behind.y = behind.g * c->eff_trans;
		c->behind.z = behind.b * c->eff_trans;
	}
	else
	{
		c->behind.x = c->argb.r * c->eff_trans;
		c->behind.y = c->argb.g * c->eff_trans;
		c->behind.z = c->argb.b * c->eff_trans;
	}
}
///////////////////////////////////////////////////////////////////////////////]
t_argb	what_is_behind_sublim(t_data *data, t_calcul_px *calcul, int sublim)
{
	t_calcul_px	c;

	if (calcul->transparence_depth == TRANSPARENCE_BOUNCES)
		return (calcul->argb);
	// ft_memset(&c, 0, sizeof(t_calcul_px));
	
	c.transparence_depth = calcul->transparence_depth + 1;
	c.reflected_depth = calcul->reflected_depth;
	c.current_gamma = ((t_obj2*)calcul->object)->param.gamma;
	c.previous_gamma = calcul->current_gamma;

	c.print = calcul->print + !!(calcul->print);
	c.c0 = new_moved_point(&calcul->inter, &calcul->v, EPSILON);
	if (((t_obj2*)calcul->object)->type == PLANE || ((t_obj2*)calcul->object)->type == SPRITE)
		c.v = calcul->v;
	else
		c.v = ft_vect_refracted_v2(&calcul->v, &calcul->v_normal, calcul->current_gamma, c.current_gamma);
	c.dist = -1.0;

	calculate_pixel_color(data, &c, sublim);
	return (c.argb);
}
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
void	put_pixel_buffer(t_data *data, int x, int y, unsigned int color);
void	ft_anti_aliasing(t_data *data);
unsigned int	ft_average_pixel(t_data *data, int x, int y);
void	ft_weighted_color(t_img *buffer, int x, int y, t_coor *rtrn, double scalar);

void	put_pixel_buffer(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;
	int		offset;

	offset = (y * data->buffer.ll + x * (data->buffer.bpp / 8));
	dst = data->buffer.addr + offset;
	*(unsigned int *)dst = color;
}

void	ft_anti_aliasing(t_data *data)
{
	int		x;
	int		y;
	unsigned int	color;

	y = 0;
	while (++y < SIZE_SCREEN_Y - 1)
	{
		x = 0;
		while (++x < SIZE_SCREEN_X - 1)
		{
			color = ft_average_pixel(data, x, y);
			mlx_pixel_put(data->mlx, data->win, x, y, color);
		}
	}
}

unsigned int	ft_average_pixel(t_data *data, int x, int y)
{
	t_coor	px = {0};
	t_rgb	color;

	ft_weighted_color(&data->buffer, x, y, &px, 0.2);

	ft_weighted_color(&data->buffer, x - 1, y, &px, 0.1);
	ft_weighted_color(&data->buffer, x + 1, y, &px, 0.1);
	ft_weighted_color(&data->buffer, x, y - 1, &px, 0.1);
	ft_weighted_color(&data->buffer, x, y + 1, &px, 0.1);

	ft_weighted_color(&data->buffer, x + 1, y + 1, &px, 0.025);
	ft_weighted_color(&data->buffer, x - 1, y + 1, &px, 0.025);
	ft_weighted_color(&data->buffer, x + 1, y - 1, &px, 0.025);
	ft_weighted_color(&data->buffer, x - 1, y - 1, &px, 0.025);

	color.r = min(255, max(0, (int)round(px.x)));
	color.g = min(255, max(0, (int)round(px.y)));
	color.b = min(255, max(0, (int)round(px.z)));

	return ((unsigned int)(color.r << 16 | color.g << 8 | color.b));
}

void	ft_weighted_color(t_img *buffer, int x, int y, t_coor *rtrn, double scalar)
{
	unsigned int color;
	color = *(unsigned int *)(buffer->addr + (y * buffer->ll + x * (buffer->bpp / 8)));

	rtrn->x += ((color >> 16) & 0xFF) * scalar;
	rtrn->y += ((color >> 8) & 0xFF) * scalar;
	rtrn->z += (color & 0xFF) * scalar;

}