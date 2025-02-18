/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/18 13:29:17 by kalipso          ###   ########.fr       */
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
t_argb	return_px_img(t_img *img, double x, double y);
t_vect	return_vect_img(t_img *img, double x, double y);
int		return_alpha_img(t_img *img, double x, double y);

int	ft_diffuse_sublim(t_data *data, t_calcul_px *c, t_light *lights);
int light_tracing_sublim(t_data *data, t_calcul_px *calcul, t_light *light);

void	ft_refracted_sublim(t_data *data, t_calcul_px *c, int sublim);
t_argb	what_is_behind_sublim(t_data *data, t_calcul_px *calcul, int sublim);

void	put_pixel_buffer(t_data *data, int x, int y, unsigned int color);
void	ft_anti_aliasing(t_data *data);
unsigned int	ft_average_pixel(t_data *data, int x, int y);
void	ft_weighted_color(t_img *buffer, int x, int y, t_coor *rtrn, double scalar);
/********************************
		B
********************************/
int		ft_loop(t_data *data);
int		ft_render_frame(t_data *data, int sublim);
int		calculate_pixel_color(t_data *data, t_calcul_px *c, int sublim);
/********************************
		C
********************************/
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);
int	ft_find_pixel_colision_sublim(t_data *data, t_calcul_px *c);
/********************************
		D	Objects
********************************/
// CIRCLE
int		distance_from_circle(t_calcul_px *calcul, void *obj, int simple);
// 	SPHERE
int		distance_from_sphere(t_calcul_px *calcul, void *obj, int simple);
// PLANE
int		distance_from_plane(t_calcul_px *calcul, void *obj, int simple);
void	h_txt_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane *plane);
void	h_nmap_plane(t_calcul_px *calcul, t_plane_calc* c, t_plane* plane);
// CYLINDER
int		distance_from_cylinder(t_calcul_px *calcul, void *obj, int simple);
// 	CONE
int		distance_from_cone(t_calcul_px *calcul, void *obj, int simple);
// 	ARROW
int		distance_from_arrow(t_calcul_px *calcul, void *obj, int simple);
// 	CUBE
int	distance_from_cube(t_calcul_px *calcul, void *obj, int simple);
// 	BIPLANE
int	distance_from_dblplane(t_calcul_px *calcul, void *obj, int simple);
// 	SPRITE
int	distance_from_sprite(t_calcul_px *calcul, void *obj, int simple);
/********************************
		E
********************************/
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
void	ft_lighting_simple(t_data *data, t_calcul_px *c);
void	ft_lighting_sublim(t_data *data, t_calcul_px *c);
// 
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
int		ft_diffuse(t_data *data, t_calcul_px *c, t_light *lights);
void	ft_reflected(t_data *data, t_calcul_px *c);
void	ft_refracted(t_data *data, t_calcul_px *c);
void	ft_specular(t_data *data, t_calcul_px *c, t_light *lights);
// 
int		something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);
double	calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_vect	ft_vect_refracted_v2(t_vect *incident, t_vect *normal, double gamma_incident, double gamma_obj);
t_argb	what_is_reflected(t_data *data, t_calcul_px *calcul);
t_argb	what_is_behind(t_data *data, t_calcul_px *calcul);
/********************************
		K
********************************/
int		key_press(int keysym, t_data *data);
int		mouse_clic(int button, int x, int y, void *data);
int		mouse_release(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);
// tools
int		direction_pad(int keysym, t_data *data);
int		keys_wasd(int keysym, t_data *data);
/********************************
		T	Tools
********************************/
// atof
int		ft_atof(char *string, double *rtrn);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);
int		ato_argb(char *str, t_argb *argb);
// tools other
void	create_vector_space(t_obj *obj);
double	h_smalest_Δ(double a, double b);
void	recalculate_obj_const(t_obj2 *obj);
t_argb	dual_color_render(t_argb *color1, t_rgb *color2, double dist);
// rotation
t_vect	combined_quaternion_rotation(t_obj *obj, double angle_α, double angle_β);
t_vect	quaternion_rotation(t_vect *v, t_vect *axis_rota, double angle_α, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_α, int posi_neg);
// vector mouv
void	move_point(t_coor* p, t_vect *v, double incre);
t_coor	new_moved_point(t_coor* p, t_vect *v, double dist);
t_vect	vect_ab(t_coor* a, t_coor* b);
t_vect	vect_ab_norm(t_coor* a, t_coor* b);
// vector ope
int		ft_normalize_vect(t_vect *vect);
double	ft_dot_product(t_vect *a, t_vect *b);
t_vect	ft_cross_product(t_vect *u, t_vect *v);
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
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
int	parse_ar(t_data *data, char **raw_split);
int	parse_cu(t_data *data, char **raw_split);
int	parse_dp(t_data *data, char **raw_split);
int	parse_xi(t_data *data, char **raw_split);
// 
int	parse_reste(t_data *data, char **raw_split, t_param *obj);
int	parse_bg_texture(t_data *data, char *path, t_img **bg_txt);
// 
int	parse_shininess(t_data *data, char *raw, t_param *obj);
int	parse_transparence(t_data *data, char *raw, t_param *obj);
int	parse_mirror(t_data *data, char *raw, t_param *obj);
int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
int	parse_amap(t_data *data, char *path, t_param *obj);
int	parse_color2(t_data *data, char *raw, t_param *obj);
/********************************
		Z
********************************/
void	end(t_data *data, int exit_code, int full_clean);
int		end2(t_data *data);


#endif
