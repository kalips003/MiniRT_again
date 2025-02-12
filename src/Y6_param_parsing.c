/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y6_param_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/12 14:09:23 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_shininess(t_data *data, char *raw, t_param *obj);
int	parse_transparence(t_data *data, char *raw, t_param *obj);
int	parse_mirror(t_data *data, char *raw, t_param *obj);
int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
int	parse_color2(t_data *data, char *raw, t_param *obj);

///////////////////////////////////////////////////////////////////////////////]
// (Shininess) S=20.0
int	parse_shininess(t_data *data, char *raw, t_param *obj)
{
	char	**split_tg;

	(void)data;
	split_tg = split(raw, ",");
	if (tab_size(split_tg) != 2)
		return (put(ERR1"bad number of args (SPECULAR - SHINY)\n"), free_tab(split_tg), 1);
	if (ft_atof(split_tg[0], &obj->specular) ||
		ft_atof(split_tg[1], &obj->shiny))
		return (1);
	if (obj->specular < 0.0 || obj->specular > 1.0)
		return (put(ERR1"(%s) specular should be [0.0,1.0]\n", split_tg[0]), free_tab(split_tg), 1);
	if (obj->shiny < 10.0 || obj->shiny > 1000.0)
		return (put(ERR1"(%s) material shininess should be [10,500+]\n", split_tg[0]), free_tab(split_tg), 1);
	free_tab(split_tg);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Transparence, gamma) T=[0,1],[1.0, 2.5+]
int	parse_transparence(t_data *data, char *raw, t_param *obj)
{
	char	**split_tg;

	(void)data;
	split_tg = split(raw, ",");
	if (tab_size(split_tg) != 2)
		return (put(ERR1"bad number of args (TRANSPARENCE - GAMMA)\n"), free_tab(split_tg), 1);
	if (ft_atof(split_tg[0], &obj->transparence) ||
		ft_atof(split_tg[1], &obj->gamma))
		return (1);
	if (obj->transparence < 0.0 || obj->transparence > 1.0)
		return (put(ERR1"(%s) transparence should be [0.0,1.0]\n", split_tg[0]), free_tab(split_tg), 1);
	if (obj->gamma < 0.0)
		return (put(ERR1"(%s) refraction gamma should be [1.0, 2.5+]\n", split_tg[1]), free_tab(split_tg), 1);
	free_tab(split_tg);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Mirroir) M=[0,1]
int	parse_mirror(t_data *data, char *raw, t_param *obj)
{
	(void)data;
	if (ft_atof(raw, &obj->mirror))
		return (1);
	if (obj->mirror < 0.0 || obj->mirror > 1.0)
		return (put(ERR1"(%s) reflection should be [0.0,1.0]\n", raw), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Texture) X=sphere_texture.xpm
int	parse_texture(t_data *data, char *path, t_param *obj)
{
	t_img	*texture;

	texture = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, texture);
	if (!texture || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);

	if (path)
		texture->img = mlx_xpm_file_to_image(data->mlx, path, &texture->sz_x, &texture->sz_y);
	if (!texture->img)
		return(put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->ll, &texture->end);
	obj->texture = texture;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Normal Map) N=sphere_normal.xpm
int	parse_nmap(t_data *data, char *path, t_param *obj)
{
	t_img	*nmap;

	nmap = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, nmap);
	if (!nmap || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);

	if (path)
		nmap->img = mlx_xpm_file_to_image(data->mlx, path, &nmap->sz_x, &nmap->sz_y);
	if (!nmap->img)
		return(put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	nmap->addr = mlx_get_data_addr(nmap->img, &nmap->bpp, &nmap->ll, &nmap->end);
	obj->normal_map = nmap;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Color2) C=255,0,112
int	parse_color2(t_data *data, char *raw, t_param *obj)
{
	(void)data;
	if (ato_rgb(raw, &obj->color2))
		return (1);
	return (0);
}