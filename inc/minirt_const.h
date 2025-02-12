/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_const.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/12 01:01:09 by kalipso          ###   ########.fr       */
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
# define TRANSPARENCE_BOUNCES 4

# define SIZE_SCREEN_Y 400
# define SIZE_SCREEN_X 600

#define CIRCLE 0
#define SPHERE 1
#define PLANE 2
#define CYLINDER 3
#define CONE 4
#define PARABOLOID 5
#define ARROW 6

#define DELTA_MOV 5.0
#define DELTA_ROTA (PI / 10)

///////////////////////////////////////////////////////////////////////////////]
// 	UNUSED?

// CONST ROTATION 5 degree
# define COS_ROTA 0.99619469809174554520
# define COS_ROTA2 0.99240387650610406567
# define SIN_ROTA 0.08715574274765816587
# define SIN_ROTA2 0.00759612349389596903
# define COSSIN_ROTA 0.08682408883346516559

#endif