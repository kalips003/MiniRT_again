/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D7_dist_cube.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/15 02:41:44 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"


typedef struct s_cube_calc {
	t_vect	o_to_inter;
	double	top;
	double	bot;

	double	dist;

	double	u;
	double	v;

	t_vect	*main_v;
	t_vect	*v2;
	t_vect	*v3;

	t_coor	inter;
	t_vect	v_norm;
} t_cube_calc;

int	distance_from_cube(t_calcul_px *calcul, void *obj, int simple);
int	h_dist_cube1(t_calcul_px *calcul, t_cube *cube, t_cube_calc *c, int simple);
void	h_dist_cube2(t_calcul_px *calcul, t_cube *cube, t_cube_calc* c);
void	h_txt_cube(t_calcul_px *calcul, t_cube_calc* c, t_cube *plane);
void	h_nmap_cube(t_calcul_px *calcul, t_cube_calc* c, t_cube *plane);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
// top = signed distance camera plane ~ cos angle
// bot = dot product view vector / plane vector (means nothing)
int	distance_from_cube(t_calcul_px *calcul, void *obj, int simple)
{
	t_cube_calc	c;
	t_cube *cube = (t_cube*)obj;
	int	rtrn;
	
	cube->other_p = (t_coor){
		cube->O.c0.x + cube->size * (cube->O.view.dx + cube->O.up.dx + cube->O.right.dx),
		cube->O.c0.y + cube->size * (cube->O.view.dy + cube->O.up.dy + cube->O.right.dy),
		cube->O.c0.z + cube->size * (cube->O.view.dz + cube->O.up.dz + cube->O.right.dz)
	};

	c.main_v = &cube->O.view;
	c.v2 = &cube->O.up;
	c.v3 = &cube->O.right;
	rtrn = h_dist_cube1(calcul, cube, &c, simple);
	c.main_v = &cube->O.up;
	c.v2 = &cube->O.view;
	rtrn |= h_dist_cube1(calcul, cube, &c, simple);
	c.main_v = &cube->O.right;
	c.v3 = &cube->O.up;
	rtrn |= h_dist_cube1(calcul, cube, &c, simple);

	if (rtrn && simple)
		return (1);
	if (rtrn)
		h_dist_cube2(calcul, cube, &c);
	return (rtrn);
}

int	h_dist_cube1(t_calcul_px *calcul, t_cube *cube, t_cube_calc *c, int simple)
{
	double da1 = -(c->main_v->dx * cube->O.c0.x + c->main_v->dy * cube->O.c0.y + c->main_v->dz * cube->O.c0.z);
	double da2 = -(c->main_v->dx * cube->other_p.x + c->main_v->dy * cube->other_p.y + c->main_v->dz * cube->other_p.z);

	double top = -(c->main_v->dx * calcul->c0.x + c->main_v->dy * calcul->c0.y + c->main_v->dz * calcul->c0.z + da1);
	double top2 = top + da1 - da2;
	double bot = c->main_v->dx * calcul->v.dx + c->main_v->dy * calcul->v.dy + c->main_v->dz * calcul->v.dz;
	
	if (fabs(bot) < EPSILON)
		return (0);
	double dist = top / bot;
	double dist2 = top2 / bot;
	if (dist < EPSILON && dist2 < EPSILON)
		return (0);
	dist = h_smalest_Δ(dist, dist2);

	t_coor	inter = new_moved_point(&calcul->c0, &calcul->v, dist);
	t_vect	o_to_inter = vect_ab(&cube->O.c0, &inter);
	double dot1 = ft_dot_product(c->v2, &o_to_inter);
	double dot2 = ft_dot_product(c->v3, &o_to_inter);

	if (dot1 < 0.0 || dot2 < 0.0 || dot1 > cube->size || dot2 > cube->size)
		return (0);
	if (simple)
		return (1);
	if (calcul->dist > 0.0 && dist > calcul->dist)
		return (0);
	c->dist = dist;
	c->inter = inter;
	c->v_norm = *c->main_v;
	c->o_to_inter = o_to_inter;
	c->u = dot1 / cube->size;
	c->v = dot2 / cube->size;
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_cube2(t_calcul_px *calcul, t_cube *cube, t_cube_calc* c)
{
	calcul->dist = c->dist;
	calcul->object = cube;
	calcul->inter = c->inter;
	calcul->px_color = cube->param.color;
	calcul->argb = cube->param.argb;
	calcul->v_normal = c->v_norm;

	if (!cube->param.texture && cube->param.color2.r >= 0)
	{
		t_vect	o_to_inter = vect_ab(&cube->O.c0, &c->inter);
		t_vect	diag = vect_ab_norm(&cube->O.c0, &cube->other_p);
		double	dist_color = ft_dot_product(&o_to_inter, &diag) / (SQRT3 * cube->size);
		calcul->argb = dual_color_render(&cube->param.argb, &cube->param.color2, dist_color);
	}
	if (cube->param.texture || cube->param.normal_map || cube->param.alpha_map)
		h_img_cube(calcul, cube, c);

	if (ft_dot_product(&calcul->v, &c->v_norm) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
}


void	h_img_cube(t_calcul_px *calcul, t_cube *cube, t_cube_calc* c)
{
	if (cube->param.texture)
		calcul->argb = return_px_img(cube->param.texture, c->u, c->v);
	if (cube->param.alpha_map)
		calcul->argb = (t_argb){return_alpha_img(cube->param.alpha_map, c->u, c->v), calcul->argb.r, calcul->argb.g, calcul->argb.b};
	if (cube->param.normal_map)
	{
		t_vect	normal_map = return_vect_img(cube->param.normal_map, c->u, c->v);
		calcul->v_normal = (t_vect){
			cube->O.right.dx * normal_map.dx + cube->O.up.dx * normal_map.dy + c->v_norm.dx * normal_map.dz,
			cube->O.right.dy * normal_map.dx + cube->O.up.dy * normal_map.dy + c->v_norm.dy * normal_map.dz,
			cube->O.right.dz * normal_map.dx + cube->O.up.dz * normal_map.dy + c->v_norm.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->v_normal);
	}
}
