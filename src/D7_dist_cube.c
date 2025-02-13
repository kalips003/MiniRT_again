/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D7_dist_cube.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/13 15:30:20 by kalipso          ###   ########.fr       */
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

	if (dot1 < EPSILON || dot2 < EPSILON || dot1 > cube->size || dot2 > cube->size)
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
	calcul->v_normal = c->v_norm;

	if (!cube->param.texture && cube->param.color2.r >= 0)
	{
		t_vect	o_to_inter = vect_ab(&cube->O.c0, &c->inter);
		t_vect	diag = vect_ab_norm(&cube->O.c0, &cube->other_p);
		double	dist_color = ft_dot_product(&o_to_inter, &diag) / (SQRT3 * cube->size);
		calcul->px_color = dual_color_render(&cube->param.color, &cube->param.color2, dist_color);
	}

	if (cube->param.texture)
		h_txt_cube(calcul, c, cube);
	if (cube->param.normal_map)
		h_nmap_cube(calcul, c, cube);
	
	if (ft_dot_product(&calcul->v, &c->v_norm) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
}


void	h_txt_cube(t_calcul_px *calcul, t_cube_calc* c, t_cube *plane)
{
	t_img *txt = plane->param.texture;

	int text_x = (int)floor(c->u * txt->sz_x);
	int text_y = (int)floor(c->v * txt->sz_y);

	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	calcul->px_color = (t_rgb){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
}

void	h_nmap_cube(t_calcul_px *calcul, t_cube_calc* c, t_cube *plane)
{
	t_img *nmap = plane->param.normal_map;

	int text_x = (int)floor(c->u * nmap->sz_x);
	int text_y = (int)floor(c->v * nmap->sz_y);

	char *pixel = nmap->addr + (text_y * nmap->ll + text_x * (nmap->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(color & 0xFF) / 255.0 * 2.0 - 1.0};
	ft_normalize_vect(&normal_map);
	// normal_map.dz *= -1; ???
	normal_map.dx *= -1;// ???
	// normal_map.dz *= -1; // Flip depth axis if needed (opengl map)

	calcul->v_normal = (t_vect){
		plane->O.right.dx * normal_map.dx + plane->O.up.dx * normal_map.dy + plane->O.view.dx * normal_map.dz,
		plane->O.right.dy * normal_map.dx + plane->O.up.dy * normal_map.dy + plane->O.view.dy * normal_map.dz,
		plane->O.right.dz * normal_map.dx + plane->O.up.dz * normal_map.dy + plane->O.view.dz * normal_map.dz,
	};
	ft_normalize_vect(&calcul->v_normal);
}