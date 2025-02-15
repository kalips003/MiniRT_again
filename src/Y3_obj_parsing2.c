/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y3_obj_parsing2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/14 22:20:07 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_ar(t_data *data, char **raw_split);
int	parse_cu(t_data *data, char **raw_split);
int	parse_dp(t_data *data, char **raw_split);
int	parse_xi(t_data *data, char **raw_split);

///////////////////////////////////////////////////////////////////////////////]
// 			ARROW
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		RADIUS = float
// 		HEIGHT = float
// 		RGB [0, 255] int
int	parse_ar(t_data *data, char **raw_split)
{
	t_arrow	*arrow;
	
	arrow = mem(0, sizeof(t_arrow));
	if (!arrow)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, arrow);

	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (ARROW OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &arrow->param))
		return (1);

	arrow->type = ARROW;
	if (ato_coor(raw_split[0], &arrow->O.c0) ||
		ato_coor(raw_split[1], (t_coor *)&arrow->O.view) ||
		ft_atof(raw_split[2], &arrow->radius) ||
		ft_atof(raw_split[3], &arrow->height) ||
		ato_argb(raw_split[4], &arrow->param.argb))
		return (1);
	if (arrow->radius < EPSILON || arrow->height < EPSILON)
		return (put(ERR1"(ARROW OBJECT) too small\n"), 1);

	if (h_parse_vect_space(&arrow->O, &arrow->O.view))
		return (1);
	arrow->xyz_other = new_moved_point(&arrow->O.c0, &arrow->O.view, arrow->height * 2 / 3);
	arrow->apex = new_moved_point(&arrow->O.c0, &arrow->O.view, arrow->height);
	arrow->slope = (9 * arrow->radius * arrow->radius) / (arrow->height * arrow->height);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CUBE
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		Size	double
// 		RGB [0, 255] int
int	parse_cu(t_data *data, char **raw_split)
{
	t_cube	*cube;
	
	cube = mem(0, sizeof(t_cube));
	if (!cube)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, cube);

	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (CUBE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &cube->param))
		return (1);
	
	cube->type = CUBE;
	if (ato_coor(raw_split[0], &cube->O.c0) ||
		ato_coor(raw_split[1], (t_coor*)&cube->O.view) ||
		ft_atof(raw_split[2], &cube->size) ||
		ato_argb(raw_split[3], &cube->param.argb))
		return (1);
	if (h_parse_vect_space(&cube->O, &cube->O.view))
		return (1);
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
// 			DOUBLE_PLAN
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		Width	double
// 		RGB [0, 255] int
int	parse_dp(t_data *data, char **raw_split)
{
	t_dblplane	*dbpl;
	
	dbpl = mem(0, sizeof(t_dblplane));
	if (!dbpl)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, dbpl);

	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (BIPLANE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &dbpl->param))
		return (1);
	
	dbpl->type = DOUBLE_PLAN;
	if (ato_coor(raw_split[0], &dbpl->O.c0) ||
		ato_coor(raw_split[1], (t_coor*)&dbpl->O.view) ||
		ft_atof(raw_split[2], &dbpl->width) ||
		ato_argb(raw_split[3], &dbpl->param.argb))
		return (1);
	if (h_parse_vect_space(&dbpl->O, &dbpl->O.view))
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			SPRITE
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		Size float
// 		RGB [0, 255] int
// 
int	parse_xi(t_data *data, char **raw_split)
{
	t_sprite	*sprite;
	
	sprite = mem(0, sizeof(t_sprite));
	if (!sprite)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, sprite);

	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (SPRITE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &sprite->param))
		return (1);
	if (!sprite->param.texture)
		return (put(ERR1"SPRITE OBJECT need a sprite\n"), 1);

	sprite->type = SPRITE;
	if (ato_coor(raw_split[0], &sprite->O.c0) ||
		ato_coor(raw_split[1], (t_coor*)&sprite->O.view) ||
		ft_atof(raw_split[2], &sprite->size) ||
		ato_argb(raw_split[3], &sprite->param.argb))
		return (1);
	if (h_parse_vect_space(&sprite->O, &sprite->O.view))
		return (1);
	sprite->d = -(sprite->O.up.dx * sprite->O.c0.x + sprite->O.up.dy * sprite->O.c0.y + sprite->O.up.dz * sprite->O.c0.z);
	return (0);
}