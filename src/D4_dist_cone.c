/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D4_dist_cone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 00:17:20 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
int		distance_from_cone(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_cone_1(t_calcul_px *c1, t_cone *cone, t_cone_calc_v2 *c2);
int		h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
int	distance_from_cone(t_calcul_px *calcul, void *obj, int simple)
{
	t_cone_calc_v2	c;
	t_cone	*cone = (t_cone*)obj;
	int	rtrn;

	c.circle = (t_circle){CIRCLE, cone->O, cone->param, cone->radius};
	rtrn = distance_from_circle(calcul, &c.circle, simple);
	
	if (rtrn && simple)
		return (1);
	if (rtrn)
		calcul->object = obj;

	if (!h_dist_cone_1(calcul, cone, &c))
		return (rtrn);
	if (c.dist < calcul->dist || calcul->dist <= 0.0)
		return (h_dist_cone_2(calcul, cone, &c, simple));
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// fills the t_cone_calc_v2 with intersection point
// if no intersection, return 0
int	h_dist_cone_1(t_calcul_px *c1, t_cone *cone, t_cone_calc_v2 *c2)
{
	t_cone_calc_v1	c;

	c.φ = c1->v.dx * cone->O.view.dx + c1->v.dy * cone->O.view.dy + c1->v.dz * cone->O.view.dz;
	c.Φ = cone->O.view.dx * (c1->c0.x - cone->apex.x) + cone->O.view.dy * (c1->c0.y - cone->apex.y) + cone->O.view.dz * (c1->c0.z - cone->apex.z);
	c.a1 = c1->v.dx - c.φ * cone->O.view.dx;
	c.a2 = c1->v.dy - c.φ * cone->O.view.dy;
	c.a3 = c1->v.dz - c.φ * cone->O.view.dz;
	c.b1 = c1->c0.x - cone->apex.x - c.Φ * cone->O.view.dx;
	c.b2 = c1->c0.y - cone->apex.y - c.Φ * cone->O.view.dy;
	c.b3 = c1->c0.z - cone->apex.z - c.Φ * cone->O.view.dz;
	c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - cone->slope * c.φ * c.φ;
	c.B = 2 * c.a1 * c.b1 + 2 * c.a2 * c.b2 + 2 * c.a3 * c.b3 - 2 * c.φ * c.Φ * cone->slope;
	c.C = c.b1 * c.b1 + c.b2 * c.b2 + c.b3 * c.b3 - c.Φ * c.Φ * cone->slope;
	c.Δ = c.B * c.B - 4 * c.A * c.C;

	if (c.Δ < EPSILON || fabs(c.A) < EPSILON)
		return (0);
	c2->det1 = (-c.B + sqrt(c.Δ)) / (2 * c.A);
	c2->det2 = (-c.B - sqrt(c.Δ)) / (2 * c.A);
	c2->dist = h_smalest_Δ(c2->det1, c2->det2);
	c2->dist_apex = -c.Φ + -c.φ * c2->dist;
	if (c2->dist < EPSILON || c2->dist_apex > cone->height || c2->dist_apex < 0.0)
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// if closest object, update t_calcul
int	h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple)
{
	double	dot_p;
	double	color_height;
	
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cone;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cone->O.c0, &cone->O.view, c->dist_apex);
	calcul->v_normal = vect_ab(&c->projec_point, &calcul->inter);
	dot_p = ft_dot_product(&calcul->v_normal, &cone->O.view);
	calcul->v_normal.dx -= (1 + cone->slope) * dot_p * cone->O.view.dx;
	calcul->v_normal.dy -= (1 + cone->slope) * dot_p * cone->O.view.dy;
	calcul->v_normal.dz -= (1 + cone->slope) * dot_p * cone->O.view.dz;
	ft_normalize_vect(&calcul->v_normal);

	color_height = (cone->height - c->dist_apex) / cone->height;
	calcul->px_color = (t_rgb){
		(int)((cone->color2.r - cone->param.color.r) * color_height + cone->param.color.r),
		(int)((cone->color2.g - cone->param.color.g) * color_height + cone->param.color.g),
		(int)((cone->color2.b - cone->param.color.b) * color_height + cone->param.color.b)
	};
	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}
