/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/13 14:00:45 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_loop(t_data *data);
int		ft_render_frame(t_data *data, int sublim);
int		calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim);
int		h_bg_texture(t_data *data, t_calcul_px *calcul);

///////////////////////////////////////////////////////////////////////////////]
// main loop refresh if file is changed
int 	ft_loop(t_data *data)
{
	struct stat	file_stat;
	t_obj		old_camera;
	
	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			old_camera = data->eye.c->O;
			usleep(100);
			data->last_modif_time = file_stat.st_mtime;
			put("REFRESH\n");
			end(data, 0, 0);
			read_file(2, data->av, data);
			data->eye.c = data->camera[0];
			data->current_camera = 0;
			data->eye.c->O = old_camera;
			data->change_obj = NULL;
			ft_render_frame(data, 0);
		}
	}
	else
		perror("stat");
	// if (data->change)//click on an object, swiitch to some variable in the object, start incresing it % max
	// 	ft_change(data);
	return (0);
}

// void	ft_change(t_data *data)
// {
// 	if (data->change_obj)
// 		// ((t_sphere *)(data->change_obj))->color.r = (((t_sphere *)(data->change_obj))->color.r + CHANGE) % 256;
// 		data->change_obj->O.c0.x += CHANGE;
// 	ft_render_frame(data);
// 	usleep(100);
// }


///////////////////////////////////////////////////////////////////////////////]
// render simple shadows, uses adjusted by tan angle
// used to render while moving
int	ft_render_frame(t_data *data, int sublim)
{
	int			y;
	int			x;
	double		angleA;
	double		angleB;
	t_calcul_px	c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->O.c0;

	y = -1;
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		angleB = atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst);
		while (++x < SIZE_SCREEN_X)
		{
			angleA = atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst);
			c.v = combined_quaternion_rotation(&data->eye.c->O, angleA, angleB);
			c.current_gamma = 1.0;
			c.previous_gamma = 1.0;
			calculate_pixel_color(data, &c, sublim);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim)
{
	if (!ft_find_pixel_colision(data, c))
	{
		if (data->bg_light[0]->texture)
			return (h_bg_texture(data, c));
		c->px_color.r = (int)(round(data->bg_light[0]->color.r * data->bg_light[0]->ratio));
		c->px_color.g = (int)(round(data->bg_light[0]->color.g * data->bg_light[0]->ratio));
		c->px_color.b = (int)(round(data->bg_light[0]->color.b * data->bg_light[0]->ratio));
		return (0);
	}
	else if (sublim)
		ft_lighting_sublim(data, c);
	else
		ft_lighting_simple(data, c);
	return (1);
}

int	h_bg_texture(t_data *data, t_calcul_px *calcul)
{
	t_img *txt = data->bg_light[0]->texture;

	double	l_θ = fmin(1.0, fmax(0.0, atan2(calcul->v.dz, calcul->v.dx)  / (2 * PI) + 0.5));
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(calcul->v.dy) / PI));

	int text_x = ((int)floor(l_θ * txt->sz_x) + txt->sz_x) % txt->sz_x;
	int text_y = ((int)floor(l_ϕ * txt->sz_y) + txt->sz_y) % txt->sz_y;
	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	calcul->px_color = (t_rgb){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
	return (0);
}