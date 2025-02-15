/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D9_dist_sprite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/15 01:01:11 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_sprite(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_sprite(t_calcul_px *calcul, t_sprite *plane, t_plane_calc* c, int simple);
void	h_img_sprite(t_calcul_px *calcul, t_plane_calc* c, t_sprite *sprite);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_sprite(t_calcul_px *calcul, void *obj, int simple)
{
	t_plane_calc	c;
	t_sprite	*sprite;
	t_coor		inter;

	sprite = (t_sprite*)obj;

	c.top = -(sprite->O.up.dx * calcul->c0.x + sprite->O.up.dy * calcul->c0.y + sprite->O.up.dz * calcul->c0.z + sprite->d);
	c.bot = sprite->O.up.dx * calcul->v.dx + sprite->O.up.dy * calcul->v.dy + sprite->O.up.dz * calcul->v.dz;
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;

	if (c.dist < EPSILON)
		return (0);
	inter = new_moved_point(&calcul->c0, &calcul->v, c.dist);
	c.o_to_inter = vect_ab(&sprite->O.c0, &inter);
	c.u = ft_dot_product(&c.o_to_inter, &sprite->O.right) / sprite->size;
	c.v = ft_dot_product(&c.o_to_inter, &sprite->O.view) / sprite->size;
	if (c.u < EPSILON || c.u > sprite->param.texture->sz_x || 
		c.v < EPSILON || c.v > sprite->param.texture->sz_y)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_sprite(calcul, sprite, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	h_dist_sprite(t_calcul_px *calcul, t_sprite *sprite, t_plane_calc* c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = sprite;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->px_color = sprite->param.color;
	calcul->argb = sprite->param.argb;
	calcul->v_normal = sprite->O.view;

	h_img_sprite(calcul, c, sprite);
	if (sprite->param.color2.r >= 0)
		calcul->argb = (t_argb){calcul->argb.a, sprite->param.color2.r, sprite->param.color2.g, sprite->param.color2.b};

	if (c->bot > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}

void	h_img_sprite(t_calcul_px *calcul, t_plane_calc* c, t_sprite *sprite)
{
	double text_x = c->u / sprite->param.texture->sz_x;
	double text_y = 1.0 - c->v / sprite->param.texture->sz_y;

	calcul->argb = return_px_img(sprite->param.texture, text_x, text_y);

	if (sprite->param.alpha_map)
		calcul->argb = (t_argb){return_alpha_img(sprite->param.alpha_map, text_x, text_y), calcul->argb.r, calcul->argb.g, calcul->argb.b};
	if (sprite->param.normal_map)
	{
		t_vect	normal_map = return_vect_img(sprite->param.normal_map, text_x, text_y);
		calcul->v_normal = (t_vect){
			sprite->O.right.dx * normal_map.dx + sprite->O.view.dx * normal_map.dy + sprite->O.up.dx * normal_map.dz,
			sprite->O.right.dy * normal_map.dx + sprite->O.view.dy * normal_map.dy + sprite->O.up.dy * normal_map.dz,
			sprite->O.right.dz * normal_map.dx + sprite->O.view.dz * normal_map.dy + sprite->O.up.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->v_normal);
	}
}
