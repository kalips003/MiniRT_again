/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D1_dist_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/15 01:10:32 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_sphere(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, t_sphere_calc *c, int simple);
void	h_img_sphere(t_calcul_px *calcul, t_sphere *sphere);

///////////////////////////////////////////////////////////////////////////////]
// 		if colition, fills in the xyz position of the closest positive contact point
// 	return dist from sphere, -1 if no colosion or not in sight
//	(x-x0)² + (y-y0)² + (z-z0)² = R²
int	distance_from_sphere(t_calcul_px *calcul, void *obj, int simple)
{
	t_sphere_calc	c;
	t_sphere		*sphere;

	sphere = (t_sphere*)obj;

	c.x0 = calcul->c0.x - sphere->O.c0.x;
	c.y0 = calcul->c0.y - sphere->O.c0.y;
	c.z0 = calcul->c0.z - sphere->O.c0.z;
	c.a = calcul->v.dx * calcul->v.dx + calcul->v.dy * calcul->v.dy + calcul->v.dz * calcul->v.dz;
	c.b = 2 * (calcul->v.dx * c.x0 + calcul->v.dy * c.y0 + calcul->v.dz * c.z0);
	c.c = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0 - sphere->radius * sphere->radius;
	c.Δ = c.b * c.b - 4 * c.a * c.c;
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	if (c.dist <= 0.0)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_sphere(calcul, sphere, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, t_sphere_calc *c, int simple)
{
	if (simple)
		return (1);
	
	calcul->dist = c->dist;
	calcul->object = (void *)sphere;
	
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, calcul->dist);
	calcul->v_normal = vect_ab_norm(&sphere->O.c0, &calcul->inter);
	calcul->px_color = sphere->param.color;
	calcul->argb = sphere->param.argb;

	if (!sphere->param.texture && sphere->param.color2.r >= 0)
		calcul->argb = dual_color_render(&sphere->param.argb, &sphere->param.color2, ft_dot_product(&calcul->v_normal, &sphere->O.view) / 2 + 0.5);

	if (sphere->param.texture || sphere->param.normal_map || sphere->param.alpha_map)
		h_img_sphere(calcul, sphere);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
void	h_img_sphere(t_calcul_px *calcul, t_sphere *sphere)
{
	double cosθ = ft_dot_product(&calcul->v_normal, &sphere->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &sphere->O.right);
	double cosϕ = ft_dot_product(&calcul->v_normal, &sphere->O.view);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(cosϕ) / PI));

	if (sphere->param.texture)
		calcul->argb = return_px_img(sphere->param.texture, l_θ, l_ϕ);
	if (sphere->param.alpha_map)
		calcul->argb = (t_argb){return_alpha_img(sphere->param.alpha_map, l_θ, l_ϕ), calcul->argb.r, calcul->argb.g, calcul->argb.b};
	if (sphere->param.normal_map)
	{
		t_vect	normal_map = return_vect_img(sphere->param.normal_map, l_θ, l_ϕ);
		t_obj	local;
		local.view = calcul->v_normal;
		local.right = ft_cross_product_norm(&local.view, &sphere->O.view);
		local.up = ft_cross_product_norm(&local.right, &local.view);
	
		calcul->v_normal = (t_vect){
			local.right.dx * normal_map.dx + local.up.dx * normal_map.dy + local.view.dx * normal_map.dz,
			local.right.dy * normal_map.dx + local.up.dy * normal_map.dy + local.view.dy * normal_map.dz,
			local.right.dz * normal_map.dx + local.up.dz * normal_map.dy + local.view.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->v_normal);
	}
}
