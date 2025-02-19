/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_anti_aliasing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/19 09:36:52 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]
void	put_pixel_buffer(t_data *data, int x, int y, unsigned int color);
void	ft_anti_aliasing(t_data *data);
unsigned int	ft_average_pixel(t_data *data, int x, int y);
void	ft_weighted_color(t_img *buffer, int x, int y, t_coor *rtrn, double scalar);
///////////////////////////////////////////////////////////////////////////////]

///////////////////////////////////////////////////////////////////////////////]
void	put_pixel_buffer(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;
	int		offset;

	offset = (y * data->buffer.ll + x * (data->buffer.bpp / 8));
	dst = data->buffer.addr + offset;
	*(unsigned int *)dst = color;
}

///////////////////////////////////////////////////////////////////////////////]
// for each pixel innnn the buffer, average the 3x3 px around, put it on screen
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

#define PX_MIDDLE 0.5
#define PX_EDGE 0.1
#define PX_CORNER 0.025

///////////////////////////////////////////////////////////////////////////////]
// does the averaging
unsigned int	ft_average_pixel(t_data *data, int x, int y)
{
	t_coor	px = {0};
	t_rgb	color;

	ft_weighted_color(&data->buffer, x, y, &px, PX_MIDDLE);

	ft_weighted_color(&data->buffer, x - 1, y, &px, PX_EDGE);
	ft_weighted_color(&data->buffer, x + 1, y, &px, PX_EDGE);
	ft_weighted_color(&data->buffer, x, y - 1, &px, PX_EDGE);
	ft_weighted_color(&data->buffer, x, y + 1, &px, PX_EDGE);

	ft_weighted_color(&data->buffer, x + 1, y + 1, &px, PX_CORNER);
	ft_weighted_color(&data->buffer, x - 1, y + 1, &px, PX_CORNER);
	ft_weighted_color(&data->buffer, x + 1, y - 1, &px, PX_CORNER);
	ft_weighted_color(&data->buffer, x - 1, y - 1, &px, PX_CORNER);

	color.r = min(255, max(0, (int)round(px.x)));
	color.g = min(255, max(0, (int)round(px.y)));
	color.b = min(255, max(0, (int)round(px.z)));

	return ((unsigned int)(color.r << 16 | color.g << 8 | color.b));
}

///////////////////////////////////////////////////////////////////////////////]
// read the pixel xy, weight it by scalar
void	ft_weighted_color(t_img *buffer, int x, int y, t_coor *rtrn, double scalar)
{
	unsigned int color;
	color = *(unsigned int *)(buffer->addr + (y * buffer->ll + x * (buffer->bpp / 8)));

	rtrn->x += ((color >> 16) & 0xFF) * scalar;
	rtrn->y += ((color >> 8) & 0xFF) * scalar;
	rtrn->z += (color & 0xFF) * scalar;

}