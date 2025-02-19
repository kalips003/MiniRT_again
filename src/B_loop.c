/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/19 14:50:59 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_loop(t_data *data);
void	h_refresh_input_file(t_data *data, time_t time);
int		ft_render_frame(t_data *data, int sublim);
int		calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim);
int		h_bg_texture(t_data *data, t_calcul_px *calcul);

///////////////////////////////////////////////////////////////////////////////]
// main loop refresh if file is changed
int 	ft_loop(t_data *data)
{
	struct stat	file_stat;
	
	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			h_refresh_input_file(data, file_stat.st_mtime);
			ft_render_frame(data, 0);
		}
	}
	else
		perror("stat");
	// if (data->change)//click on an object, swiitch to some variable in the object, start incresing it % max
	// 	ft_change(data);
	return (0);
}

void	h_refresh_input_file(t_data *data, time_t time)
{
	t_camera		*old_camera;

	old_camera = mem(0, sizeof(t_camera));
	*old_camera = *data->eye.c;
	usleep(100);
	data->last_modif_time = time;
	printf("REFRESH\n");
	end(data, 0, 0);
	data->camera = expand_tab((void **)data->camera, old_camera);
	read_file(2, data->av, data);
	data->change_obj = NULL;
	data->current_camera = 0;
	data->eye.c = data->camera[0];
}

void	h_refresh_input_file_v2(t_data *data, time_t time)
{
	t_obj		old_camera;

	old_camera = data->eye.c->O;
	usleep(100);
	data->last_modif_time = time;
	put("REFRESH\n");
	end(data, 0, 0);
	read_file(2, data->av, data);
	usleep(100);
	data->change_obj = NULL;
	data->current_camera = 0;
	data->eye.c = data->camera[0];
	data->eye.c->O = old_camera;
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
	int			x;
	int			y;
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
			put_pixel_buffer(data, x, y, c.argb.r << 16 | c.argb.g << 8 | c.argb.b);
			// mlx_pixel_put(data->mlx, data->win, x, y, c.argb.r << 16 | c.argb.g << 8 | c.argb.b);
		}
	}
	if (sublim && ANTI_ALIASING)
		ft_anti_aliasing(data);
	else
		mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// require ray origin (c0), ray vector (v)
// fills calcul.argb with the pixel color shaded of the intersection
int	calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim)
{
	if (!ft_find_pixel_colision(data, c))
	{
		if (data->bg_light[0]->texture)
			return (h_bg_texture(data, c));
		c->argb.r = (int)(round(data->bg_light[0]->color.r * data->bg_light[0]->ratio));
		c->argb.g = (int)(round(data->bg_light[0]->color.g * data->bg_light[0]->ratio));
		c->argb.b = (int)(round(data->bg_light[0]->color.b * data->bg_light[0]->ratio));
		return (0);
	}
	else if (sublim)
		ft_lighting_sublim(data, c);
	else
		ft_lighting_simple(data, c);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// IF ambient light has a texture, return the correct pixel instead of simple bg color
int	h_bg_texture(t_data *data, t_calcul_px *calcul)
{
	double	l_θ;
	double	l_ϕ;
	
	l_θ = fmin(1.0, fmax(0.0, atan2(calcul->v.dz, calcul->v.dx)  / (2 * PI) + 0.5));
	l_ϕ = fmin(1.0, fmax(0.0, acos(calcul->v.dy) / PI));

	calcul->argb = return_px_img(data->bg_light[0]->texture, l_θ, l_ϕ);
	return (0);
}