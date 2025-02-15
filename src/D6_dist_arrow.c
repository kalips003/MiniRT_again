/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D6_dist_arrow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/14 20:52:31 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
int		distance_from_arrow(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_cone_1(t_calcul_px *c1, t_cone *cone, t_cone_calc_v2 *c2);
int		h_dist_cone_2(t_calcul_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple);

///////////////////////////////////////////////////////////////////////////////]
int	distance_from_arrow(t_calcul_px *calcul, void *obj, int simple)
{
	t_arrow_calc	c;
	t_arrow	*arrow = (t_arrow*)obj;
	int	rtrn;

	c.cy = (t_cylinder){CYLINDER, arrow->O, arrow->param, arrow->xyz_other, arrow->height * 2 / 3, arrow->radius / 2};
	c.co = (t_cone){CONE, arrow->O, arrow->param, arrow->height / 3, arrow->radius, arrow->apex, arrow->slope};
	c.co.O.c0 = new_moved_point(&c.co.O.c0, &arrow->O.view, arrow->height * 2 / 3);
	rtrn = distance_from_cylinder(calcul, &c.cy, simple);
	rtrn |= distance_from_cone(calcul, &c.co, simple);
	
	if (rtrn && simple)
		return (1);
	if (rtrn)
		calcul->object = obj;
	return (rtrn);
}

