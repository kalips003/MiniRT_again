/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/25 02:22:09 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	keys_wasd(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
int	keys_wasd(int keysym, t_data *data)
{
	if (data->change && data->change_obj)
	{
		if (keysym == XK_Up)
			move_point(&data->change_obj->c0, &data->eye.c->view, 5.0);
		else if (keysym == XK_Down)
			move_point(&data->change_obj->c0, &data->eye.c->view, -5.0);
		else if (keysym == XK_Right)
			move_point(&data->change_obj->c0, &data->eye.c->right, -5.0);
		else// (keysym == XK_Left)
			move_point(&data->change_obj->c0, &data->eye.c->right, +5.0);
	}
	else
	{
		if (keysym == XK_Up)
			move_point(&data->eye.c->xyz, &data->eye.c->view, 5.0);
		else if (keysym == XK_Down)
			move_point(&data->eye.c->xyz, &data->eye.c->view, -5.0);
		else if (keysym == XK_Right)
			move_point(&data->eye.c->xyz, &data->eye.c->right, -5.0);
		else// (keysym == XK_Left)
			move_point(&data->eye.c->xyz, &data->eye.c->right, +5.0);
	}
}
