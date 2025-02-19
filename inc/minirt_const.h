/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_const.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/19 14:25:33 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_CONST_H
# define MINIRT_CONST_H

///////////////////////////////////////////////////////////////////////////////]
// 
// 			░█▀▀█ ░█▀▀▀█ ░█▄─░█ ░█▀▀▀█ ▀▀█▀▀ ─█▀▀█ ░█▄─░█ ▀▀█▀▀ ░█▀▀▀█ 
// 			░█─── ░█──░█ ░█░█░█ ─▀▀▀▄▄ ─░█── ░█▄▄█ ░█░█░█ ─░█── ─▀▀▀▄▄ 
// 			░█▄▄█ ░█▄▄▄█ ░█──▀█ ░█▄▄▄█ ─░█── ░█─░█ ░█──▀█ ─░█── ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]

# define PI 3.14159265358979323846

# define EPSILON 1e-6
# define SCALAR_LIGHT_DIST 300.0
# define SCALAR_SPECULAR 1.0
# define SCALAR_SHINY 1.0
# define REFLECTION_BOUNCES 1
# define TRANSPARENCE_BOUNCES 8

# define SIZE_SCREEN_Y 800
# define SIZE_SCREEN_X 1200

#define CIRCLE 0
#define SPHERE 1
#define PLANE 2
#define CYLINDER 3
#define CONE 4
#define PARABOLOID 5
#define ARROW 6
#define CUBE 7
#define DOUBLE_PLAN 8
#define SPRITE 9

#define DELTA_MOV 4.0
#define DELTA_ROTA (PI / 16)

# define SQRT3 1.7320508075688771931766041
///////////////////////////////////////////////////////////////////////////////]
# define ANTI_ALIASING 0

#endif