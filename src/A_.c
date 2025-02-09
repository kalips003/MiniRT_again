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


// typedef void* (*t_in_shadow_of)(t_calcul_px*, void*);
// t_in_shadow_of g_ft_in_shadow_of[] = {
// 	in_shadow_of_sphere_void,
// 	in_shadow_of_plane_void,
// 	in_shadow_of_cicle_void,
// 	in_shadow_of_cylinder_void,
// 	in_shadow_of_cone_void
// };

void	create_vector_space(t_obj *obj);
void	f_calculate_combined_quaternion_better(t_obj *obj, double angle_α, double angle_β, t_vect *rotated);

typedef double (*t_dist_of)(t_calcul_px *calcul, void *obj);

typedef struct s_ft_obj
{
	char		*name;
	t_dist_of	ft_dist_of;
}	t_ft_obj;

static const t_ft_obj	dico_ft[] = {
	{"Circle", parse_pl},
	{"Plane", parse_pl},
	{"Sphere", parse_sp},
	{"Cylinder", parse_cy},
	{"Cone", parse_co},
	{NULL, NULL}
};




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
			*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}};
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
void	recalculate_cone(t_cone *cone)
{
	cone->apex = new_moved_point(&cone->O.c0, &cone->O.view, cone->height);
	cone->slope = (cone->radius * cone->radius) / (cone->height * cone->height);
}

void	recalculate_cylinder(t_cylinder *cylinder)
{
	cylinder->xyz_other = new_moved_point(&cylinder->O.c0, &cylinder->O.view, cylinder->height);
}

void	recalculate_plane(t_plane *plane)
{
	plane->d = -(plane->O.view.dx * plane->O.c0.x + plane->O.view.dy * plane->O.c0.y + plane->O.view.dz * plane->O.c0.z);
}

///////////////////////////////////////////////////////////////////////////////]
// fills rtrn with the coordonates of the rotated vector view object, around
// 		angle_α ~ UP then around angle_β ~ RIGHT
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion_better(t_obj *obj, double angle_α, double angle_β, t_vect *rotated)
{
	t_camera_calc	calc;

	calc.cosA = cos(angle_α / 2);
	calc.sinA = sin(angle_α / 2);
	calc.cosB = cos(-angle_β / 2);
	calc.sinB = sin(-angle_β / 2);

	calc.Qw = calc.cosA*calc.cosB - calc.sinA*calc.sinB * (obj->right.dx * obj->up.dx + obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
	calc.Qi = calc.cosB*calc.sinA * obj->up.dx + calc.cosA*calc.sinB * obj->right.dx + calc.sinA*calc.sinB*(obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
	calc.Qj = calc.cosB*calc.sinA * obj->up.dy + calc.cosA*calc.sinB * obj->right.dy + calc.sinA*calc.sinB*(obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
	calc.Qk = calc.cosB*calc.sinA * obj->up.dz + calc.cosA*calc.sinB * obj->right.dz + calc.sinA*calc.sinB*(obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);

	rotated->dx = obj->view.dx * (calc.Qw * calc.Qw + calc.Qi * calc.Qi - calc.Qj * calc.Qj - calc.Qk * calc.Qk) + 2*obj->view.dy * (calc.Qi*calc.Qj - calc.Qw*calc.Qk) + 2*obj->view.dz * (calc.Qi*calc.Qk + calc.Qw*calc.Qj);
	rotated->dy = obj->view.dy * (calc.Qw * calc.Qw + calc.Qj * calc.Qj - calc.Qi * calc.Qi - calc.Qk * calc.Qk) + 2*obj->view.dz * (calc.Qj*calc.Qk - calc.Qw*calc.Qi) + 2*obj->view.dx * (calc.Qi*calc.Qj + calc.Qw*calc.Qk);
	rotated->dz = obj->view.dz * (calc.Qw * calc.Qw + calc.Qk * calc.Qk - calc.Qi * calc.Qi - calc.Qj * calc.Qj) + 2*obj->view.dx * (calc.Qi*calc.Qk - calc.Qw*calc.Qj) + 2*obj->view.dy * (calc.Qj*calc.Qk + calc.Qw*calc.Qi);

	ft_normalize_vect(rotated);
	// multi touche fluid rotation !!!!
}