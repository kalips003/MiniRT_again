/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E0_lighting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 23:36:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
void	ft_lighting_simple(t_data *data, t_calcul_px *c);
void	ft_lighting_sublim(t_data *data, t_calcul_px *c);

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


///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_simple(t_data *data, t_calcul_px *c)
{
	t_coor ambient;
	t_coor	diffuse;
	t_light	**lights;
	double adjusted_intensity;
	
	ambient = ft_ambient(data, c);
	memset(&diffuse, 0 , sizeof(t_coor));

	lights = data->light_source - 1;
	while (++lights && *lights)
	{
		c->dist_light = dist_two_points(&c->inter, &(*lights)->xyz);
		c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;
	}

	if (c->print == 1)
	{
		printf("cos_angle = %f\n", c->cos_angle);
		printf("adjusted_intensity = %f\n", adjusted_intensity);
	}
	// t_rgb	reflected = what_is_reflected(data, c);
	// double mirror_scalar = ((t_obj2*)c->object)->param.mirror;

	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

	// c->px_color.r = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.x + diffuse.x) + mirror_scalar * reflected.r)));
	// c->px_color.g = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.y + diffuse.y) + mirror_scalar * reflected.g)));
	// c->px_color.b = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.z + diffuse.z) + mirror_scalar * reflected.b)));
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_sublim(t_data *data, t_calcul_px *c)
{
	t_coor ambient;

	ambient = ft_ambient(data, c);
	// diffuse = ft_diffuse(data, c);


	t_coor	diffuse;
	t_coor	reflected;
	t_light	**lights;
	double cos_angle;
	double adjusted_intensity;
	
	lights = data->light_source - 1;
	memset(&diffuse, 0 , sizeof(t_coor));

	while (++lights && *lights)
	{
		c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		// adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light);
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;

		t_vect reflected_view = ft_vect_reflected(&c->v, &c->v_normal);
		double	cos_angle2 = fmax(-1.0, fmin(1.0, ft_dot_product(&c->v_light, &reflected_view)));
		if (cos_angle2 < 0.0)
			continue ;
		adjusted_intensity = SCALAR_SPECULAR * (*lights)->ratio * pow(cos_angle2, SCALAR_SHINY);
		// adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += (*lights)->color.r * adjusted_intensity;
		diffuse.y += (*lights)->color.g * adjusted_intensity;
		diffuse.z += (*lights)->color.b * adjusted_intensity;
	}
	
	// t_light	**lights;
	// lights = data->light_source - 1;
	// while (lights && *(++lights))
	// {
	// 	c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
	// 	if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
	// 		continue ;
	// 	ft_diffuse_and_reflected(data, c, *lights);
	// }


	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

}





