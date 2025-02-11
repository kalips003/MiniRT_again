/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y5_param_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 18:43:35 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_reste(t_data *data, char **raw_split, t_param *obj);

#define	PARAM_DICO "STMXNC"

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// (Shininess) S=[1.0, +]
// (Transparence, gamma) T=[0,1],[1.0, 2.5+]
// (Mirroir) M=[0,1]
// (Texture) X=sphere_texture.xpm
// (Normal Map) N=sphere_normal.xpm
static const t_ft_param_parsing g_ft_param_parsing[] = {
	parse_shininess,
	parse_transparence,
	parse_mirror,
	parse_texture,
	parse_nmap,
	parse_color2
};

int	parse_reste(t_data *data, char **raw_split, t_param *obj)
{
	int	index;
	
	obj->gamma = 1.0;
	obj->shiny = 1.0;
	obj->color2.r = -1;
	while (raw_split && *raw_split)
	{
		index = wii(**raw_split, PARAM_DICO);
		if (index < 0)
			return (put(ERR7"(%s) UNKNNOWN PARAMETER\n", *raw_split), 1);
		if (g_ft_param_parsing[index](data, &(*raw_split)[2], obj))
			return (1);
		raw_split++;
	}
	return (0);
}
