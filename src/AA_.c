/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]

typedef struct s_cone_calc_v1 {
	double	φ;
	double	Φ;

	double	a1;
	double	a2;
	double	a3;

	double	b1;
	double	b2;
	double	b3;

	double A;
	double B;
	double C;

	double Δ;
} t_cone_calc_v1;

typedef struct s_cone_calc_v2 {
	double	det1;
	double	det2;

	double	dist_apex;
	double	dist;
	t_coor	projec_point;
	t_circle_v2	circle;
} t_cone_calc_v2;


int	distance_from_cone_v3(t_calcul_px *calcul, void *obj)
{
	t_cone_calc_v2	c;
	t_cone	*cone = (t_cone*)obj;

	if (!h_dist_cone_1(calcul, cone, &c))
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_cone_2(calcul, cone, &c);
	
	c.circle = (t_circle_v2){0, obj, cone->O.c0, cone->radius, cone->O.view};
	distance_from_cicle_v3(calcul, &c.circle);
	return (1);
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
void	h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c)
{
	double	dot_p;
	double	color_height;
	
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
		(int)((cone->color2.r - cone->color.r) * color_height + cone->color.r),
		(int)((cone->color2.g - cone->color.g) * color_height + cone->color.g),
		(int)((cone->color2.b - cone->color.b) * color_height + cone->color.b)
	};
	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
}

void	recalculate_cone(t_cone *cone)
{
	cone->apex = new_moved_point(&cone->O.c0, &cone->O.view, cone->height);
	cone->apex = (t_coor){
		cone->O.c0.x + cone->height * cone->O.view.dx,
		cone->O.c0.y + cone->height * cone->O.view.dy,
		cone->O.c0.z + cone->height * cone->O.view.dz};
	cone->slope = (cone->radius * cone->radius) / (cone->height * cone->height);
}


///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]

void	recalculate_cylinder(t_cylinder *cy)
{
	cy->xyz_other = new_moved_point(&cy->O.c0, &cy->O.view, cy->height);
}

typedef struct s_cylinder_calc_v1 {

	double	A;
	double	B;
	
	double x0;
	double y0;
	double z0;
	double x1;
	double y1;
	double z1;

	double a;
	double b;
	double c;

	double Δ;
} t_cylinder_calc_v1;

typedef struct s_cylinder_calc_v2 {
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
	t_circle_v2	circle;
} t_cylinder_calc_v2;

int	distance_from_cylinder_v3(t_calcul_px *calcul, void *obj)
{
	t_cylinder_calc_v2	c;
	t_cylinder *cy = (t_cylinder*)obj;
	int	rtrn;


	c.circle = (t_circle_v2){0, obj, cy->O.c0, cy->radius, cy->O.view};
	rtrn = distance_from_cicle_v3(calcul, &c.circle);
	c.circle = (t_circle_v2){0, obj, cy->xyz_other, cy->radius, cy->O.view};
	rtrn &= distance_from_cicle_v3(calcul, &c.circle);

	if (!h_dist_cylinder_1(calcul, cy, &c))
		return (rtrn);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_cylinder_2(calcul, cy, &c);
	return (1);
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
void	h_dist_cone_2(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c)
{
	calcul->dist = c->dist;
	calcul->object = cylinder;

	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cylinder->O.c0, &cylinder->O.view, c->dist_h);
	calcul->v_normal = vect_ab_norm(&c->projec_point, &calcul->inter);
	calcul->px_color = cylinder->color;

	if (cylinder->texture)
		h_txt_cylinder(calcul, cylinder, c);

	if (cylinder->normal_map)
		h_nmap_cylinder(calcul, cylinder, c);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}

void	h_txt_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc_v2 *c)
{
	t_img *txt = cylinder->texture;

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
	t_img *txt = cylinder->normal_map;

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