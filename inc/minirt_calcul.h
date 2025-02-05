/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_calcul.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/01 20:04:36 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_CALCUL_H
# define MINIRT_CALCUL_H

# include "minirt_general_struct.h"

///////////////////////////////////////////////////////////////////////////////]
//
// 				░█▀▀█ ─█▀▀█ ░█─── ░█▀▀█ ░█─░█ ░█─── ░█▀▀▀█ 
// 				░█─── ░█▄▄█ ░█─── ░█─── ░█─░█ ░█─── ─▀▀▀▄▄ 
// 				░█▄▄█ ░█─░█ ░█▄▄█ ░█▄▄█ ─▀▄▄▀ ░█▄▄█ ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]


///////////////////////////////////////////////////////////////////////////////]
// 	GLOBAL PX CALCUL STRUCT
typedef struct s_calcul_px
{
	// camera origin + vector
	void	*object;
	t_coor	c0;
	t_vect	v;

	// point intersection + vector normal to the surface
	t_coor	inter;
	t_vect	v_normal;
	
	t_rgb	px_color;
	double	dist;

	double	dist_temp;
	int		inside;
	t_coor	point_temp;
	t_coor	tmp_color;
	// double	tmp_dist;

	double dist_light;
	t_vect	v_light;
	double	cos_angle;

	t_rgb	px_color_behind;

}	t_calcul_px;

///////////////////////////////////////////////////////////////////////////////]
// 	CIRCLE
typedef struct s_circle_calc {
	double	top;
	double	bot;

	double	pl_cst;
	t_coor	inter_temp;

	double	dist;
} t_circle_calc;

///////////////////////////////////////////////////////////////////////////////]
// 	SPHERE
typedef struct s_sphere_calc {
	// t_sphere	*sphere;
	double x0;
	double y0;
	double z0;

	double a;
	double b;
	double c;

	double Δ;

	double	det1;
	double	det2;

	double dist;
} t_sphere_calc;


///////////////////////////////////////////////////////////////////////////////]
// 	SPHERE
typedef struct s_plane_calc {
	double	top;
	double	bot;

	double	dist;
} t_plane_calc;

typedef struct s_cylinder_calc {
	double	radius;

	double	A;
	double	B;
	
	double x0;
	double y0;
	double z0;
	double x1;
	double y1;
	double z1;

	double a;
	double b;
	double c;

	double Δ;
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
} t_cylinder_calc;

typedef struct s_cone_calc {
	double	φ;

	double	b1;
	double	b2;
	double	b3;

	double	Φ;

	double	a1;
	double	a2;
	double	a3;
	double	slope;

	double A;
	double B;
	double C;

	double Δ;
	double	det1;
	double	det2;

	double	dist_apex;
	double	dist;
	t_coor	projec_point;
} t_cone_calc;

typedef struct s_camera_calc {
	double cosA;
	double sinA;
	double cosB;
	double sinB;

	double Qw;
	double Qi;
	double Qj;
	double Qk;
} t_camera_calc;

#endif