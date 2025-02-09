/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/09 14:28:47 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// # include <pthread.h>
// # include <sys/stat.h>
// # include <signal.h>
// # include <dirent.h>
// # include <termios.h>
// # include <termcap.h>
// # include <sys/ioctl.h>
// # include <readline/readline.h>
// # include <readline/history.h>
// # include <signal.h>
// # include <time.h>
// # include <sys/types.h>

# include "../mlx_linux/mlx.h"
# include <X11/X.h>
# include <math.h>
# include <X11/keysym.h>

# include "libft.h"

# include "minirt_const.h"
# include "minirt_struct.h"
# include "minirt_calcul.h"
# include "minirt_objects.h"
# include "minirt_general_struct.h"


///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]


///////////////////////////////////////////////////////////////////////////////]
/********************************
		A
********************************/
void	create_vector_space(t_obj *obj);
void	f_calculate_combined_quaternion_better(t_obj *obj, double angle_α, double angle_β, t_vect *rotated);
/********************************
		B
********************************/
int		ft_loop(t_data *data);
int		ft_render_frame(t_data *data, int sublim);
void	calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim);
/********************************
		C
********************************/
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);
/********************************
		D	Objects
********************************/
// CIRCLE
int		distance_from_circle(t_calcul_px *calcul, void *obj, int simple);
// 	SPHERE
int		distance_from_sphere(t_calcul_px *calcul, void *obj, int simple);
// PLANE
int		distance_from_plane(t_calcul_px *calcul, void *obj, int simple);
// CYLINDER
int		distance_from_cylinder(t_calcul_px *calcul, void *obj, int simple);
// 	CONE
int		distance_from_cone(t_calcul_px *calcul, void *obj, int simple);
/********************************
		E
********************************/
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
void	ft_lighting_simple(t_data *data, t_calcul_px *c);
void	ft_lighting_sublim(t_data *data, t_calcul_px *c);
// 
int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);
// 
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_rgb	what_is_reflected(t_data *data, t_calcul_px *calcul);
t_rgb	what_is_behind(t_data *data, t_calcul_px *calcul);
/********************************
		K
********************************/
int	key_press(int keysym, t_data *data);
int	mouse_clic(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);
// tools
void	function_1(t_data *data, t_obj *obj);
int	direction_pad(int keysym, t_data *data);
int	keys_wasd(int keysym, t_data *data);
/********************************
		T	Tools
********************************/
// atof
int			ft_atof_v2(char *string, double *rtrn);
int			ato_coor_v2(char *str, t_coor *xyz);
int			ato_rgb_v2(char *str, t_rgb *rgb);
// vector ope
int		ft_normalize_vect_v2(t_vect *vect, int print_err);
double	ft_dot_product(t_vect *a, t_vect *b);
t_vect	ft_cross_product(t_vect *u, t_vect *v);
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
// vector mouv
void	move_point(t_coor* p, t_vect *v, double incre);
t_coor	new_moved_point(t_coor* p, t_vect *v, double dist);
t_vect	vect_ab(t_coor* a, t_coor* b);
t_vect	vect_ab_norm(t_coor* a, t_coor* b);
// tools other
int		ft_normalize_vect(t_vect *vect);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);
double	h_smalest_Δ(double a, double b);
// 
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
void	h_vector_space(t_obj *obj);
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, int posi_neg);
/********************************
		Y
********************************/
void	initialization(int ac, char **av, t_data *data);
void	read_file(int ac, char **av, t_data *data);
// 
int	ft_parse_line(t_data *data, char *line);
int	h_parse_vect_space(t_obj *obj, t_vect *view);
int	parse_A(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);
// 
int	parse_ci(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);
int	parse_co(t_data *data, char **raw_split);
// 
int	parse_reste(t_data *data, char **raw_split, t_param *obj);
int	parse_shininess(t_data *data, char *raw, t_param *obj);
int	parse_transparence(t_data *data, char *raw, t_param *obj);
int	parse_mirror(t_data *data, char *raw, t_param *obj);
int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
/********************************
		TTT	Tools
********************************/
// atof
int		ft_atof_v2(char *string, double *rtrn);
int		ato_coor_v2(char *str, t_coor *xyz);
int		ato_rgb_v2(char *str, t_rgb *rgb);
// vect ope
int		ft_normalize_vect_v2(t_vect *vect, int print_err);
double	ft_dot_product(t_vect *a, t_vect *b);
t_vect	ft_cross_product(t_vect *u, t_vect *v);
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
// vect mouv
void	move_point(t_coor* p, t_vect *v, double incre);
t_coor	new_moved_point(t_coor* p, t_vect *v, double dist);
t_vect	vect_ab(t_coor* a, t_coor* b);
t_vect	vect_ab_norm(t_coor* a, t_coor* b);
// other
int		ft_normalize_vect(t_vect *vect);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);
double	h_smalest_Δ(double a, double b);
/********************************
		YYY
********************************/
int	ft_parse_line(t_data *data, char *line);
int	parse_reste(t_data *data, char **raw_split, t_param *obj);
int	h_parse_vect_space(t_obj *obj, t_vect *view);
// 
int	parse_A(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);
int	parse_ci(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);
int	parse_co(t_data *data, char **raw_split);
// 
int	parse_shininess(t_data *data, char *raw, t_param *obj);
int	parse_transparence(t_data *data, char *raw, t_param *obj);
int	parse_mirror(t_data *data, char *raw, t_param *obj);
int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
/********************************
		Z
********************************/
void	end(t_data *data, int exit_code, int full_clean);
int	end2(t_data *data);


#endif
