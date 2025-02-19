/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E1_light_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/19 15:56:14 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_coor	ft_ambient(t_data *data, t_calcul_px *c);
int		ft_diffuse(t_data *data, t_calcul_px *c, t_light *lights);
void	ft_reflected(t_data *data, t_calcul_px *c);
void	ft_refracted(t_data *data, t_calcul_px *c);
void	ft_specular(t_data *data, t_calcul_px *c, t_light *lights);

///////////////////////////////////////////////////////////////////////////////]
t_coor	ft_ambient(t_data *data, t_calcul_px *c)
{
	t_coor	color_ambient;

// HANDLE HERE PARTIAL OCCLUSION
	color_ambient.x = c->argb.r * data->bg_light[0]->color.r / 255.0 * data->bg_light[0]->ratio;
	color_ambient.y = c->argb.g * data->bg_light[0]->color.g / 255.0 * data->bg_light[0]->ratio;
	color_ambient.z = c->argb.b * data->bg_light[0]->color.b / 255.0 * data->bg_light[0]->ratio;

	return (color_ambient);
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_reflected(t_data *data, t_calcul_px *c)
{
	double	mirror_scalar;
	t_argb	reflected;

	mirror_scalar = ((t_obj2*)c->object)->param.mirror;

	if (mirror_scalar > EPSILON)
	{
		reflected = what_is_reflected(data, c);
		c->reflected.x = reflected.r * mirror_scalar;
		c->reflected.y = reflected.g * mirror_scalar;
		c->reflected.z = reflected.b * mirror_scalar;
	}
	else
	{
		c->reflected.x = c->argb.r * mirror_scalar;
		c->reflected.y = c->argb.g * mirror_scalar;
		c->reflected.z = c->argb.b * mirror_scalar;
	}
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_refracted(t_data *data, t_calcul_px *c)
{

	t_argb	behind;

	c->eff_trans = fmin(1.0, c->argb.a / 255.0 + ((t_obj2*)c->object)->param.transparence);
	if (c->eff_trans > EPSILON)
	{
		behind = what_is_behind(data, c);
		c->behind.x = behind.r * c->eff_trans;
		c->behind.y = behind.g * c->eff_trans;
		c->behind.z = behind.b * c->eff_trans;
	}
	else
	{
		c->behind.x = c->argb.r * c->eff_trans;
		c->behind.y = c->argb.g * c->eff_trans;
		c->behind.z = c->argb.b * c->eff_trans;
	}
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
void	ft_specular(t_data *data, t_calcul_px *c, t_light *lights)
{
	t_vect reflected_light;
	double	cos_angle;
	double	adjusted_intensity;

	reflected_light = ft_vect_reflected(&c->v_light, &c->v_normal);
	cos_angle = fmin(1.0, ft_dot_product(&c->v, &reflected_light));
	if (cos_angle < 0.0)
		return ;
	adjusted_intensity = ((t_obj2*)c->object)->param.specular * c->transp_light.ratio * pow(cos_angle, ((t_obj2*)c->object)->param.shiny);

	c->diffuse.x += c->transp_light.color.r * adjusted_intensity;
	c->diffuse.y += c->transp_light.color.g * adjusted_intensity;
	c->diffuse.z += c->transp_light.color.b * adjusted_intensity;
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_diffuse(t_data *data, t_calcul_px *c, t_light *lights)
{
	double adjusted_intensity;
	
	c->dist_light = dist_two_points(&c->inter, &lights->xyz);
	c->v_light = vect_ab_norm(&c->inter, &lights->xyz);
	c->cos_angle = ft_dot_product(&c->v_light, &c->v_normal);

	int	block = something_block_the_light(data, c, lights);
	if (c->print)
		printf("%d) cos angle light: %.3f\n\tlight blocked? %d\n", c->print, c->cos_angle, block);
	// if (c->cos_angle < EPSILON || (block = something_block_the_light(data, c, lights)))
	if (c->cos_angle < EPSILON || block)
		return (0);
	adjusted_intensity = c->transp_light.ratio * c->cos_angle;
	adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (1 + c->dist_light * c->dist_light);

	c->diffuse.x += c->argb.r * c->transp_light.color.r / 255.0 * adjusted_intensity;
	c->diffuse.y += c->argb.g * c->transp_light.color.g / 255.0 * adjusted_intensity;
	c->diffuse.z += c->argb.b * c->transp_light.color.b / 255.0 * adjusted_intensity;
	return (1);
}