/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_calcul.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/13 16:21:19 by kalipso          ###   ########.fr       */
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

// lighting values
	double dist_light;
	t_vect	v_light;
	double	cos_angle;

// 
	int	reflected_depth;
// transparence
	int	transparence_depth;
	double	current_gamma;
	// double	transp_l_ratio;
	// t_coor	transp_light;
	t_light	transp_light;
// 
	t_coor	diffuse;
	t_coor	reflected;
	t_coor	behind;

// bit for printing info
	int	print;

}	t_calcul_px;

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// 	CIRCLE
typedef struct s_circle_calc {
	double	top;
	double	bot;

	double	pl_cst;
	t_coor	inter_temp;

	double	dist;
	double	dist_center;
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
// 	PLANE
typedef struct s_plane_calc {
	t_vect	o_to_inter;
	double	top;
	double	bot;

	double	dist;

	double	u;
	double	v;
} t_plane_calc;


///////////////////////////////////////////////////////////////////////////////]
// 	CYLINDER
typedef struct s_cylinder_calc_v1 {

	double	A;
	double	B;
	
	double	x0;
	double	y0;
	double	z0;
	double	x1;
	double	y1;
	double	z1;

	double	a;
	double	b;
	double	c;

	double	Δ;
} t_cylinder_calc_v1;

typedef struct s_cylinder_calc_v2 {
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
	t_circle	circle;
} t_cylinder_calc_v2;

///////////////////////////////////////////////////////////////////////////////]
// 	CONE
typedef struct s_cone_calc_v1 {
	double	φ;
	double	Φ;

	double	a1;
	double	a2;
	double	a3;

	double	b1;
	double	b2;
	double	b3;

	double	A;
	double	B;
	double	C;

	double	Δ;
} t_cone_calc_v1;

typedef struct s_cone_calc_v2 {
	double	det1;
	double	det2;

	double	dist_apex1;
	double	dist_apex2;
	double	dist_apex;
	double	dist;

	double	color_height;

	t_coor	projec_point;
	t_circle	circle;
} t_cone_calc_v2;

///////////////////////////////////////////////////////////////////////////////]
// 	ARROW
typedef struct s_arrow_calc {
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
	t_cone		co;
	t_cylinder	cy;
} t_arrow_calc;

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
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