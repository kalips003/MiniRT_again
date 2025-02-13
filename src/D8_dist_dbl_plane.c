/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D8_dist_dbl_plane.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/13 13:49:50 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"


typedef struct s_dblplane_calc {
	t_vect	o_to_inter;
	double	top;
	double	bot;

	double	dist;

	double	u;
	double	v;
	
	double	top2;
	double	dist2;
} t_dblplane_calc;


int	distance_from_dblplane(t_calcul_px *calcul, void *obj, int simple);
int	h_dist_dblplane(t_calcul_px *calcul, t_dblplane *plane, t_dblplane_calc* c, int simple);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
// top = signed distance camera plane ~ cos angle
// bot = dot product view vector / plane vector (means nothing)
int	distance_from_dblplane(t_calcul_px *calcul, void *obj, int simple)
{
	t_dblplane_calc	c;
	t_dblplane	*bpl = (t_dblplane*)obj;
	
	c.top = -(bpl->O.view.dx * calcul->c0.x + bpl->O.view.dy * calcul->c0.y + bpl->O.view.dz * calcul->c0.z + bpl->d);
	c.top2 = -(bpl->O.view.dx * calcul->c0.x + bpl->O.view.dy * calcul->c0.y + bpl->O.view.dz * calcul->c0.z + bpl->other_d);
	c.bot = bpl->O.view.dx * calcul->v.dx + bpl->O.view.dy * calcul->v.dy + bpl->O.view.dz * calcul->v.dz;
	
	if (fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	c.dist2 = c.top2 / c.bot;

	if (c.dist <= EPSILON && c.dist2 <= EPSILON)
		return (0);
	if (simple)
		return (1);
	c.dist = h_smalest_Δ(c.dist, c.dist2);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_dblplane(calcul, bpl, &c, simple));
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
int	h_dist_dblplane(t_calcul_px *calcul, t_dblplane *plane, t_dblplane_calc* c, int simple)
{
	calcul->dist = c->dist;
	calcul->object = plane;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->px_color = plane->param.color;
	calcul->v_normal = plane->O.view;

	if (plane->param.color2.r >= 0 || plane->param.texture || plane->param.normal_map)
	{
		c->o_to_inter = vect_ab(&calcul->inter, &plane->O.c0);
		c->u = ft_dot_product(&c->o_to_inter, &plane->O.right) * (plane->param.gamma);
		c->v = ft_dot_product(&c->o_to_inter, &plane->O.up) * (plane->param.gamma);
	}
	if (!plane->param.texture && plane->param.color2.r >= 0)
		if (((int)floor(c->u) + (int)floor(c->v)) % 2)
			calcul->px_color = plane->param.color2;

	if (plane->param.texture)
		h_txt_plane(calcul, (t_plane_calc*)c, (t_plane*)plane);
	if (plane->param.normal_map)
		h_nmap_plane(calcul, (t_plane_calc*)c, (t_plane*)plane);
	
	if (c->bot > 0.0)
	// if (ft_dot_product(&calcul->v, &plane->O.view) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

	if (c->dist <= EPSILON || c->dist2 <= EPSILON)
		return (2);// inside the wall
	return (1);
}
