/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_general_struct.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/12 11:58:44 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_GENERAL_STRUCT_H
# define MINIRT_GENERAL_STRUCT_H

///////////////////////////////////////////////////////////////////////////////]
// 
// 			█▀▀▄ █▀▀█ ▀▀█▀▀ █▀▀█ 　 █▀▀ ▀▀█▀▀ █▀▀█ █░░█ █▀▀ ▀▀█▀▀ 
// 			█░░█ █▄▄█ ░░█░░ █▄▄█ 　 ▀▀█ ░░█░░ █▄▄▀ █░░█ █░░ ░░█░░ 
// 			▀▀▀░ ▀░░▀ ░░▀░░ ▀░░▀ 　 ▀▀▀ ░░▀░░ ▀░▀▀ ░▀▀▀ ▀▀▀ ░░▀░░
//
///////////////////////////////////////////////////////////////////////////////]

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_coor
{
	double	x;
	double	y;
	double	z;
}	t_coor;

typedef struct s_vect
{
	double	dx;
	double	dy;
	double	dz;
}	t_vect;

typedef struct s_img
{
	void	*img;
	char	*addr;

	int		bpp;
	int		ll;
	int		end;
	int		sz_x;
	int		sz_y;
}	t_img;

////////////////////////////////////////////]
typedef struct s_obj
{
	t_coor		c0;
	t_vect		view;
	t_vect		up;
	t_vect		right;

}	t_obj;


typedef struct s_param
{
	t_rgb	color;
	t_rgb	color2;

	double	shiny;
	double	specular;
	double	mirror;
	double	transparence;
	double	gamma;
	t_img	*texture;
	t_img	*normal_map;

}	t_param;//		objeccts param


typedef struct s_obj2
{
	int		type;
	t_obj	O;
	t_param	param;

}	t_obj2;//		objeccts param

#endif