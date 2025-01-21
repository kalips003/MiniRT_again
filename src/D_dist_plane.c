/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/20 10:56:18 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_plane_v2(t_calcul_px *calcul, t_plane *p);
void	h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane_v2(t_calcul_px *calcul, t_plane *p)
{
	t_plane_calc	c;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(p->abc.dx * calcul->c0.x + p->abc.dy * calcul->c0.y + p->abc.dz * calcul->c0.z + p->d);
	c.bot = p->abc.dx * calcul->v_view.dx + p->abc.dy * calcul->v_view.dy + p->abc.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (-1.0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_plane(calcul, p, &c);
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c)
{
	calcul->dist = c->dist;
	calcul->object = (void *)plane;

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * c->dist,
		calcul->c0.y + calcul->v_view.dy * c->dist,
		calcul->c0.z + calcul->v_view.dz * c->dist};
	calcul->v_normal = plane->abc;

	calcul->px_color = plane->color;
		
	if (ft_vect_dot_product(&calcul->v_view, &plane->abc) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
}