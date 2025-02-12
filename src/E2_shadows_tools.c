/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2_shadows_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/12 00:57:09 by kalipso          ###   ########.fr       */
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

	if (calcul->reflected_depth == REFLECTION_BOUNCES)
		return (calcul->px_color);
	c.reflected_depth = calcul->reflected_depth + 1;
	c.c0 = calcul->inter;
	c.v = ft_vect_reflected(&calcul->v, &calcul->v_normal);
	c.dist = -1.0;
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);
}

t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
{
	t_vect refracted;
	t_vect	Ipara;
	t_vect	Iortho;
	double	dot = ft_dot_product(incident, normal);

// Incident_∥​
	Ipara = (t_vect){
		dot * normal->dx,
		dot * normal->dy,
		dot * normal->dz
	};
// Incident_⊥
	Iortho = (t_vect){
		incident->dx - Ipara.dx,
		incident->dy - Ipara.dy,
		incident->dz - Ipara.dz
	};
	double sinθ1 = sqrt(Iortho.dx * Iortho.dx + Iortho.dy *Iortho.dy + Iortho.dz * Iortho.dz);
	double sinθ2 = gamma_incident / gamma_obj * sinθ1;
	if (sinθ2 > 1.0)
		printf("error reflection");
	t_vect	Rortho = (t_vect){
		gamma_incident / gamma_obj * Iortho.dx,
		gamma_incident / gamma_obj * Iortho.dy,
		gamma_incident / gamma_obj * Iortho.dz
	};
	double s = -sqrt(1 - sinθ2 * sinθ2);
	t_vect	Rpara = (t_vect){
		s * normal->dx,
		s * normal->dy,
		s * normal->dz
	};
	refracted = (t_vect){
		Rortho.dx + Rpara.dx,
		Rortho.dy + Rpara.dy,
		Rortho.dz + Rpara.dz
	};

	return (refracted);
}


t_vect	ft_vect_refracted_v2(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
{
	t_vect refracted;

	double	dot = ft_dot_product(incident, normal);
	double η = gamma_incident / gamma_obj;
	double sinθ2 = η * sqrt(1 - dot * dot);
	if (sinθ2 > 1.0)
		return (ft_vect_reflected(incident, normal));
		// return (printf("error reflection\n"), ft_vect_reflected(incident, normal));
	double s = -sqrt(1 - sinθ2 * sinθ2);
	refracted = (t_vect){
		η * (incident->dx - dot * normal->dx) + s * normal->dx,
		η * (incident->dy - dot * normal->dy) + s * normal->dy,
		η * (incident->dz - dot * normal->dz) + s * normal->dz
	};
	return (refracted);
}

t_vect	ft_vect_refracted_v3(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj)
{
	double cos_i = -ft_dot_product(incident, normal);
	double index = 1.0 / gamma_obj;
	double cos_r = sqrt(1.0 - index * index * (1 - cos_i * cos_i));

	t_vect refracted = (t_vect){
		index * incident->dx + (index * cos_i - cos_r) * normal->dx,
		index * incident->dy + (index * cos_i - cos_r) * normal->dy,
		index * incident->dz + (index * cos_i - cos_r) * normal->dz
	};
}

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
	// calculate_pixel_color(data, &c, 0);

	if (!calculate_pixel_color(data, &c, 0) || ((t_obj2*)calcul->object)->type == PLANE)
		return (c.px_color);
	c.c0 = new_moved_point(&c.inter, &c.v, EPSILON);
	c.v = ft_vect_refracted_v2(&c.v, &c.v_normal, ((t_obj2*)calcul->object)->param.gamma, calcul->current_gamma);
	c.dist = -1.0;
	calculate_pixel_color(data, &c, 0);
	return (c.px_color);

}