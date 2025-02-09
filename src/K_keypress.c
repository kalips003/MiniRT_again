/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 14:14:56 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	key_press(int keysym, t_data *data);
int	mouse_clic(int button, int x, int y, void *data);
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


// int mlx_mouse_hook(void *win_ptr, int (*funct_ptr)(), void *param);
// int mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct_ptr)(), void *param);
// ButtonPress = 4, ButtonRelease = 5, MotionNotify = 6
// Left Button: 1, Middle Button: 2, Right Button: 3, Scroll Up: 4, Scroll Down: 5
int mouse_clic(int button, int x, int y, void *data)
{
	if (button == 3) // Right mouse button
		print_clic((t_data *)data, x, y);
	return (0);
}

void	print_clic(t_data *data, int x, int y)
{
	t_calcul_px c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->O.c0;
	f_calculate_combined_quaternion(data, atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst), atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst), &c.v);
	calculate_pixel_color(data, &c, 0);

	printf(CLEAR);
	printf("Camera = [%f,%f,%f\t%f,%f,%f]\n\n", data->eye.c->O.c0.x, data->eye.c->O.c0.y, data->eye.c->O.c0.z, c.v.dx, c.v.dy, c.v.dz);
	printf("Mouse clicked at position (%d, %d)\n", x, y);
	if (c.object)
	{
		t_obj *obj = &((t_sphere*)c.object)->O;
		printf("\tOBJECT: view= %f,%f,%f\nup= %f,%f,%f\nright= %f,%f,%f\n", \
		obj->view.dx, obj->view.dy, obj->view.dz,\
		obj->up.dx, obj->up.dy, obj->up.dz, \
		obj->right.dx, obj->right.dy, obj->right.dz);
	}
	printf("Vector normal surface = [%f, %f, %f]\n", c.v_normal.dx, c.v_normal.dy, c.v_normal.dz);
	printf("Intersection point = [%f, %f, %f]\n", c.inter.x, c.inter.y, c.inter.z);

// set the object thats gonna change
	if (c.object == data->change_obj)
		data->change_obj = NULL;
	else
		data->change_obj = c.object;
	printf("\tOBJECT.1 = %p;OBJECT.2 = %p\n", c.object, data->change_obj);
	c.dist = -1.0;
	c.c0 = c.inter;

}


