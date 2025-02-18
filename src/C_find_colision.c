/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_find_colision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/16 07:05:11 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);
int	ft_find_pixel_colision_sublim(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
typedef int (*t_dist_of)(t_calcul_px*, void*, int);

static const t_dist_of g_ft_dist_of[] = {
	distance_from_circle,
	distance_from_sphere,
	distance_from_plane,
	distance_from_cylinder,
	distance_from_cone,
	NULL,
	distance_from_arrow,
	distance_from_cube,
	distance_from_dblplane,
	distance_from_sprite,
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
	int		rtrn;

	c->dist = -1.0;
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{

		rtrn = g_ft_dist_of[((t_obj2*)*obj_ptr)->type](c, *obj_ptr, 0);
	
	}
	// if (transparence && max_step limint not reached) ... ;

	return (c->dist != -1.0);
}

int	ft_find_pixel_colision_sublim(t_data *data, t_calcul_px *c)
{
	void	**obj_ptr;
	int		rtrn;

	rtrn = 0;
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
		rtrn |= g_ft_dist_of[((t_obj2*)*obj_ptr)->type](c, *obj_ptr, 0);

	return (rtrn);
}
