/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/18 09:27:55 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		key_press(int keysym, t_data *data);
int		mouse_clic(int button, int x, int y, void *data);
int		mouse_release(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);

///////////////////////////////////////////////////////////////////////////////]
int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	if (keysym == XK_KP_Enter)
		return (ft_render_frame(data, 1));

	if (direction_pad(keysym, data))
		return (ft_render_frame(data, 0));
	else if (keys_wasd(keysym, data))
		return (ft_render_frame(data, 0));
	else if (keysym == XK_space)
		data->change ^= 1;
	else
		return (0);
	ft_render_frame(data, 0);
	return (0);
}

// Left Button: 1, Middle Button: 2, Right Button: 3, Scroll Up: 4, Scroll Down: 5
int mouse_clic(int button, int x, int y, void *data)
{
	if (button == 3) // Right mouse button
		print_clic((t_data *)data, x, y);
	else if (button == 1)
	{
		((t_data*)data)->eye.clic_px_x = x;
		((t_data*)data)->eye.clic_px_y = y;
	}
	else if (button == 4 || button == 5)
	{
		if (button == 4)
			((t_data*)data)->zoom *= 2;
		else
			((t_data*)data)->zoom /= 2;
		printf(C_411"ZOOMED MOVEMENT: "RESET RED"%.3f\n"RESET, ((t_data*)data)->zoom);
	}
	return (0);
}

int mouse_release(int button, int x, int y, void *data)
{
	double angleA;
	double angleB;

	if (button == 1)
	{
		angleA = 1.5 * (((t_data*)data)->eye.clic_px_x - x) * ((t_data*)data)->eye.c->fov_cst;
		angleB = 1.5 * (((t_data*)data)->eye.clic_px_y - y) * ((t_data*)data)->eye.c->fov_cst;
		((t_data*)data)->eye.c->O.view = combined_quaternion_rotation(&((t_data*)data)->eye.c->O, angleA, angleB);
		create_vector_space(&((t_data*)data)->eye.c->O);
		ft_render_frame((t_data *)data, 0);
	}
	return (0);
}

static const char* g_obj_names[] = {
	"Circle",
	"Sphere",
	"Plane",
	"Cylinder",
	"Cone",
	"Paraboloid",
	"Arrow",
	"Cube",
	"Bi-Plane",
	"Sprite",
	NULL
};

void	print_clic(t_data *data, int x, int y)
{
	t_calcul_px c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->O.c0;
	c.print = 1;
	c.v = combined_quaternion_rotation(&data->eye.c->O, atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst), atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst));
	printf(CLEAR);
	printf(C_512"Camera\t[%.1f,%.1f,%.1f\t\t%.3f,%.3f,%.3f]\n\n", data->eye.c->O.c0.x, data->eye.c->O.c0.y, data->eye.c->O.c0.z, c.v.dx, c.v.dy, c.v.dz);
	printf("\tview \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.view.dx, data->eye.c->O.view.dy, data->eye.c->O.view.dz);
	printf("\tup \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.up.dx, data->eye.c->O.up.dy, data->eye.c->O.up.dz);
	printf("\tright \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.right.dx, data->eye.c->O.right.dy, data->eye.c->O.right.dz);
	printf(C_323"\nMouse clicked at pixel (%d, %d)\n", x, y);
	
	printf(C_244"\n-------------------[INSIDE PRINT DATA]-------------------\n");
	calculate_pixel_color(data, &c, 1);
	printf(C_244"---------------------------------------------------------\n");

	if (c.object)
	{
		t_obj2 *obj = c.object;
		printf(C_142"\nOBJECT:\t\t%s", g_obj_names[obj->type]);
		printf(C_533"\n\n\txyz \t[%.1f,%.1f,%.1f]\n", obj->O.c0.x, obj->O.c0.y, obj->O.c0.z);
		printf(C_455"\n\tview \t[%.2f,%.2f,%.2f]\n", obj->O.view.dx, obj->O.view.dy, obj->O.view.dz);
		printf("\tup \t[% .2f,% .2f,% .2f]\n", obj->O.up.dx, obj->O.up.dy, obj->O.up.dz);
		printf("\tright \t[% .2f,% .2f,% .2f]\n", obj->O.right.dx, obj->O.right.dy, obj->O.right.dz);
		printf(C_433"\n\tcolor obj\t[%d,%d,%d]\n", obj->param.argb.r, obj->param.argb.g, obj->param.argb.b);
		printf(C_433"\tpx color\t[%d,%d,%d]\n\n", c.argb.r, c.argb.g, c.argb.b);
	}
	printf(C_134"Vector normal surface\t[%.3f, %.3f, %.3f]\n", c.v_normal.dx, c.v_normal.dy, c.v_normal.dz);
	printf("Intersection point\t[%.2f, %.2f, %.2f]\n\n", c.inter.x, c.inter.y, c.inter.z);

	if (c.object == data->change_obj)
		data->change_obj = NULL;
	else
		data->change_obj = c.object;
	printf(C_440"OBJECT SELECTED %p\n", data->change_obj);
}
