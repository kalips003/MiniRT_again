/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/18 10:41:03 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_STRUCT_H
# define MINIRT_STRUCT_H

# include <time.h>

# include "minirt_objects.h"
# include "minirt_general_struct.h"

///////////////////////////////////////////////////////////////////////////////]
//
// 		▒█▀▀▀█ ▀▀█▀▀ ▒█▀▀█ ▒█░▒█ ▒█▀▀█ ▀▀█▀▀ ▒█░▒█ ▒█▀▀█ ▒█▀▀▀ ▒█▀▀▀█ 
// 		░▀▀▀▄▄ ░▒█░░ ▒█▄▄▀ ▒█░▒█ ▒█░░░ ░▒█░░ ▒█░▒█ ▒█▄▄▀ ▒█▀▀▀ ░▀▀▀▄▄ 
// 		▒█▄▄▄█ ░▒█░░ ▒█░▒█ ░▀▄▄▀ ▒█▄▄█ ░▒█░░ ░▀▄▄▀ ▒█░▒█ ▒█▄▄▄ ▒█▄▄▄█
//
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
	
	// t_rgb	px_color;
	t_argb	argb;
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
	double	previous_gamma;
	// double	transp_l_ratio;
	// t_coor	transp_light;
	t_light	transp_light;
	double	eff_trans;
// 
	t_coor	diffuse;
	t_coor	reflected;
	t_coor	behind;

	// bit for printing info
	int	print;
	
}	t_calcul_px;

////////////////////////////////////////////]
////////////////////////////////////////////]
typedef struct s_eye
{
	t_camera	*c;
	// unit angle in radians per pixel
	// 	double		px;
	// // position of upper-left screen (rotation in radian)
	// 	double		px0;
	// 	double		py0;
	
	int			current_camera;
	int			clic_px_x;
	int			clic_px_y;
	
}	t_eye;

////////////////////////////////////////////]
typedef struct s_data
{
	void	*mlx;
	void	*win;
	
	// t_img	buffer; <---- buffer not used, all buffer lines commented
	
	t_ambient_light	**bg_light;
	t_camera		**camera;
	t_light			**light_source;
	
	void		**objects;
	t_img		**textures;
	
	t_img		buffer;
	// shortcut to the vector camera
	t_eye	eye;
	int		current_camera;
	
	char	**av;
	time_t	last_modif_time;
	
	
	t_obj2	*change_obj;
	int		change;
	double	zoom;
	
}	t_data;
////////////////////////////////////////////]
////////////////////////////////////////////]
////////////////////////////////////////////]

typedef int (*t_ft_parse_object)(t_data *data, char **split);

typedef struct s_parsing_dico_pair
{
	char				*name;
	t_ft_parse_object	exe;
}	t_dico_pair;

typedef int (*t_ft_param_parsing)(t_data*, char*, t_param*);

#endif