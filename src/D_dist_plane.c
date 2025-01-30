/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/29 15:03:32 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_plane_v2(t_calcul_px *calcul, t_plane *p);
void	h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c);
t_rgb	ft_textures_plane(t_calcul_px *calcul, t_plane* plane);
t_vect	ft_nmap_plane(t_calcul_px *calcul, t_plane* plane);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane_v2(t_calcul_px *calcul, t_plane *p)
{
	t_plane_calc	c;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(p->O.view.dx * calcul->c0.x + p->O.view.dy * calcul->c0.y + p->O.view.dz * calcul->c0.z + p->d);
	c.bot = p->O.view.dx * calcul->v_view.dx + p->O.view.dy * calcul->v_view.dy + p->O.view.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (-1.0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_plane(calcul, p, &c);
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c)
{
	calcul->dist = c->dist;
	calcul->object = (void *)plane;

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * c->dist,
		calcul->c0.y + calcul->v_view.dy * c->dist,
		calcul->c0.z + calcul->v_view.dz * c->dist};

	calcul->v_normal = plane->O.view;

	if (plane->texture)
		calcul->px_color = ft_textures_plane(calcul, plane);
	else	
		calcul->px_color = plane->color;
	
	if (plane->normal_map)
		calcul->v_normal = ft_nmap_plane(calcul, plane);
	
	if (ft_vect_dot_product(&calcul->v_view, &plane->O.view) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
}

t_rgb	ft_textures_plane(t_calcul_px *calcul, t_plane* plane)
{
	t_img *texture = ((t_plane*)calcul->object)->texture;
	t_vect	o_to_inter = (t_vect){
		calcul->inter.x - plane->O.c0.x,
		calcul->inter.y - plane->O.c0.y,
		calcul->inter.z - plane->O.c0.z
	};
	double	u = ft_vect_dot_product(&o_to_inter, &plane->O.right) * (1 + plane->gamma);
	double	v = ft_vect_dot_product(&o_to_inter, &plane->O.up) * (1 + plane->gamma);

	int text_x = ((int)floor(u) % texture->sz_x + texture->sz_x) % texture->sz_x;
	int text_y = ((int)floor(v) % texture->sz_y + texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_rgb	rtrn = {
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};

	return (rtrn);
}

t_vect	ft_nmap_plane(t_calcul_px *calcul, t_plane* plane)
{
	t_img *texture = ((t_plane*)calcul->object)->normal_map;
	t_vect	o_to_inter = (t_vect){
		calcul->inter.x - plane->O.c0.x,
		calcul->inter.y - plane->O.c0.y,
		calcul->inter.z - plane->O.c0.z
	};
	double	u = ft_vect_dot_product(&o_to_inter, &plane->O.right);
	double	v = ft_vect_dot_product(&o_to_inter, &plane->O.up);

	int text_x = ((int)floor(u) % texture->sz_x + texture->sz_x) % texture->sz_x;
	int text_y = ((int)floor(v) % texture->sz_y + texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		// (color & 0xFF) / 255.0 * 2.0 - 1.0};
		(color & 0xFF) / 255.0};
	ft_normalize_vect(&normal_map);
	// normal_map.dz *= -1; ???
	// normal_map.dx *= -1; ???
	// normal_map.dz *= -1; // Flip depth axis if needed (opengl map)

	t_vect world_normal = {
		ft_vect_dot_product(&normal_map, &plane->O.up),
		ft_vect_dot_product(&normal_map, &plane->O.view),
		ft_vect_dot_product(&normal_map, &plane->O.right)
	};
	ft_normalize_vect(&world_normal);
	
	return (world_normal);
}