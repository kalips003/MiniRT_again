/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c);
///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c)
{
	t_coor ambient;
	t_coor	diffuse;
	t_light	**lights;
	double adjusted_intensity;
	
	ambient = ft_ambient(data, c);
	memset(&diffuse, 0 , sizeof(t_coor));

	lights = data->light_source - 1;
	while (++lights && *lights)
	{
		c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;
	}

	t_rgb	reflected = what_is_reflected(data, c);
	double mirror_scalar = ((t_sphere*)c->object)->mirror;

	// c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	// c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	// c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

	c->px_color.r = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.x + diffuse.x) + mirror_scalar * reflected.r)));
	c->px_color.g = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.y + diffuse.y) + mirror_scalar * reflected.g)));
	c->px_color.b = fmax(0, fmin(255, round((1 - mirror_scalar) * (ambient.z + diffuse.z) + mirror_scalar * reflected.b)));
}


// block try: single void**

#define CIRCLE 0
#define SPHERE 1
#define PLANE 2
#define CYLINDER 3
#define CONE 4

typedef void* (*t_in_shadow_of)(t_calcul_px*, void*);
t_in_shadow_of g_ft_in_shadow_of[] = {
	in_shadow_of_sphere_void,
	in_shadow_of_plane_void,
	in_shadow_of_cicle_void,
	in_shadow_of_cylinder_void,
	in_shadow_of_cone_void
};

typedef double (*t_dist_of)(t_calcul_px *calcul, void *obj);
t_dist_of g_ft_dist_of[] = {
	in_shadow_of_sphere_void,
	in_shadow_of_plane_void,
	in_shadow_of_cicle_void,
	in_shadow_of_cylinder_void,
	in_shadow_of_cone_void
};

int	in_shadow_of_sphere_void(t_calcul_px *calcul, void *sphere);
int	in_shadow_of_plane_void(t_calcul_px *calcul, void *p);
int	in_shadow_of_cicle_void(t_calcul_px *calcul, void *circle);
int	in_shadow_of_cylinder_void(t_calcul_px *calcul, void *cy);
int	in_shadow_of_cone_void(t_calcul_px *calcul, void *cone);

// {
// 	while (i < )
// 		if (g_f_in_shadow_of[obj->type](t_data, t_calcul_px) == 1)
// };


int	ft_find_pixel_colision_v2(t_data *data, t_calcul_px *c)
{
	void	**obj_ptr;

	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		
	}
}



typedef int (*t_ft_parse_object)(t_data *data, char **split);

typedef struct s_parsing_dico_pair
{
	char		*name;
	t_ft_parse_object	exe;
}	t_dico_pair;

static const t_dico_pair	dico[] = {
	{"A", parse_A},
	{"L", parse_L},
	{"C", parse_C},
	{"pl", parse_pl},
	{"sp", parse_sp},
	{"cy", parse_cy},
	{"co", parse_co},
	{NULL, NULL}
};

typedef double (*t_dist_of)(t_calcul_px *calcul, void *obj);

typedef struct s_ft_obj
{
	char		*name;
	t_dist_of	ft_dist_of;
}	t_ft_obj;


static const t_ft_obj	dico_ft[] = {
	{"Circle", parse_pl},
	{"Plane", parse_pl},
	{"Sphere", parse_sp},
	{"Cylinder", parse_cy},
	{"Cone", parse_co},
	{NULL, NULL}
};






///////////////////////////////////////////////////////////////////////////////]
// takes an obj with a view vector filled, create the vector space
// 
// Right = Camera x Y = {-Cz, 0, Cx}
// Up = Right x Camera = {-CxCy, Cx²+Cz², -CyCz}
void	create_vector_space(t_obj *obj)
{
// if camera vector is == Y vector
	if (fabs(obj->view.dx) < EPSILON && fabs(obj->view.dz) < EPSILON)
	{
		if (obj->view.dy > 0)
			*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}};
		else
			*obj = (t_obj){obj->c0, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}};
		return ;
	}
	obj->right.dx = -obj->view.dz;
	obj->right.dy = 0;
	obj->right.dz = obj->view.dx;
	ft_normalize_vect(&obj->right);
	obj->up.dx = -obj->view.dx * obj->view.dy;
	obj->up.dy = obj->view.dx * obj->view.dx + obj->view.dz * obj->view.dz;
	obj->up.dz = -obj->view.dy * obj->view.dz;
	ft_normalize_vect(&obj->up);
}

void	recalculate_cone(t_cone *cone)
{
	cone->apex = new_moved_point(&cone->O.c0, &cone->O.view, cone->height);
}


void	recalculate_cylinder(t_cylinder *cylinder)
{
	cylinder->xyz_other = new_moved_point(&cylinder->O.c0, &cylinder->O.view, cylinder->height);
}


void	recalculate_plane(t_plane *plane)
{
	plane->d = -(plane->O.view.dx * plane->O.c0.x + plane->O.view.dy * plane->O.c0.y + plane->O.view.dz * plane->O.c0.z);
}
