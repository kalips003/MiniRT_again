/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E0_lighting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/18 10:10:06 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	ft_lighting_simple(t_data *data, t_calcul_px *c);
void	ft_lighting_sublim(t_data *data, t_calcul_px *c);


///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_simple(t_data *data, t_calcul_px *c)
{
	t_light	**lights;
	t_coor	final;
	double	obj_scalar;

	c->diffuse = ft_ambient(data, c);

	lights = data->light_source - 1;
	while (++lights && *lights)
	{
		if (!ft_diffuse(data, c, *lights))
			continue ;
		ft_specular(data, c, *lights);
	}

	ft_reflected(data, c);
	ft_refracted(data, c);

	obj_scalar = fmax(0.0, 1.0 - ((t_obj2*)c->object)->param.mirror - c->eff_trans);
	final = (t_coor){
		obj_scalar * c->diffuse.x + c->reflected.x + c->behind.x,
		obj_scalar * c->diffuse.y + c->reflected.y + c->behind.y,
		obj_scalar * c->diffuse.z + c->reflected.z + c->behind.z
	};
	if (c->print == 1)
	{
		printf("obj_scalar: %.2f; c->eff_trans: %.2f\n", obj_scalar, c->eff_trans);
		printf("c->diffuse: [%.2f,%.2f,%.2f]\n", c->diffuse.x, c->diffuse.y, c->diffuse.z);
		printf("c->reflected: [%.2f,%.2f,%.2f]\n", c->reflected.x, c->reflected.y, c->reflected.z);
		printf("c->behind: [%.2f,%.2f,%.2f]\n", c->behind.x, c->behind.y, c->behind.z);
	}
	c->argb.r = fmax(0, fmin(255, round(final.x)));
	c->argb.g = fmax(0, fmin(255, round(final.y)));
	c->argb.b = fmax(0, fmin(255, round(final.z)));
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_sublim(t_data *data, t_calcul_px *c)
{
	t_light	**lights;
	t_coor	final;
	double	obj_scalar;

	c->diffuse = ft_ambient(data, c);

	lights = data->light_source - 1;
	while (++lights && *lights)
	{
		if (!ft_diffuse_sublim(data, c, *lights))
			continue ;
		ft_specular(data, c, *lights);
	}

	ft_reflected(data, c);
	ft_refracted_sublim(data, c, 1);

	obj_scalar = fmax(0.0, 1.0 - ((t_obj2*)c->object)->param.mirror - c->eff_trans);
	final = (t_coor){
		obj_scalar * c->diffuse.x + c->reflected.x + c->behind.x,
		obj_scalar * c->diffuse.y + c->reflected.y + c->behind.y,
		obj_scalar * c->diffuse.z + c->reflected.z + c->behind.z
	};
	if (c->print == 1)
	{
		printf("obj_scalar: %.2f; c->eff_trans: %.2f\n", obj_scalar, c->eff_trans);
		printf("c->diffuse: [%.2f,%.2f,%.2f]\n", c->diffuse.x, c->diffuse.y, c->diffuse.z);
		printf("c->reflected: [%.2f,%.2f,%.2f]\n", c->reflected.x, c->reflected.y, c->reflected.z);
		printf("c->behind: [%.2f,%.2f,%.2f]\n", c->behind.x, c->behind.y, c->behind.z);
	}
	c->argb.r = fmax(0, fmin(255, round(final.x)));
	c->argb.g = fmax(0, fmin(255, round(final.y)));
	c->argb.b = fmax(0, fmin(255, round(final.z)));
}


