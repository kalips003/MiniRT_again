/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D4_dist_cone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/19 17:29:15 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
int		distance_from_cone(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_cone_1(t_calcul_px *c1, t_cone *cone, t_cone_calc_v2 *c2);
int		h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple);
void	h_img_cone(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c);

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
	c.det1 = (-c.B + sqrt(c.Δ)) / (2 * c.A);
	c.det2 = (-c.B - sqrt(c.Δ)) / (2 * c.A);
	c.dist_apex1 = -c.Φ + -c.φ * c.det1;
	c.dist_apex2 = -c.Φ + -c.φ * c.det2;
	if (!h_colision_cone(c1, cone, &c, c2))
		return (0);
	c2->dist_apex = -c.Φ - c.φ * c2->dist;
	return (1);
}

int	h_colision_cone(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v1 *c, t_cone_calc_v2 *c2)
{
	int d1_valid;
	int d2_valid;
	double dot;

	
	c2->apex_to_camera = vect_ab_norm(&cone->apex, &calcul->c0);
	dot = -ft_dot_product(&cone->O.view, &c2->apex_to_camera);
	if (calcul->print)
		printf("dot= %.3f, cone->angle= %.3f, cos(angle)= %.3f\n", dot, cone->angle, cos(cone->angle));
	if (-c->Φ > EPSILON && -c->Φ < cone->height && dot > EPSILON && dot > cos(cone->angle))
		c2->inside = 1;
	else
		c2->inside = 0;
	
	d1_valid = (c->det1 > EPSILON && c->dist_apex1 < cone->height && c->dist_apex1 > EPSILON);
	d2_valid = (c->det2 > EPSILON && c->dist_apex2 < cone->height && c->dist_apex2 > EPSILON);
	if (d1_valid && d2_valid)
		c2->dist = fmin(c->det1, c->det2);
	else if (d1_valid)
		c2->dist = c->det1;
	else if (d2_valid)
		c2->dist = c->det2;
	else
		return (0);
	return (1);
}
///////////////////////////////////////////////////////////////////////////////]
// if closest object, update t_calcul
int	h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple)
{
	double	dot_p;

	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cone;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cone->apex, &cone->O.view, -c->dist_apex);

	
	calcul->v_normal = vect_ab_norm(&c->projec_point, &calcul->inter);
	t_vect	axis = ft_cross_product(&calcul->v_normal, &cone->O.view);
	calcul->v_normal = quaternion_rotation(&calcul->v_normal, &axis, cone->angle, 1);

	if (c->inside)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	
	calcul->argb = cone->param.argb;
	
	c->color_height = 1.0 - (cone->height - c->dist_apex) / cone->height;
	if (!cone->param.texture && cone->param.color2.r >= 0)
		calcul->argb = dual_color_render(&cone->param.argb, &cone->param.color2, c->color_height);
	
	if (cone->param.texture || cone->param.normal_map || cone->param.alpha_map)
		h_img_cone(calcul, cone, c);
	
	// if (c->det1 < 0.0 || c->det2 < 0.0)
	if (calcul->print)
		printf("inside: %d\n", c->inside);
	return (1);
}

int	h_dist_cone_2_v1(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple)
{
	double	dot_p;

	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cone;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cone->apex, &cone->O.view, -c->dist_apex);

	
	calcul->v_normal = vect_ab(&c->projec_point, &calcul->inter);
	dot_p = ft_dot_product(&calcul->v_normal, &cone->O.view);
	calcul->v_normal.dx -= dot_p * cone->O.view.dx * cone->slope;
	calcul->v_normal.dy -= dot_p * cone->O.view.dy * cone->slope;
	calcul->v_normal.dz -= dot_p * cone->O.view.dz * cone->slope;
	ft_normalize_vect(&calcul->v_normal);

	calcul->argb = cone->param.argb;
	
	c->color_height = (cone->height - c->dist_apex) / cone->height;
	if (!cone->param.texture && cone->param.color2.r >= 0)
		calcul->argb = dual_color_render(&cone->param.argb, &cone->param.color2, c->color_height);

	if (cone->param.texture || cone->param.normal_map || cone->param.alpha_map)
		h_img_cone(calcul, cone, c);

	// if (c->det1 < 0.0 || c->det2 < 0.0)
	if (c->inside)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	if (calcul->print)
		printf("inside: %d\n", c->inside);
	return (1);
}
///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
void	h_img_cone(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c)
{
	double cosθ = ft_dot_product(&calcul->v_normal, &cone->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &cone->O.right);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));

	if (cone->param.texture)
		calcul->argb = return_px_img(cone->param.texture, l_θ, c->color_height);
	if (cone->param.alpha_map)
		calcul->argb = (t_argb){return_alpha_img(cone->param.alpha_map, l_θ, c->color_height), calcul->argb.r, calcul->argb.g, calcul->argb.b};
	if (cone->param.normal_map)
	{
		t_vect	normal_map = return_vect_img(cone->param.normal_map, l_θ, c->color_height);
		t_obj	local;
		local.view = calcul->v_normal;
		local.right = ft_cross_product_norm(&local.view, &cone->O.view);
		local.up = ft_cross_product_norm(&local.right, &local.view);
	
		calcul->v_normal = (t_vect){
			local.right.dx * normal_map.dx + local.up.dx * normal_map.dy + local.view.dx * normal_map.dz,
			local.right.dy * normal_map.dx + local.up.dy * normal_map.dy + local.view.dy * normal_map.dz,
			local.right.dz * normal_map.dx + local.up.dz * normal_map.dy + local.view.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->v_normal);
	}
}
