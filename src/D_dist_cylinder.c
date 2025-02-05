/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/31 16:44:53 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy)
{
	t_cylinder_calc	c;

	cy->xyz_other = (t_coor){
		cy->O.c0.x + cy->height * cy->O.view.dx,
		cy->O.c0.y + cy->height * cy->O.view.dy,
		cy->O.c0.z + cy->height * cy->O.view.dz};

	c.radius = cy->diameter / 2;
	// (P - E).W = At + B
	c.A = calcul->v.dx * cy->O.view.dx + calcul->v.dy * cy->O.view.dy + calcul->v.dz * cy->O.view.dz;
	c.B = cy->O.view.dx * (calcul->c0.x - cy->O.c0.x) + cy->O.view.dy * (calcul->c0.y - cy->O.c0.y) + cy->O.view.dz * (calcul->c0.z - cy->O.c0.z);
	
	// (P - E) - ((P - E).W) * W = {X0t + X1, Y0t + Y1, Z0t + Z1};
	c.x0 = calcul->v.dx - c.A * cy->O.view.dx;
	c.y0 = calcul->v.dy - c.A * cy->O.view.dy;
	c.z0 = calcul->v.dz - c.A * cy->O.view.dz;
	c.x1 = calcul->c0.x - c.B * cy->O.view.dx - cy->O.c0.x;
	c.y1 = calcul->c0.y - c.B * cy->O.view.dy - cy->O.c0.y;
	c.z1 = calcul->c0.z - c.B * cy->O.view.dz - cy->O.c0.z;

	c.a = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0;
	c.b = 2 * (c.x0 * c.x1 + c.y0 * c.y1 + c.z0 * c.z1);
	c.c = c.x1 * c.x1 + c.y1 * c.y1 + c.z1 * c.z1 - c.radius * c.radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_h = c.A * c.dist + c.B;

	if (c.dist < 0.0 || c.dist_h > cy->height || c.dist_h < 0.0)//cylinder behind camera  // hit the cylinder but outside of bounds
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_cylinder(calcul, cy, &c);
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	calcul->dist = c->dist;
	calcul->object = (void *)cylinder;

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v.dx * c->dist,
		calcul->c0.y + calcul->v.dy * c->dist,
		calcul->c0.z + calcul->v.dz * c->dist};

	c->projec_point = (t_coor){
		cylinder->O.c0.x + c->dist_h * cylinder->O.view.dx, 
		cylinder->O.c0.y + c->dist_h * cylinder->O.view.dy, 
		cylinder->O.c0.z + c->dist_h * cylinder->O.view.dz};

	calcul->v_normal = (t_vect){
		calcul->inter.x - c->projec_point.x,
		calcul->inter.y - c->projec_point.y,
		calcul->inter.z - c->projec_point.z};
	ft_normalize_vect(&calcul->v_normal);
	
	if (cylinder->texture)
		calcul->px_color = ft_textures_cylinder(calcul, cylinder, c);
	else
		calcul->px_color = cylinder->color;

	if (cylinder->normal_map)
		calcul->v_normal = ft_nmap_cylinder(calcul, cylinder, c);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}

///////////////////////////////////////////////////////////////////////////////]
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	t_img *texture = ((t_cylinder*)calcul->object)->texture;

	double cosθ = ft_dot_product(&calcul->v_normal, &cylinder->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &cylinder->O.right);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));

	// int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	// int text_y = (int)((1 - c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;
	int text_x = ((int)floor(l_θ * texture->sz_x) + texture->sz_x) % texture->sz_x;
	int text_y = ((int)floor((1 - c->dist_h / cylinder->height) * texture->sz_y) + texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_rgb	rtrn = {
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};

	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	t_img *texture = ((t_cylinder*)calcul->object)->normal_map;

	double cosθ = ft_dot_product(&calcul->v_normal, &cylinder->O.up);
	double sinθ = ft_dot_product(&calcul->v_normal, &cylinder->O.right);
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));
	int text_x = ((int)floor(l_θ * texture->sz_x) + texture->sz_x) % texture->sz_x;
	int text_y = ((int)floor((1 - c->dist_h / cylinder->height) * texture->sz_y) + texture->sz_y) % texture->sz_y;
	// int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	// int text_y = (int)((c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		// (color & 0xFF) / 255.0 * 2.0 - 1.0};
		(color & 0xFF) / 255.0};
	ft_normalize_vect(&normal_map);
	// normal_map.dz *= -1;// ???
	normal_map.dy *= -1;// ???
	normal_map.dx *= -1;// ???

	t_obj	local;
	local.view = calcul->v_normal;
	local.up = cylinder->O.view;
	local.right = ft_cross_product(&local.view, &local.up);
	ft_normalize_vect(&local.right);

	t_vect world_normal = {
		ft_dot_product(&normal_map, &local.right),
		ft_dot_product(&normal_map, &local.up),
		ft_dot_product(&normal_map, &local.view)
	};
	ft_normalize_vect(&world_normal);

	// normal_map.dz *= -1; // Flip depth axis if needed (opengl map)
	// world_normal.dx = normal_map.dx * x.right.dx + normal_map.dy * x.up.dx + normal_map.dz * x.view.dx;
	// world_normal.dy = normal_map.dx * x.right.dy + normal_map.dy * x.up.dy + normal_map.dz * x.view.dy;
	// world_normal.dz = normal_map.dx * x.right.dz + normal_map.dy * x.up.dz + normal_map.dz * x.view.dz;
	return (world_normal);
}