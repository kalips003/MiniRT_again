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

t_vect	combined_quaternion_rotation(t_obj *obj, double angle_α, double angle_β);
t_vect	quaternion_rotation(t_vect *v, t_vect *axis_rota, double angle_α, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_α, int posi_neg);

///////////////////////////////////////////////////////////////////////////////]
// return the rotated vector view object, around
// 		angle_α ~ UP then around angle_β ~ RIGHT
// Q rotation combined = qβ.qα
t_vect	combined_quaternion_rotation(t_obj *obj, double angle_α, double angle_β)
{
	t_camera_calc	c;
	t_vect			rotated;

	c.cosA = cos(angle_α / 2);
	c.sinA = sin(-angle_α / 2);
	c.cosB = cos(angle_β / 2);
	c.sinB = sin(-angle_β / 2);

	c.Qw = c.cosA*c.cosB - c.sinA*c.sinB * (obj->right.dx * obj->up.dx + obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
	c.Qi = c.cosB*c.sinA * obj->up.dx + c.cosA*c.sinB * obj->right.dx + c.sinA*c.sinB*(obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
	c.Qj = c.cosB*c.sinA * obj->up.dy + c.cosA*c.sinB * obj->right.dy + c.sinA*c.sinB*(obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
	c.Qk = c.cosB*c.sinA * obj->up.dz + c.cosA*c.sinB * obj->right.dz + c.sinA*c.sinB*(obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);

	rotated.dx = obj->view.dx * (c.Qw * c.Qw + c.Qi * c.Qi - c.Qj * c.Qj - c.Qk * c.Qk) + 2*obj->view.dy * (c.Qi*c.Qj - c.Qw*c.Qk) + 2*obj->view.dz * (c.Qi*c.Qk + c.Qw*c.Qj);
	rotated.dy = obj->view.dy * (c.Qw * c.Qw + c.Qj * c.Qj - c.Qi * c.Qi - c.Qk * c.Qk) + 2*obj->view.dz * (c.Qj*c.Qk - c.Qw*c.Qi) + 2*obj->view.dx * (c.Qi*c.Qj + c.Qw*c.Qk);
	rotated.dz = obj->view.dz * (c.Qw * c.Qw + c.Qk * c.Qk - c.Qi * c.Qi - c.Qj * c.Qj) + 2*obj->view.dx * (c.Qi*c.Qk - c.Qw*c.Qj) + 2*obj->view.dy * (c.Qj*c.Qk + c.Qw*c.Qi);

	ft_normalize_vect(&rotated);
	return (rotated);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the vector of fixed angle around axis
t_vect	quaternion_rotation(t_vect *v, t_vect *axis_rota, double angle_α, int posi_neg)
{
	t_vect	rtrn;
	
	double	cosα = cos(angle_α / 2);
	double	sinα = posi_neg * sin(angle_α / 2);

	double	Qw = cosα;
	double	Qi = sinα * axis_rota->dx;
	double	Qj = sinα * axis_rota->dy;
	double	Qk = sinα * axis_rota->dz;

	double	W = -(Qi * v->dx + Qj * v->dy + Qk * v->dz);
	double	Qx = Qw * v->dx + Qj * v->dz - Qk * v->dy;
	double	Qy = Qw * v->dy + Qk * v->dx - Qi * v->dz;
	double	Qz = Qw * v->dz + Qi * v->dy - Qj * v->dx;

	rtrn.dx = -Qi * W + Qx * Qw - Qy * Qk + Qz * Qj;
	rtrn.dy = -Qj * W + Qy * Qw - Qz * Qi + Qx * Qk;
	rtrn.dz = -Qk * W + Qz * Qw - Qx * Qj + Qy * Qi;

	// ft_normalize_vect(&rtrn);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate object of angle around axis
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_α, int posi_neg)
{
	t_vect new_view = quaternion_rotation(&obj->view, axis_rota, angle_α, posi_neg);
	t_vect new_up = quaternion_rotation(&obj->up, axis_rota, angle_α, posi_neg);
	// t_vect new_right = quaternion_rotation(&obj->right, axis_rota, angle_α, posi_neg);
	t_vect new_right = ft_cross_product_norm(&new_view, &new_up);

	obj->view = new_view;
	obj->up = new_up;
	obj->right = new_right;
}