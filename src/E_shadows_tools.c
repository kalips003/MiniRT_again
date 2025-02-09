/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_shadows_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 14:49:40 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_rgb	what_is_reflected(t_data *data, t_calcul_px *calcul);
t_rgb	what_is_behind(t_data *data, t_calcul_px *calcul);


t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal)
{
	t_vect reflected;
	double	dot_pro;

	dot_pro = 2.0 * ft_dot_product(incident, normal);
	reflected.dx = incident->dx - dot_pro * normal->dx;
	reflected.dy = incident->dy - dot_pro * normal->dy;
	reflected.dz = incident->dz - dot_pro * normal->dz;
	ft_normalize_vect(&reflected);
	return (reflected);
}

t_rgb	what_is_reflected(t_data *data, t_calcul_px *calcul)
{
	t_calcul_px	c;
	c.c0 = calcul->inter;
	c.v = ft_vect_reflected(&calcul->v, &calcul->v_normal);
	c.dist = -1;
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);
}


t_rgb	what_is_behind(t_data *data, t_calcul_px *calcul)
{
	t_calcul_px	c;
	c.c0 = calcul->inter;

	double cos_i = -ft_dot_product(&calcul->v, &calcul->v_normal);
	double index = 1.0 / ((t_obj2*)calcul->object)->param.gamma;
	double cos_r = sqrt(1.0 - index * index * (1 - cos_i * cos_i));

	c.v = (t_vect){
		index * calcul->v.dx + (index * cos_i - cos_r) * calcul->v_normal.dx,
		index * calcul->v.dy + (index * cos_i - cos_r) * calcul->v_normal.dy,
		index * calcul->v.dz + (index * cos_i - cos_r) * calcul->v_normal.dz
	};
	ft_normalize_vect(&c.v);
	calculate_pixel_color(data, &c, 0);
	// hopefully, calcul->obj == c.obj

	cos_i = -ft_dot_product(&c.v, &c.v_normal);
	index = ((t_obj2*)calcul->object)->param.gamma;
	cos_r = sqrt(1.0 - index * index * (1 - cos_i * cos_i));
	c.v = (t_vect){
		index * calcul->v.dx + (index * cos_i - cos_r) * calcul->v_normal.dx,
		index * calcul->v.dy + (index * cos_i - cos_r) * calcul->v_normal.dy,
		index * calcul->v.dz + (index * cos_i - cos_r) * calcul->v_normal.dz
	};
	ft_normalize_vect(&c.v);
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);
}