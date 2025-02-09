/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DD_dist_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 16:56:13 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_sphere(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, t_sphere_calc *c, int simple);
void	h_txt_sphere(t_calcul_px *calcul, t_sphere *sphere);
void	h_nmap_sphere(t_calcul_px *calcul, t_sphere *sphere, t_sphere_calc *c);

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


	if (sphere->param.texture)
		h_txt_sphere(calcul, sphere);
	if (sphere->param.normal_map)
		h_nmap_sphere(calcul, sphere, c);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
void	h_txt_sphere(t_calcul_px *calcul, t_sphere *sphere)
{
	t_img *txt = sphere->param.texture;

	double cosθ = ft_dot_product(&calcul->v_normal, &sphere->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &sphere->O.right);
	double cosϕ = ft_dot_product(&calcul->v_normal, &sphere->O.view);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(cosϕ) / PI));

	int text_x = ((int)floor(l_θ * txt->sz_x) + txt->sz_x) % txt->sz_x;
	int text_y = ((int)floor(l_ϕ * txt->sz_y) + txt->sz_y) % txt->sz_y;
	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	calcul->px_color = (t_rgb){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
}

///////////////////////////////////////////////////////////////////////////////]
void	h_nmap_sphere(t_calcul_px *calcul, t_sphere *sphere, t_sphere_calc *c)
{
	t_img *nmap = sphere->param.normal_map;

	double cosθ = ft_dot_product(&calcul->v_normal, &sphere->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &sphere->O.right);
	double cosϕ = ft_dot_product(&calcul->v_normal, &sphere->O.view);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(cosϕ) / PI));

	int text_x = ((int)floor(l_θ * nmap->sz_x) + nmap->sz_x) % nmap->sz_x;
	int text_y = ((int)floor(l_ϕ * nmap->sz_y) + nmap->sz_y) % nmap->sz_y;
	char *pixel = nmap->addr + (text_y * nmap->ll + text_x * (nmap->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect normal_map = (t_vect){
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(color & 0xFF) / 255.0};
	ft_normalize_vect(&normal_map);
	// normal_map.dz *= -1;// ???
	// normal_map.dy *= -1;// ???
	// normal_map.dx *= -1;// ???

	t_obj	local;// ! if both aligned, cross product not defined?
	local.view = calcul->v_normal;
	local.right = ft_cross_product_norm(&local.view, &sphere->O.view);
	local.up = ft_cross_product_norm(&local.right, &local.view);

	
	calcul->v_normal = (t_vect){
		ft_dot_product(&normal_map, &local.right),
		ft_dot_product(&normal_map, &local.up),
		ft_dot_product(&normal_map, &local.view)
	};
	ft_normalize_vect(&calcul->v_normal);
}