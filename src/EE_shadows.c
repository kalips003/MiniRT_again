/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EE_shadows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 14:19:03 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);

///////////////////////////////////////////////////////////////////////////////]
typedef int (*t_in_shadow_of)(t_calcul_px*, void*, int);

static const t_in_shadow_of g_in_shadow_of[] = {
	distance_from_circle,
	distance_from_sphere,
	distance_from_plane,
	distance_from_cylinder,
	distance_from_cone,
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
	calcul.v = vect_ab_norm(&calcul.c0, &light->xyz);
	dist_light = dist_two_points(&calcul.c0, &light->xyz);
	calcul.dist = dist_light;

	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		if (g_in_shadow_of[((t_obj2*)*obj_ptr)->type](c, *obj_ptr, 1))
		{
			if (((t_obj2*)*obj_ptr)->param.transparence < EPSILON)
				return (1);
		} 
	}
	

	return (0);
}
