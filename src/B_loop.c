/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 17:31:37 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_loop(t_data *data);
int		ft_render_frame(t_data *data, int sublim);
void	calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim);

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
			// f_calculate_combined_quaternion(data, angleA, angleB, &c.v);
			c.v = combined_quaternion_rotation(&data->eye.c->O, angleA, angleB);
			calculate_pixel_color(data, &c, sublim);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
void	calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim)
{
	if (!ft_find_pixel_colision(data, c))
	{
		c->px_color.r = (int)(round(data->bg_light[0]->color.r * data->bg_light[0]->ratio));
		c->px_color.g = (int)(round(data->bg_light[0]->color.g * data->bg_light[0]->ratio));
		c->px_color.b = (int)(round(data->bg_light[0]->color.b * data->bg_light[0]->ratio));
	}
	else if (sublim)
		ft_lighting_sublim(data, c);
	else
		ft_lighting_simple(data, c);
}