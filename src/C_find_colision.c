/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_find_colision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 16:56:42 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
typedef int (*t_dist_of)(t_calcul_px*, void*, int);

static const t_dist_of g_ft_dist_of[] = {
	distance_from_circle,
	distance_from_sphere,
	distance_from_plane,
	distance_from_cylinder,
	distance_from_cone,
	NULL
};
///////////////////////////////////////////////////////////////////////////////]

///////////////////////////////////////////////////////////////////////////////]
// 	find the closest object colision from vector view
// fills calcul data struct with:
// 		position xyz of the colision (default 0,0,0)
// 		vector normal to the surface ...
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c)
{
	void	**obj_ptr;

	c->dist = -1.0;
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		g_ft_dist_of[((t_obj2*)*obj_ptr)->type](c, *obj_ptr, 0);
	}
	// if (transparence && max_step limint not reached) ... ;

	return (c->dist != -1.0);
}




// int	ft_find_pixel_colision(t_data *data, t_calcul_px *c)
// {
// 	t_sphere	**sphere_ptr;
// 	t_plane		**plane_ptr;
// 	t_cylinder	**cyl_ptr;
// 	t_cone		**cone_ptr;

// 	c->dist = -1.0;
// 	sphere_ptr = data->spheres - 1;
// 	while (++sphere_ptr && *sphere_ptr)
// 		distance_from_sphere_v2(c, *sphere_ptr);

// 	plane_ptr = data->planes - 1;
// 	while (++plane_ptr && *plane_ptr)
// 		distance_from_plane_v2(c, *plane_ptr);


// 	cyl_ptr = data->cylinders - 1;
// 	while (++cyl_ptr && *cyl_ptr)
// 	{
// 		distance_from_cylinder_v2(c, *cyl_ptr);
// 		distance_from_cicle_v2(c, (t_circle_v2){0,
// 			*cyl_ptr, (*cyl_ptr)->O.c0, (*cyl_ptr)->radius, (*cyl_ptr)->O.view});
// 		distance_from_cicle_v2(c, (t_circle_v2){0,
// 			*cyl_ptr, (*cyl_ptr)->xyz_other, (*cyl_ptr)->radius, (*cyl_ptr)->O.view});
// 	}

// 	cone_ptr = data->cones - 1;
// 	while (++cone_ptr && *cone_ptr)
// 	{
// 		distance_from_cone(c, *cone_ptr);
// 		distance_from_cicle_v2(c, (t_circle_v2){0,
// 			*cone_ptr, (*cone_ptr)->O.c0, (*cone_ptr)->radius, (*cone_ptr)->O.view});
// 	}
// 	return (c->dist != -1.0);
// }