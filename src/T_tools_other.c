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

void	create_vector_space(t_obj *obj);
double	h_smalest_Δ(double a, double b);
void	recalculate_obj_const(t_obj2 *obj);
t_argb	dual_color_render(t_argb *color1, t_rgb *color2, double dist);

///////////////////////////////////////////////////////////////////////////////]
// takes an obj with a view vector filled, create the vector space
// 
// Right = Camera x Y = {-Cz, 0, Cx}
// Up = Right x Camera = {-CxCy, Cx²+Cz², -CyCz}
void	create_vector_space(t_obj *obj)
{
// if camera vector is == Y vector
	if (fabs(obj->view.dx) < EPSILON && fabs(obj->view.dz) < EPSILON)
	{
		if (obj->view.dy > 0)
			*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}};
		else
			*obj = (t_obj){obj->c0, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}};
		return ;
	}
	obj->right.dx = -obj->view.dz;
	obj->right.dy = 0;
	obj->right.dz = obj->view.dx;
	ft_normalize_vect(&obj->right);
	obj->up.dx = -obj->view.dx * obj->view.dy;
	obj->up.dy = obj->view.dx * obj->view.dx + obj->view.dz * obj->view.dz;
	obj->up.dz = -obj->view.dy * obj->view.dz;
	ft_normalize_vect(&obj->up);
}

///////////////////////////////////////////////////////////////////////////////]
// 	return smalest positive
double	h_smalest_Δ(double a, double b)
{
	if (a < EPSILON && b < EPSILON)
		return (-1.0);
	if (a < EPSILON)
		return (b);
	if (b < EPSILON)
		return (a);
	return (a * (a < b) + b * (b < a));
}

///////////////////////////////////////////////////////////////////////////////]
void	recalculate_obj_const(t_obj2 *obj)
{
	if (!obj)
		return ;
	
	if (obj->type == CONE)
	{
		((t_cone*)obj)->apex = new_moved_point(&obj->O.c0, &obj->O.view, ((t_cone*)obj)->height);
		((t_cone*)obj)->slope = pow(((t_cone*)obj)->radius, 2.0) / pow(((t_cone*)obj)->height, 2.0);
	}
	else if (obj->type == CYLINDER)
		((t_cylinder*)obj)->xyz_other = new_moved_point(&obj->O.c0, &obj->O.view, ((t_cylinder*)obj)->height);
	else if (obj->type == PLANE)
		((t_plane*)obj)->d = -(obj->O.view.dx * obj->O.c0.x + obj->O.view.dy * obj->O.c0.y + obj->O.view.dz * obj->O.c0.z);
	else if (obj->type == ARROW)
	{
		((t_arrow*)obj)->xyz_other = new_moved_point(&obj->O.c0, &obj->O.view, ((t_arrow*)obj)->height * 2 / 3);
		((t_arrow*)obj)->apex = new_moved_point(&obj->O.c0, &obj->O.view, ((t_arrow*)obj)->height);
		((t_arrow*)obj)->slope = (9.0 * pow(((t_arrow*)obj)->radius, 2.0)) / pow(((t_arrow*)obj)->height, 2.0);
	}
	else if (obj->type == DOUBLE_PLAN)
	{
		((t_dblplane*)obj)->d = -(obj->O.view.dx * obj->O.c0.x + obj->O.view.dy * obj->O.c0.y + obj->O.view.dz * obj->O.c0.z);
		((t_dblplane*)obj)->other_point = new_moved_point(&obj->O.c0, &obj->O.view, ((t_dblplane*)obj)->width);
		((t_dblplane*)obj)->other_d = -(obj->O.view.dx * ((t_dblplane*)obj)->other_point.x + obj->O.view.dy * ((t_dblplane*)obj)->other_point.y + obj->O.view.dz * ((t_dblplane*)obj)->other_point.z);
	}
	else if (obj->type == SPRITE)
		((t_sprite*)obj)->d = -(obj->O.up.dx * obj->O.c0.x + obj->O.up.dy * obj->O.c0.y + obj->O.up.dz * obj->O.c0.z);
}

///////////////////////////////////////////////////////////////////////////////]
// scale color based on a [0,1] distance
t_argb	dual_color_render(t_argb *color1, t_rgb *color2, double dist)
{
	t_argb	rtrn;

	rtrn.a = color1->a;
	rtrn.r = (int)((color2->r - color1->r) * dist + color1->r);
	rtrn.g = (int)((color2->g - color1->g) * dist + color1->g);
	rtrn.b = (int)((color2->b - color1->b) * dist + color1->b);
	return (rtrn);
}