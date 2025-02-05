/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/31 15:25:51 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_cone(t_calcul_px *calcul, t_cone *cy);
void	h_dist_cone(t_calcul_px *calcul, t_cone *cone, t_cone_calc *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cone(t_calcul_px *calcul, t_cone *cone)
{
	t_cone_calc	c;

	cone->apex = (t_coor){
		cone->O.c0.x + cone->height * cone->O.view.dx,
		cone->O.c0.y + cone->height * cone->O.view.dy,
		cone->O.c0.z + cone->height * cone->O.view.dz};

	c.φ = calcul->v.dx * cone->O.view.dx + calcul->v.dy * cone->O.view.dy + calcul->v.dz * cone->O.view.dz;
	c.Φ = cone->O.view.dx * (calcul->c0.x - cone->apex.x) + cone->O.view.dy * (calcul->c0.y - cone->apex.y) + cone->O.view.dz * (calcul->c0.z - cone->apex.z);
	c.slope = (cone->radius * cone->radius) / (cone->height * cone->height);

	c.a1 = calcul->v.dx - c.φ * cone->O.view.dx;
	c.a2 = calcul->v.dy - c.φ * cone->O.view.dy;
	c.a3 = calcul->v.dz - c.φ * cone->O.view.dz;

	c.b1 = calcul->c0.x - cone->apex.x - c.Φ * cone->O.view.dx;
	c.b2 = calcul->c0.y - cone->apex.y - c.Φ * cone->O.view.dy;
	c.b3 = calcul->c0.z - cone->apex.z - c.Φ * cone->O.view.dz;

	c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - c.slope * c.φ * c.φ;
	c.B = 2 * c.a1 * c.b1 + 2 * c.a2 * c.b2 + 2 * c.a3 * c.b3 - 2 * c.φ * c.Φ * c.slope;
	c.C = c.b1 * c.b1 + c.b2 * c.b2 + c.b3 * c.b3 - c.Φ * c.Φ * c.slope;

	c.Δ = c.B * c.B - 4 * c.A * c.C;
	if (c.Δ < EPSILON || fabs(c.A) < EPSILON)
		return (-1.0);

	c.det1 = (-c.B + sqrt(c.Δ)) / (2 * c.A);
	c.det2 = (-c.B - sqrt(c.Δ)) / (2 * c.A);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_apex = -c.Φ + -c.φ * c.dist;// height from apex

	if (c.dist < EPSILON || c.dist_apex > cone->height || c.dist_apex < 0.0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_cone(calcul, cone, &c);
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_cone(t_calcul_px *calcul, t_cone *cone, t_cone_calc *c)
{
	calcul->dist = c->dist;
	calcul->object = (void *)cone;

	// calcul->inter = new_moved_point(&calcul->c0, &calcul->v_view, c->dist);
	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v.dx * c->dist,
		calcul->c0.y + calcul->v.dy * c->dist,
		calcul->c0.z + calcul->v.dz * c->dist};

	// c->projec_point = new_moved_point(&cone->O.c0, &cone->O.view, c->dist_apex);
	c->projec_point = (t_coor){
		cone->O.c0.x + c->dist_apex * cone->O.view.dx, 
		cone->O.c0.y + c->dist_apex * cone->O.view.dy, 
		cone->O.c0.z + c->dist_apex * cone->O.view.dz};

	// calcul->v_normal = vect_ab(&c->projec_point, &calcul->inter);
	calcul->v_normal = (t_vect){
		calcul->inter.x - c->projec_point.x,
		calcul->inter.y - c->projec_point.y,
		calcul->inter.z - c->projec_point.z};
	double	dot = ft_dot_product(&calcul->v_normal, &cone->O.view);
	calcul->v_normal.dx -= (1 + c->slope) * dot * cone->O.view.dx;
	calcul->v_normal.dy -= (1 + c->slope) * dot * cone->O.view.dy;
	calcul->v_normal.dz -= (1 + c->slope) * dot * cone->O.view.dz;
	ft_normalize_vect(&calcul->v_normal);
	
	// calcul->px_color = cone->color;
	double	color_height = (cone->height - c->dist_apex) / cone->height;
	calcul->px_color = (t_rgb){
		(int)((cone->color2.r - cone->color.r) * color_height + cone->color.r),
		(int)((cone->color2.g - cone->color.g) * color_height + cone->color.g),
		(int)((cone->color2.b - cone->color.b) * color_height + cone->color.b)
	};

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}
