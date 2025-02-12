/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E1_shadows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/11 15:17:51 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
typedef int (*t_in_shadow_of)(t_calcul_px*, void*, int);

static const t_in_shadow_of g_in_shadow_of[] = {
	distance_from_circle,
	distance_from_sphere,
	distance_from_plane,
	distance_from_cylinder,
	distance_from_cone,
	NULL,
	distance_from_arrow,
	NULL
};
///////////////////////////////////////////////////////////////////////////////]

///////////////////////////////////////////////////////////////////////////////]
int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light)
{
	void	**obj_ptr;
	t_calcul_px	calcul;
	double	dist_light;

	calcul.c0 = c->inter;
	calcul.v = c->v_light;
	calcul.dist = c->dist_light;

	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		if (g_in_shadow_of[((t_obj2*)*obj_ptr)->type](&calcul, *obj_ptr, 1))
		{
			if (((t_obj2*)*obj_ptr)->param.transparence < EPSILON)
				return (1);
		}
	}
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
// calculate angle between camera ray and light source at intersection
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal)
{
	t_vect	l;
	double	cos_theta;

	l = vect_ab_norm(intersection, light);
	cos_theta = ft_dot_product(&l, normal);

	return (cos_theta);
}

t_coor	ft_ambient(t_data *data, t_calcul_px *c)
{
	t_coor	color_ambient;

// HANDLE HERE PARTIAL OCCLUSION
	color_ambient.x = c->px_color.r * data->bg_light[0]->color.r / 255.0 * data->bg_light[0]->ratio;
	color_ambient.y = c->px_color.g * data->bg_light[0]->color.g / 255.0 * data->bg_light[0]->ratio;
	color_ambient.z = c->px_color.b * data->bg_light[0]->color.b / 255.0 * data->bg_light[0]->ratio;

	return (color_ambient);
}
