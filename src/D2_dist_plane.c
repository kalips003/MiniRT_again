/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D2_dist_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/13 01:20:40 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_plane(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c, int simple);
void	h_txt_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane *plane);
void	h_nmap_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane* plane);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_plane(t_calcul_px *calcul, void *obj, int simple)
{
	t_plane_calc	c;
	t_plane	*plane;

	plane = (t_plane*)obj;

	c.top = -(plane->O.view.dx * calcul->c0.x + plane->O.view.dy * calcul->c0.y + plane->O.view.dz * calcul->c0.z + plane->d);
	c.bot = plane->O.view.dx * calcul->v.dx + plane->O.view.dy * calcul->v.dy + plane->O.view.dz * calcul->v.dz;
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0.0)
		return (0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_plane(calcul, plane, &c, simple));
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
int	h_dist_plane(t_calcul_px *calcul, t_plane *plane, t_plane_calc* c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = plane;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->px_color = plane->param.color;
	calcul->v_normal = plane->O.view;

	if (plane->param.color2.r >= 0 || plane->param.texture || plane->param.normal_map)
	{
		c->o_to_inter = vect_ab(&calcul->inter, &plane->O.c0);
		c->u = ft_dot_product(&c->o_to_inter, &plane->O.right) * (plane->param.gamma);
		c->v = ft_dot_product(&c->o_to_inter, &plane->O.up) * (plane->param.gamma);
	}
	if (!plane->param.texture && plane->param.color2.r >= 0)
		if (((int)floor(c->u) + (int)floor(c->v)) % 2)
			calcul->px_color = plane->param.color2;

	if (plane->param.texture)
		h_txt_plane(calcul, c, plane);
	if (plane->param.normal_map)
		h_nmap_plane(calcul, c, plane);
		
	if (c->bot > 0.0)
	// if (ft_dot_product(&calcul->v, &plane->O.view) > 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	return (1);
}


void	h_txt_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane *plane)
{
	t_img *txt = plane->param.texture;

	int text_x = ((int)floor(c->u) % txt->sz_x + txt->sz_x) % txt->sz_x;
	int text_y = ((int)floor(c->v) % txt->sz_y + txt->sz_y) % txt->sz_y;

	char *pixel = txt->addr + (text_y * txt->ll + text_x * (txt->bpp / 8));
	int color = *(unsigned int *)pixel;

	calcul->px_color = (t_rgb){
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
}

void	h_nmap_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane *plane)
{
	t_img *nmap = plane->param.normal_map;

	int text_x = ((int)floor(c->u) % nmap->sz_x + nmap->sz_x) % nmap->sz_x;
	int text_y = ((int)floor(c->v) % nmap->sz_y + nmap->sz_y) % nmap->sz_y;

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