/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D3_dist_cylinder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:07:55 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 00:14:18 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_cylinder(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_cylinder_1(t_calcul_px *c1, t_cylinder *cy, t_cylinder_calc_v2 *c2);
int		h_dist_cylinder_2(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c, int simple);
void	h_txt_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c);
void	h_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
int	distance_from_cylinder(t_calcul_px *calcul, void *obj, int simple)
{
	t_cylinder_calc_v2	c;
	t_cylinder *cy = (t_cylinder*)obj;
	int	rtrn;

	c.circle = (t_circle){CIRCLE, cy->O, cy->param, cy->radius};
	rtrn = distance_from_circle(calcul, &c.circle, simple);
	c.circle.O.c0 = new_moved_point(&cy->O.c0, &cy->O.view, cy->height);
	rtrn |= distance_from_circle(calcul, &c.circle, simple);

	if (rtrn && simple)
		return (1);
	if (rtrn)
		calcul->object = obj;
	if (!h_dist_cylinder_1(calcul, cy, &c))
		return (rtrn);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_cylinder_2(calcul, cy, &c, simple));
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// fills the t_cone_calc_v2 with intersection point
// if no intersection, return 0
int	h_dist_cylinder_1(t_calcul_px *c1, t_cylinder *cy, t_cylinder_calc_v2 *c2)
{
	t_cylinder_calc_v1	c;

	c.A = c1->v.dx * cy->O.view.dx + c1->v.dy * cy->O.view.dy + c1->v.dz * cy->O.view.dz;
	c.B = cy->O.view.dx * (c1->c0.x - cy->O.c0.x) + cy->O.view.dy * (c1->c0.y - cy->O.c0.y) + cy->O.view.dz * (c1->c0.z - cy->O.c0.z);
	c.x0 = c1->v.dx - c.A * cy->O.view.dx;
	c.y0 = c1->v.dy - c.A * cy->O.view.dy;
	c.z0 = c1->v.dz - c.A * cy->O.view.dz;
	c.x1 = c1->c0.x - c.B * cy->O.view.dx - cy->O.c0.x;
	c.y1 = c1->c0.y - c.B * cy->O.view.dy - cy->O.c0.y;
	c.z1 = c1->c0.z - c.B * cy->O.view.dz - cy->O.c0.z;
	c.a = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0;
	c.b = 2 * (c.x0 * c.x1 + c.y0 * c.y1 + c.z0 * c.z1);
	c.c = c.x1 * c.x1 + c.y1 * c.y1 + c.z1 * c.z1 - cy->radius * cy->radius;
	c.Δ = c.b * c.b - 4 * c.a * c.c;
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c2->det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c2->det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c2->dist = h_smalest_Δ(c2->det1, c2->det2);
	c2->dist_h = c.A * c2->dist + c.B;
	if (c2->dist < EPSILON || c2->dist_h > cy->height || c2->dist_h < 0.0)//cylinder behind camera  // hit the cylinder but outside of bounds
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
int	h_dist_cylinder_2(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cylinder;

	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cylinder->O.c0, &cylinder->O.view, c->dist_h);
	calcul->v_normal = vect_ab_norm(&c->projec_point, &calcul->inter);
	calcul->px_color = cylinder->param.color;

	if (cylinder->param.texture)
		h_txt_cylinder(calcul, cylinder, c);
	if (cylinder->param.normal_map)
		h_nmap_cylinder(calcul, cylinder, c);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_txt_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c)
{
	t_img *txt = cylinder->param.texture;

	double cosθ = ft_dot_product(&calcul->v_normal, &cylinder->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &cylinder->O.right);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));

	int text_x = ((int)floor(l_θ * txt->sz_x) + txt->sz_x) % txt->sz_x;
	int text_y = ((int)floor((1 - c->dist_h / cylinder->height) * txt->sz_y) + txt->sz_y) % txt->sz_y;
	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	calcul->px_color = (t_rgb){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
}

void	h_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c)
{
	t_img *txt = cylinder->param.normal_map;

	double cosθ = ft_dot_product(&calcul->v_normal, &cylinder->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &cylinder->O.right);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));

	int text_x = ((int)floor(l_θ * txt->sz_x) + txt->sz_x) % txt->sz_x;
	int text_y = ((int)floor((1 - c->dist_h / cylinder->height) * txt->sz_y) + txt->sz_y) % txt->sz_y;
	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(color & 0xFF) / 255.0};
	ft_normalize_vect(&normal_map);
	// normal_map.dx *= -1;// ???
	// normal_map.dy *= -1;// ???
	// normal_map.dz *= -1;// ???

	t_obj	local;
	local.view = calcul->v_normal;
	local.up = cylinder->O.view;
	local.right = ft_cross_product_norm(&local.view, &local.up);

	calcul->v_normal = (t_vect){
		ft_dot_product(&normal_map, &local.right),
		ft_dot_product(&normal_map, &local.up),
		ft_dot_product(&normal_map, &local.view)
	};
	ft_normalize_vect(&calcul->v_normal);
}