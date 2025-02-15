/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_objects.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/14 20:45:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "minirt_general_struct.h"

///////////////////////////////////////////////////////////////////////////////]
// 
// 				░█▀▀▀█ ░█▀▀█ ───░█ ░█▀▀▀ ░█▀▀█ ▀▀█▀▀ ░█▀▀▀█ 
// 				░█──░█ ░█▀▀▄ ─▄─░█ ░█▀▀▀ ░█─── ─░█── ─▀▀▀▄▄ 
// 				░█▄▄▄█ ░█▄▄█ ░█▄▄█ ░█▄▄▄ ░█▄▄█ ─░█── ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]

// AMBIENT LIGHT = A
typedef struct s_ambient_light
{
	double	ratio;
	t_argb	color;
	t_img	*texture;
}	t_ambient_light;

// CAMERA = C
typedef struct s_camera
{
	t_obj	O;
	int		fov;
	double	fov_cst;// = tan(fov/2) / screenX

}	t_camera;

// LIGHT SOURCE = L
typedef struct s_light
{
	t_coor	xyz;
	double	ratio;
	t_argb	color;
}	t_light;

////////////////////////////////////////////]
// SPHERE = sp
typedef struct s_sphere
{
	int		type;
	t_obj	O;
	t_param	param;

	double	radius;
}	t_sphere;

// PLANE = pl
typedef struct s_plane
{
	int		type;
	t_obj	O;
	t_param	param;

	double	d;
}	t_plane;

// CYLINDER = cy
typedef struct s_cylinder
{
	int		type;
	t_obj	O;
	t_param	param;

	t_coor	xyz_other;
	double	height;
	double	radius;
}	t_cylinder;

// CIRCLE = ci
typedef struct s_cicle
{
	int		type;
	t_obj	O;
	t_param	param;

	double	radius;
}	t_circle;

// CONE = co
typedef struct s_cone
{
	int		type;
	t_obj	O;
	t_param	param;

	double	height;
	double	radius;

	t_coor	apex;
	double	slope;
}	t_cone;

// ARROW = co
typedef struct s_arrow
{
	int		type;
	t_obj	O;
	t_param	param;

	double	height;
	double	radius;

	t_coor	xyz_other;
	t_coor	apex;
	double	slope;
}	t_arrow;

// SQUARE = sq
typedef struct s_cube
{
	int		type;
	t_obj	O;
	t_param	param;

	double	size;
	t_coor	other_p;
	
	double	d1;
	double	d2;
	double	d3;
	t_coor	other_p2;
	t_coor	other_p3;
	
}	t_cube;

// PLANE = pl
typedef struct s_dblplane
{
	int		type;
	t_obj	O;
	t_param	param;

	double	d;
	double	width;
	t_coor	other_point;
	double	other_d;

}	t_dblplane;

// PLANE = pl
typedef struct s_sprite
{
	int		type;
	t_obj	O;
	t_param	param;

	double	d;
	double	size;
}	t_sprite;

#endif