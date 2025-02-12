/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E0_lighting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/12 01:15:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	ft_lighting_simple(t_data *data, t_calcul_px *c);
void	ft_lighting_sublim(t_data *data, t_calcul_px *c);


///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_simple(t_data *data, t_calcul_px *c)
{
	t_coor	ambient;
	t_coor	diffuse;
	t_light	**lights;
	double adjusted_intensity;
	
	ambient = ft_ambient(data, c);
	memset(&diffuse, 0 , sizeof(t_coor));

	lights = data->light_source - 1;
	while (++lights && *lights)
	{
		c->dist_light = dist_two_points(&c->inter, &(*lights)->xyz);
		c->v_light = vect_ab_norm(&c->inter, &(*lights)->xyz);
		c->cos_angle = ft_dot_product(&c->v_light, &c->v_normal);
		if (c->cos_angle < EPSILON || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);

		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;


	}


	t_rgb	reflected = c->px_color;
	double mirror_scalar = ((t_obj2*)c->object)->param.mirror;
	if (mirror_scalar > EPSILON)
		reflected = what_is_reflected(data, c);

	t_rgb	behind = c->px_color;
	double transparence_scalar = ((t_obj2*)c->object)->param.transparence;
	if (transparence_scalar > EPSILON)
		behind = what_is_behind(data, c);

	t_coor	final = (t_coor){
		(1 - mirror_scalar - transparence_scalar) * (ambient.x + diffuse.x) + mirror_scalar * reflected.r + transparence_scalar * behind.r,
		(1 - mirror_scalar - transparence_scalar) * (ambient.y + diffuse.y) + mirror_scalar * reflected.g + transparence_scalar * behind.g,
		(1 - mirror_scalar - transparence_scalar) * (ambient.z + diffuse.z) + mirror_scalar * reflected.b + transparence_scalar * behind.b
	};
	c->px_color.r = fmax(0, fmin(255, round(final.x)));
	c->px_color.g = fmax(0, fmin(255, round(final.y)));
	c->px_color.b = fmax(0, fmin(255, round(final.z)));
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_sublim(t_data *data, t_calcul_px *c)
{
	t_coor ambient;

	ambient = ft_ambient(data, c);
	// diffuse = ft_diffuse(data, c);


	t_coor	diffuse;
	t_light	**lights;
	double cos_angle;
	double adjusted_intensity;
	
	lights = data->light_source - 1;
	memset(&diffuse, 0 , sizeof(t_coor));

	while (++lights && *lights)
	{
		c->dist_light = dist_two_points(&c->inter, &(*lights)->xyz);
		c->v_light = vect_ab_norm(&c->inter, &(*lights)->xyz);
		c->cos_angle = ft_dot_product(&c->v_light, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;



		t_vect reflected_view = ft_vect_reflected(&c->v, &c->v_normal);
		double	cos_angle2 = fmax(-1.0, fmin(1.0, ft_dot_product(&c->v_light, &reflected_view)));
		if (cos_angle2 < 0.0)
			continue ;
		adjusted_intensity = SCALAR_SPECULAR * (*lights)->ratio * pow(cos_angle2, ((t_obj2*)c->object)->param.shiny);
		// adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += (*lights)->color.b / 255.0 * adjusted_intensity;
	}
	

	t_rgb	reflected = c->px_color;
	double mirror_scalar = ((t_obj2*)c->object)->param.mirror;
	if (mirror_scalar > EPSILON)
		reflected = what_is_reflected(data, c);

	t_rgb	behind = c->px_color;
	double transparence_scalar = ((t_obj2*)c->object)->param.transparence;
	if (transparence_scalar > EPSILON)
		behind = what_is_behind(data, c);

	t_coor	final = (t_coor){
		(1 - mirror_scalar - transparence_scalar) * (ambient.x + diffuse.x) + mirror_scalar * reflected.r + transparence_scalar * behind.r,
		(1 - mirror_scalar - transparence_scalar) * (ambient.y + diffuse.y) + mirror_scalar * reflected.g + transparence_scalar * behind.g,
		(1 - mirror_scalar - transparence_scalar) * (ambient.z + diffuse.z) + mirror_scalar * reflected.b + transparence_scalar * behind.b
	};
	c->px_color.r = fmax(0, fmin(255, round(final.x)));
	c->px_color.g = fmax(0, fmin(255, round(final.y)));
	c->px_color.b = fmax(0, fmin(255, round(final.z)));

}





