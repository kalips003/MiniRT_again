/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_find_colision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/21 12:22:34 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
// 	find the pixel color of the closest object
// fills:
// position xyz of the colision (default 0,0,0)
// abc vector normal to the surface
// color of object in view
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;

	c->dist = -1.0;
	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
		distance_from_sphere_v2(c, *sphere_ptr);

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
		distance_from_plane_v2(c, *plane_ptr);


	cyl_ptr = data->cylinders - 1;
	while (++cyl_ptr && *cyl_ptr)
	{
		distance_from_cylinder_v2(c, *cyl_ptr);
		distance_from_cicle_v2(c, (t_circle_v2){
			*cyl_ptr, (*cyl_ptr)->c0, (*cyl_ptr)->radius, (*cyl_ptr)->v});
		distance_from_cicle_v2(c, (t_circle_v2){
			*cyl_ptr, (*cyl_ptr)->xyz_other, (*cyl_ptr)->radius, (*cyl_ptr)->v});
	}
	return (c->dist != -1.0);
}
