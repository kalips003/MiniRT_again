/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/08 00:58:44 by kalipso          ###   ########.fr       */
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


////////////////////////////////////////////]
////////////////////////////////////////////]
typedef struct s_eye
{
	t_camera	*c;
// unit angle in radians per pixel
	double		px;
// position of upper-left screen (rotation in radian)
	double		px0;
	double		py0;

	int			current_camera;

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

// shortcut to the vector camera
	t_eye	eye;
	int		current_camera;

	char	**av;
	time_t	last_modif_time;


	t_sphere	*change_obj;
	int		change;

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