/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2_shadows_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/12 15:25:00 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);
double	calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_rgb	what_is_reflected(t_data *data, t_calcul_px *calcul);
t_vect	ft_vect_refracted_v2(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj);
t_rgb	what_is_behind(t_data *data, t_calcul_px *calcul);

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
	double	transp;

	calcul.c0 = c->inter;
	calcul.v = c->v_light;
	calcul.dist = c->dist_light;

	c->transp_light = *light;
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		transp = ((t_obj2*)*obj_ptr)->param.transparence;
		if (g_in_shadow_of[((t_obj2*)*obj_ptr)->type](&calcul, *obj_ptr, 1))
		{
			if (transp < EPSILON)
				return (1);
			transp = 1.0 - transp;
			c->transp_light.ratio *= transp;
			c->transp_light.color.r *= ((t_obj2*)*obj_ptr)->param.color.r / 255.0 * transp;
			c->transp_light.color.g *= ((t_obj2*)*obj_ptr)->param.color.g / 255.0 * transp;
			c->transp_light.color.b *= ((t_obj2*)*obj_ptr)->param.color.b / 255.0 * transp;
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

///////////////////////////////////////////////////////////////////////////////]
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

///////////////////////////////////////////////////////////////////////////////]
t_rgb	what_is_reflected(t_data *data, t_calcul_px *calcul)
{
	t_calcul_px	c;

	if (calcul->reflected_depth == REFLECTION_BOUNCES)
		return (calcul->px_color);
	c.reflected_depth = calcul->reflected_depth + 1;
	c.c0 = calcul->inter;
	c.v = ft_vect_reflected(&calcul->v, &calcul->v_normal);
	c.dist = -1.0;
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_vect_refracted_v2(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
{
	t_vect refracted;

	double	dot = ft_dot_product(incident, normal);
	double η = gamma_incident / gamma_obj;
	double sinθ2 = η * sqrt(1 - dot * dot);
	if (sinθ2 > 1.0)
		return (ft_vect_reflected(incident, normal));
	double s = -sqrt(1 - sinθ2 * sinθ2);
	refracted = (t_vect){
		η * (incident->dx - dot * normal->dx) + s * normal->dx,
		η * (incident->dy - dot * normal->dy) + s * normal->dy,
		η * (incident->dz - dot * normal->dz) + s * normal->dz
	};
	return (refracted);
}

///////////////////////////////////////////////////////////////////////////////]
t_rgb	what_is_behind(t_data *data, t_calcul_px *calcul)
{
	t_calcul_px	c;

	if (calcul->transparence_depth == TRANSPARENCE_BOUNCES)
		return (calcul->px_color);
	c.transparence_depth = calcul->transparence_depth + 1;

	c.c0 = new_moved_point(&calcul->inter, &calcul->v, EPSILON);
	if (((t_obj2*)calcul->object)->type == PLANE)
		c.v = calcul->v;
	else
		c.v = ft_vect_refracted_v2(&calcul->v, &calcul->v_normal, calcul->current_gamma, ((t_obj2*)calcul->object)->param.gamma);
	c.dist = -1.0;
	c.current_gamma = 1.0;
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);
}










