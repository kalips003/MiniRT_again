/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/10 17:39:29 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		direction_pad(int keysym, t_data *data);
int		keys_wasd(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
int	direction_pad(int keysym, t_data *data)
{
	t_coor	*xyz;

	if (!data->change && data->change_obj)
		xyz = &data->change_obj->O.c0;
	else
		xyz = &data->eye.c->O.c0;
	
	if (keysym == XK_Home)
		move_point(xyz, &data->eye.c->O.up, DELTA_MOV);
	else if (keysym == XK_End)
		move_point(xyz, &data->eye.c->O.up, -DELTA_MOV);
	else if (keysym == XK_Up)
		move_point(xyz, &data->eye.c->O.view, DELTA_MOV);
	else if (keysym == XK_Down)
		move_point(xyz, &data->eye.c->O.view, -DELTA_MOV);
	else if (keysym == XK_Right)
		move_point(xyz, &data->eye.c->O.right, DELTA_MOV);
	else if (keysym == XK_Left)
		move_point(xyz, &data->eye.c->O.right, -DELTA_MOV);
	else
		return (0);
	return (recalculate_obj_const(data->change_obj), 1);
}

int	keys_wasd(int keysym, t_data *data)
{
	t_obj	*obj;

	if (!data->change && data->change_obj)
		obj = &data->change_obj->O;
	else
		obj = &data->eye.c->O;

	if (keysym == XK_a)
		rotation_obj(obj, &obj->up, PI / 8, 1);
	else if (keysym == XK_d)
		rotation_obj(obj, &obj->up, PI / 8, -1);
	else if (keysym == XK_w)
		rotation_obj(obj, &obj->right, PI / 8, 1);
	else if (keysym == XK_s)
		rotation_obj(obj, &obj->right, PI / 8, -1);
	else if (keysym == XK_q)
		rotation_obj(obj, &obj->view, PI / 8, 1);
	else if (keysym == XK_e)
		rotation_obj(obj, &obj->view, PI / 8, -1);
	else if (keysym == XK_n)
	{
		data->eye.current_camera++;
		data->eye.c = data->camera[data->eye.current_camera];
		if (!data->eye.c)
		{
			data->eye.c = data->camera[0];
			data->eye.current_camera = 0;
		}
	}
	else
		return (0);
	return (recalculate_obj_const(data->change_obj), 1);

}

///////////////////////////////////////////////////////////////////////////////]
typedef void t_ft_change(t_data *data, t_obj *obj);
void	function_1(t_data *data, t_obj *obj);

typedef struct s_dico_pair2
{
	char		*name;
	t_ft_change	*exe;
}	t_dico_pair2;

static const t_dico_pair2	dico[] = {
	{"Moves to the Right", function_1},
	{NULL, NULL}
};

// funcction de transformatioin basee sur un tableau, touches "-" et "+" swap through
void	function_1(t_data *data, t_obj *obj)
{
	move_point(&obj->c0, &obj->right, 5.0);
}

void	function_2(t_data *data, t_obj *obj)
{
	// move color loop
	move_point(&obj->c0, &obj->right, 5.0);
}
///////////////////////////////////////////////////////////////////////////////]


