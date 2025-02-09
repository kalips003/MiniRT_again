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

void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
void	h_vector_space(t_obj *obj);
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, int posi_neg);

///////////////////////////////////////////////////////////////////////////////]
// takes an obj with a view vector filled, create the vector space
void	h_vector_space(t_obj *obj)
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

// Right = Camera x Y = {-Cz, 0, Cx}
	obj->right.dx = -obj->view.dz;
	obj->right.dy = 0;
	obj->right.dz = obj->view.dx;
	ft_normalize_vect(&obj->right);

// Up = Right x Camera = {-CxCy, Cx²+Cz², -CyCz}
	obj->up.dx = -obj->view.dx * obj->view.dy;
	obj->up.dy = obj->view.dx * obj->view.dx + obj->view.dz * obj->view.dz;
	obj->up.dz = -obj->view.dy * obj->view.dz;
	ft_normalize_vect(&obj->up);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the camera (eye) vector of fixed angle
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg)
{
	t_vect *c = &data->eye.c->O.view;
	
	double Vx2 = axis_rota->dx * axis_rota->dx;
	double Vy2 = axis_rota->dy * axis_rota->dy;
	double Vz2 = axis_rota->dz * axis_rota->dz;


	double resultx = COS_ROTA2 * c->dx + SIN_ROTA2 * (c->dx * (Vx2 - Vy2 - Vz2) + 2 * axis_rota->dx * (axis_rota->dy * c->dy + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dy * c->dz - axis_rota->dz * c->dy);
	double resulty = COS_ROTA2 * c->dy + SIN_ROTA2 * (c->dy * (Vy2 - Vx2 - Vz2) + 2 * axis_rota->dy * (axis_rota->dx * c->dx + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dz * c->dx - axis_rota->dx * c->dz);
	double resultz = COS_ROTA2 * c->dz + SIN_ROTA2 * (c->dz * (Vz2 - Vx2 - Vy2) + 2 * axis_rota->dz * (axis_rota->dx * c->dx + axis_rota->dy * c->dy)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dx * c->dy - axis_rota->dy * c->dx);
	c->dx = resultx;
	c->dy = resulty;
	c->dz = resultz;

	ft_normalize_vect(c);
	h_vector_space(&data->eye.c->O);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the obj (eye) vector of fixed angle
void	rotation_obj(t_obj *obj, t_vect *axis_rota, int posi_neg)
{
	printf("\tOBJECT: view= %f,%f,%f\nup= %f,%f,%f\nright= %f,%f,%f\n",
	obj->view.dx, obj->view.dy, obj->view.dz,\
	obj->up.dx, obj->up.dy, obj->up.dz, \
	obj->right.dx, obj->right.dy, obj->right.dz);
	
	t_vect	cross = ft_cross_product(axis_rota, &obj->view);
	double	dot = ft_dot_product(axis_rota, &obj->view);
	ft_normalize_vect(&cross);
	t_vect	new_view = {
		obj->view.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
		obj->view.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
		obj->view.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
	};
	ft_normalize_vect(&new_view);
	
	cross = ft_cross_product(axis_rota, &obj->up);
	dot = ft_dot_product(axis_rota, &obj->up);
	ft_normalize_vect(&cross);

	t_vect	new_up = {
		obj->up.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
		obj->up.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
		obj->up.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
	};
	ft_normalize_vect(&new_up);
	
	cross = ft_cross_product(&new_up, &new_view);
	// cross = ft_vect_cross_product(&new_view, &new_up);
	ft_normalize_vect(&cross);
	// cross = (t_vect){-cross.dx, -cross.dy, -cross.dz};
	*obj = (t_obj){obj->c0, new_view, new_up, cross};
}