/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/23 13:42:12 by kalipso          ###   ########.fr       */
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

	// c.φ = calcul->v_view.dx * cone->v.dx + calcul->v_view.dy * cone->v.dy + calcul->v_view.dz * cone->v.dz;
	// c.Φ = cone->v.dx * (calcul->c0.x - cone->apex.x) + cone->v.dy * (calcul->c0.y - cone->apex.y) + cone->v.dz * (calcul->c0.z - cone->apex.z);
	// c.slope = (cone->radius * cone->radius) / (cone->height * cone->height);

	// c.a1 = calcul->v_view.dx - c.φ * cone->v.dx;
	// c.a2 = calcul->v_view.dy - c.φ * cone->v.dy;
	// c.a3 = calcul->v_view.dz - c.φ * cone->v.dz;

	// c.b1 = calcul->c0.x - cone->apex.x - c.Φ * cone->v.dx;
	// c.b2 = calcul->c0.y - cone->apex.y - c.Φ * cone->v.dy;
	// c.b3 = calcul->c0.z - cone->apex.z - c.Φ * cone->v.dz;

	// c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - c.slope * c.φ * c.φ;
	// c.B = 2 * c.a1 * c.b1 + 2 * c.a2 * c.b2 + 2 * c.a3 * c.b3 - 2 * c.φ * c.Φ * c.slope;
	// c.C = c.b1 * c.b1 + c.b2 * c.b2 + c.b3 * c.b3 - c.Φ * c.Φ * c.slope;

	c.φ = -(calcul->v_view.dx * cone->v.dx + calcul->v_view.dy * cone->v.dy + calcul->v_view.dz * cone->v.dz);
	c.Φ = -(cone->v.dx * (calcul->c0.x - cone->apex.x) + cone->v.dy * (calcul->c0.y - cone->apex.y) + cone->v.dz * (calcul->c0.z - cone->apex.z));
	c.slope = (cone->radius * cone->radius) / (cone->height * cone->height);

	c.a1 = -calcul->v_view.dx - c.φ * cone->v.dx;
	c.a2 = -calcul->v_view.dy - c.φ * cone->v.dy;
	c.a3 = -calcul->v_view.dz - c.φ * cone->v.dz;

	c.b1 = -calcul->c0.x + cone->apex.x - c.Φ * cone->v.dx;
	c.b2 = -calcul->c0.y + cone->apex.y - c.Φ * cone->v.dy;
	c.b3 = -calcul->c0.z + cone->apex.z - c.Φ * cone->v.dz;

	c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - c.slope * c.φ * c.φ;
	c.B = 2 * c.a1 * c.b1 + 2 * c.a2 * c.b2 + 2 * c.a3 * c.b3 - 2 * c.φ * c.Φ * c.slope;
	c.C = c.b1 * c.b1 + c.b2 * c.b2 + c.b3 * c.b3 - c.Φ * c.Φ * c.slope;

	c.Δ = c.B * c.B - 4 * c.A * c.C;
	if (c.Δ < EPSILON || fabs(c.A) < EPSILON)
		return (-1.0);

	c.det1 = (-c.B + sqrt(c.Δ)) / (2 * c.A);
	c.det2 = (-c.B - sqrt(c.Δ)) / (2 * c.A);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_h = c.Φ + c.φ * c.dist;// height from apex

	if (c.dist < 0.0 || c.dist_h > cone->height || c.dist_h < 0.0)
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

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * c->dist,
		calcul->c0.y + calcul->v_view.dy * c->dist,
		calcul->c0.z + calcul->v_view.dz * c->dist};

	c->projec_point = (t_coor){
		cone->c0.x + c->dist_h * cone->v.dx, 
		cone->c0.y + c->dist_h * cone->v.dy, 
		cone->c0.z + c->dist_h * cone->v.dz};

	calcul->v_normal = (t_vect){
		calcul->inter.x - c->projec_point.x,
		calcul->inter.y - c->projec_point.y,
		calcul->inter.z - c->projec_point.z};
	double	dot = ft_vect_dot_product(&calcul->v_normal, &cone->v);
	calcul->v_normal.dx -= (1 + c->slope) * dot * cone->v.dx;
	calcul->v_normal.dy -= (1 + c->slope) * dot * cone->v.dy;
	calcul->v_normal.dz -= (1 + c->slope) * dot * cone->v.dz;
	ft_normalize_vect(&calcul->v_normal);
	
	// calcul->px_color = cone->color;
	double	color_height = (cone->height - c->dist_h) / cone->height;
	calcul->px_color = (t_rgb){
		(int)((cone->color2.r - cone->color.r) * color_height + cone->color.r),
		(int)((cone->color2.g - cone->color.g) * color_height + cone->color.g),
		(int)((cone->color2.b - cone->color.b) * color_height + cone->color.b)
	};

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}
