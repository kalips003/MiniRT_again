/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/22 10:52:04 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"


typedef struct s_cone_calc {
	double	φ;

	double	b1;
	double	b2;
	double	b3;

	double	Φ;

	double	a1;
	double	a2;
	double	a3;
	double	slope;

	double A;
	double B;
	double C;

	double Δ;
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
} t_cone_calc;


double	distance_from_cone(t_calcul_px *calcul, t_cone *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cone(t_calcul_px *calcul, t_cone *cone)
{
	t_cone_calc	c;

	c.φ = calcul->v_view.dx * cone->v.dx + calcul->v_view.dy * cone->v.dy + calcul->v_view.dz * cone->v.dz;

	c.b1 = calcul->c0.x - cone->c0.x - cone->height * cone->v.dx;
	c.b2 = calcul->c0.y - cone->c0.y - cone->height * cone->v.dy;
	c.b3 = calcul->c0.z - cone->c0.z - cone->height * cone->v.dz;

	c.Φ = cone->v.dx * c.b1 + cone->v.dy * c.b2 + cone->v.dz * c.b3;

	c.a1 = calcul->v_view.dx - c.φ * cone->v.dx;
	c.a2 = calcul->v_view.dy - c.φ * cone->v.dy;
	c.a3 = calcul->v_view.dz - c.φ * cone->v.dz;
	c.slope = (cone->radius * cone->radius) / (cone->height * cone->height);

	c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - c.slope * c.φ * c.φ;
	c.B = c.a1 * c.b1 + c.a2 * c.b2 + c.a3 * c.b3 - 2 * c.φ * c.slope;
	c.C = pow(c.b1 - c.Φ * cone->v.dx, 2) + pow(c.b2 - c.Φ * cone->v.dy, 2) + pow(c.b3 - c.Φ * cone->v.dz, 2) - c.Φ * c.Φ * c.slope;

	c.Δ = c.B * c.B - 4 * c.A * c.C;
	if (c.Δ < EPSILON || fabs(c.A) < EPSILON)
		return (-1.0);

	c.det1 = (-c.B + sqrt(c.Δ)) / (2 * c.A);
	c.det2 = (-c.B - sqrt(c.Δ)) / (2 * c.A);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_h = c.Φ + c.φ * c.dist;// height from apex

	if (c.dist < 0.0 || c.dist_h > cone->height || c.dist_h < 0.0)//cylinder behind camera  // hit the cylinder but outside of bounds
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
		cylinder->c0.x + c->dist_h * cylinder->v.dx, 
		cylinder->c0.y + c->dist_h * cylinder->v.dy, 
		cylinder->c0.z + c->dist_h * cylinder->v.dz};

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
	t_camera	cy_cross;
	cy_cross.view = cylinder->v;
	h_camera_calc_up_right_vect(&cy_cross);
	
	t_img *texture = ((t_cylinder*)calcul->object)->texture;

	double cosθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.up);
	double sinθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.right);
	double θ = atan2(sinθ, cosθ);
	double	l_θ = fmin(1.0, fmax(0.0, θ  / (2 * PI) + 0.5));
	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)((c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;

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
	t_camera	cy_cross;
	cy_cross.view = cylinder->v;
	h_camera_calc_up_right_vect(&cy_cross);
	
	t_img *texture = ((t_sphere*)calcul->object)->normal_map;

	double cosθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.up);
	double sinθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.right);
	double θ = atan2(sinθ, cosθ);
	double	l_θ = fmin(1.0, fmax(0.0, θ  / (2 * PI) + 0.5));
	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)((c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		// (color & 0xFF) / 255.0 * 2.0 - 1.0};
		(color & 0xFF) / 255.0};
	ft_normalize_vect(&normal_map);

	t_camera	x;
	x.view = calcul->v_normal;
	x.up = cylinder->v;
	x.right = ft_vect_cross_product(&x.view, &x.up);
	ft_normalize_vect(&x.right);

	t_vect world_normal;

	// 	[NTB].[normal_map]
	// world_normal.dx = normal_map.dx * x.right.dx + normal_map.dy * x.up.dx + normal_map.dz * x.view.dx;
	// world_normal.dy = normal_map.dx * x.right.dy + normal_map.dy * x.up.dy + normal_map.dz * x.view.dy;
	// world_normal.dz = normal_map.dx * x.right.dz + normal_map.dy * x.up.dz + normal_map.dz * x.view.dz;

	// world_normal.dx = normal_map.dx * x.view.dx + normal_map.dy * x.up.dx + normal_map.dz * x.right.dx;
	// world_normal.dy = normal_map.dx * x.view.dy + normal_map.dy * x.up.dy + normal_map.dz * x.right.dy;
	// world_normal.dz = normal_map.dx * x.view.dz + normal_map.dy * x.up.dz + normal_map.dz * x.right.dz;

	normal_map.dz *= -1; // Flip depth axis if needed (opengl map)
	world_normal.dx = normal_map.dx * x.right.dx + normal_map.dy * x.up.dx + normal_map.dz * x.view.dx;
	world_normal.dy = normal_map.dx * x.right.dy + normal_map.dy * x.up.dy + normal_map.dz * x.view.dy;
	world_normal.dz = normal_map.dx * x.right.dz + normal_map.dy * x.up.dz + normal_map.dz * x.view.dz;

	ft_normalize_vect(&world_normal);

	return (normal_map);
}