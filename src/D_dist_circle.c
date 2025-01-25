/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_circle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/24 14:53:40 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_cicle_v2(t_calcul_px *calcul, t_circle_v2 circle);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
double	distance_from_cicle_v2(t_calcul_px *calcul, t_circle_v2 circle)
{
	t_plane_calc	c;
	t_coor	inter_temp;
	double	d = -(circle.v.dx * circle.c0.x + circle.v.dy * circle.c0.y + circle.v.dz * circle.c0.z);

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(circle.v.dx * calcul->c0.x + circle.v.dy * calcul->c0.y + circle.v.dz * calcul->c0.z + d);
	c.bot = circle.v.dx * calcul->v_view.dx + circle.v.dy * calcul->v_view.dy + circle.v.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to d plane
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (-1.0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0)
		return (-1.0);
	inter_temp = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist,
			calcul->c0.y + calcul->v_view.dy * c.dist,
			calcul->c0.z + calcul->v_view.dz * c.dist};

	if (dist_two_points(&inter_temp, &circle.c0) > circle.radius)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
	{
		calcul->dist = c.dist;
		calcul->px_color = ((t_cylinder *)(circle.obj))->color;

		calcul->inter = inter_temp;

		calcul->v_normal = circle.v;
		if (ft_vect_dot_product(&calcul->v_view, &circle.v) > 0.0)
			calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
		// ft_normalize_vect(&calcul->v_normal);
		calcul->object = circle.obj;
	}
	return (c.dist);
}

// void	h_dist_circle(t_calcul_px *calcul, t_circle circle);