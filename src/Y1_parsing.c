/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y1_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 17:17:39 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_parse_line(t_data *data, char *line);
int	h_parse_vect_space(t_obj *obj, t_vect *view);
int	parse_A(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);

static const t_dico_pair	dico[] = {
	{"A", parse_A},
	{"L", parse_L},
	{"C", parse_C},
	{"ci", parse_ci},
	{"pl", parse_pl},
	{"sp", parse_sp},
	{"cy", parse_cy},
	{"co", parse_co},
	{"ar", parse_ar},
	{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////////////]
// input line = "# C 	0,0,-100		0,0,1	70"
int	ft_parse_line(t_data *data, char *line)
{
	char **params;
	int	i;
// if line empty, skip it
	if (line[0] == '\n' || line[0] == '#')
		return (free_s(line), 0);
// split the raw
	params = split(line, " \t\n");
	free_s(line);
	if (!params)
		return (put(ERRM"split\n"), 2);
// find the identifier in the dico, process the object with the appropriate function
	i = -1;
	while (dico[++i].name)
	{
		if (same_str(params[0], dico[i].name))
		{
			if (dico[i].exe(data, params + 1))
				return (free_tab(params), put(ERR3"parsing error\n"), 1);
			break ;
		}
	}
	free_tab(params);
	if (!dico[i].name)
		return (put(ERR4"Unknown object\n"), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// check is object vector is too big, create vector space while at it
int	h_parse_vect_space(t_obj *obj, t_vect *view)
{
	if (view)
	{
		if (view->dx < -1.0 || view->dx > 1.0 || 
				view->dy < -1.0 || view->dy > 1.0 || 
				view->dz < -1.0 || view->dz > 1.0)
			return (put(ERR1"Vector should be [-1.0,1.0]\n"), 1);
		ft_normalize_vect(view);
		create_vector_space(obj);
	}
	else
		*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}};
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// 			AMBIENT LIGHT
// 		ambient lighting ratio [0.0,1.0] float
// 		RGB [0, 255] int
int	parse_A(t_data *data, char **raw_split)
{
	t_ambient_light	*light;

	light = mem(0, sizeof(t_ambient_light));
	if (!light)
		return (put(ERRM), 2);
	data->bg_light = (t_ambient_light **)expand_tab((void **)data->bg_light, light);
	
	if (tab_size(raw_split) != 2)
		return (put(ERR1"bad number of args (AMBIENT LIGHT)\n"), 1);
	if (ft_atof(raw_split[0], &light->ratio) ||
		ato_rgb(raw_split[1], &light->color))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) ambient lighting ratio should be [0.0,1.0]\n", raw_split[0]), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			LIGHT
// 		XYZ = float
// 		BRIGHTNESS [0.0,1.0] float
// 		RGB [0, 255] int
int	parse_L(t_data *data, char **raw_split)
{
	t_light	*light;

	light = mem(0, sizeof(t_camera));
	if (!light)
		return (put(ERRM), 2);
	data->light_source = (t_light **)expand_tab((void **)data->light_source, light);
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (LIGHT SOURCE)\n"), 1);

	if (ato_coor(raw_split[0], &light->xyz) || 
		ft_atof(raw_split[1], &light->ratio) || 
		ato_rgb(raw_split[2], &light->color))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) light source ratio should be [0.0,1.0]\n", raw_split[1]), 1);

	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CAMERA
// 		position XYZ = float
// 		vector camera [-1,1] float
// 		FOV [0, 180] int
int	parse_C(t_data *data, char **raw_split)
{
	t_camera	*camera;
	int			err;
	
	camera = mem(0, sizeof(t_camera));
	if (!camera)
		return (put(ERRM), 2);
	data->camera = (t_camera **)expand_tab((void **)data->camera, camera);
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (CAMERA)\n"), 1);
	err = 0;
	camera->fov = ft_atoi(raw_split[2], &err);
	if (err || ato_coor(raw_split[0], &camera->O.c0) || 
		ato_coor(raw_split[1], (t_coor*)&camera->O.view))
		return (1);
	if (camera->fov < 0 || camera->fov > 180)
		return (put(ERR1"(%s) camera fov should be [0, 180]\n", raw_split[0]), 1);
	if(h_parse_vect_space(&camera->O, &camera->O.view))
		return (1);
	camera->fov_cst = tan(camera->fov / 2) / SIZE_SCREEN_X;
	return (0);
}
