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

int		ft_normalize_vect(t_vect *vect);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);
double	h_smalest_Δ(double a, double b);

///////////////////////////////////////////////////////////////////////////////]
// Normalize vector
int	ft_normalize_vect(t_vect *vect)
{
	double normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
	if (normal_magnitude == 0.0)
		return (put("VECTOR = [%f, %f, %f]\n", vect->dx, vect->dy, vect->dz), 1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// fills rtrnn with the ccoordonates of the rotated vector camera for [x,y]pixel
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
{
	t_camera *c = data->eye.c;
	t_camera_calc	calc;

	calc.cosA = cos(angle_α / 2);
	calc.sinA = sin(angle_α / 2);
	calc.cosB = cos(-angle_β / 2);
	calc.sinB = sin(-angle_β / 2);

	calc.Qw = calc.cosA*calc.cosB - calc.sinA*calc.sinB * (c->O.right.dx * c->O.up.dx + c->O.right.dy * c->O.up.dy + c->O.right.dz * c->O.up.dz);
	calc.Qi = calc.cosB*calc.sinA * c->O.up.dx + calc.cosA*calc.sinB * c->O.right.dx + calc.sinA*calc.sinB*(c->O.right.dy * c->O.up.dz - c->O.right.dz * c->O.up.dy);
	calc.Qj = calc.cosB*calc.sinA * c->O.up.dy + calc.cosA*calc.sinB * c->O.right.dy + calc.sinA*calc.sinB*(c->O.right.dz * c->O.up.dx - c->O.right.dx * c->O.up.dz);
	calc.Qk = calc.cosB*calc.sinA * c->O.up.dz + calc.cosA*calc.sinB * c->O.right.dz + calc.sinA*calc.sinB*(c->O.right.dx * c->O.up.dy - c->O.right.dy * c->O.up.dx);

	rtrn->dx = c->O.view.dx * (calc.Qw * calc.Qw + calc.Qi * calc.Qi - calc.Qj * calc.Qj - calc.Qk * calc.Qk) + 2*c->O.view.dy * (calc.Qi*calc.Qj - calc.Qw*calc.Qk) + 2*c->O.view.dz * (calc.Qi*calc.Qk + calc.Qw*calc.Qj);
	rtrn->dy = c->O.view.dy * (calc.Qw * calc.Qw + calc.Qj * calc.Qj - calc.Qi * calc.Qi - calc.Qk * calc.Qk) + 2*c->O.view.dz * (calc.Qj*calc.Qk - calc.Qw*calc.Qi) + 2*c->O.view.dx * (calc.Qi*calc.Qj + calc.Qw*calc.Qk);
	rtrn->dz = c->O.view.dz * (calc.Qw * calc.Qw + calc.Qk * calc.Qk - calc.Qi * calc.Qi - calc.Qj * calc.Qj) + 2*c->O.view.dx * (calc.Qi*calc.Qk - calc.Qw*calc.Qj) + 2*c->O.view.dy * (calc.Qj*calc.Qk + calc.Qw*calc.Qi);

	ft_normalize_vect(rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// 	return smalest positive
double h_smalest_Δ(double a, double b)
{
	if (a < EPSILON && b < EPSILON)
		return (-1.0);
	if (a < EPSILON)
		return (b);
	if (b < EPSILON)
		return (a);
	return (a * (a < b) + b * (b < a));
}
