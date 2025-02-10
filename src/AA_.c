#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]
// Normalize vector
int	ft_normalize_vect_v2(t_vect *vect, int print_err)
{
	double normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
	if (normal_magnitude == 0.0 && print_err)
		return (1);
		// return (put("VECTOR = [%f, %f, %f]\n", vect->dx, vect->dy, vect->dz), 1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// fills rtrnn with the coordonates of the rotated vector camera for [x,y]pixel
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
{
	t_camera *c = data->eye.c;
	t_camera_calc	calc;

	calc.cosA = cos(angle_α / 2);
	calc.sinA = sin(angle_α / 2);
	calc.cosB = cos(-angle_β / 2);
	calc.sinB = sin(-angle_β / 2);

	calc.Qw = calc.cosA*calc.cosB - calc.sinA*calc.sinB * (c->O.right.dx * c->O.up.dx + c->O.right.dy * c->O.up.dy + c->O.right.dz * c->O.up.dz);
	calc.Qi = calc.cosB*calc.sinA * c->O.up.dx + calc.cosA*calc.sinB * c->O.right.dx + calc.sinA*calc.sinB*(c->O.right.dy * c->O.up.dz - c->O.right.dz * c->O.up.dy);
	calc.Qj = calc.cosB*calc.sinA * c->O.up.dy + calc.cosA*calc.sinB * c->O.right.dy + calc.sinA*calc.sinB*(c->O.right.dz * c->O.up.dx - c->O.right.dx * c->O.up.dz);
	calc.Qk = calc.cosB*calc.sinA * c->O.up.dz + calc.cosA*calc.sinB * c->O.right.dz + calc.sinA*calc.sinB*(c->O.right.dx * c->O.up.dy - c->O.right.dy * c->O.up.dx);

	rtrn->dx = c->O.view.dx * (calc.Qw * calc.Qw + calc.Qi * calc.Qi - calc.Qj * calc.Qj - calc.Qk * calc.Qk) + 2*c->O.view.dy * (calc.Qi*calc.Qj - calc.Qw*calc.Qk) + 2*c->O.view.dz * (calc.Qi*calc.Qk + calc.Qw*calc.Qj);
	rtrn->dy = c->O.view.dy * (calc.Qw * calc.Qw + calc.Qj * calc.Qj - calc.Qi * calc.Qi - calc.Qk * calc.Qk) + 2*c->O.view.dz * (calc.Qj*calc.Qk - calc.Qw*calc.Qi) + 2*c->O.view.dx * (calc.Qi*calc.Qj + calc.Qw*calc.Qk);
	rtrn->dz = c->O.view.dz * (calc.Qw * calc.Qw + calc.Qk * calc.Qk - calc.Qi * calc.Qi - calc.Qj * calc.Qj) + 2*c->O.view.dx * (calc.Qi*calc.Qk - calc.Qw*calc.Qj) + 2*c->O.view.dy * (calc.Qj*calc.Qk + calc.Qw*calc.Qi);

	ft_normalize_vect(rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// fills rtrn with the coordonates of the rotated vector view object, around
// 		angle_α ~ UP then around angle_β ~ RIGHT
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion_better(t_obj *obj, double angle_α, double angle_β, t_vect *rotated)
{
	t_camera_calc	c;

	c.cosA = cos(angle_α / 2);
	c.sinA = sin(-angle_α / 2);
	c.cosB = cos(angle_β / 2);
	c.sinB = sin(-angle_β / 2);

	c.Qw = c.cosA*c.cosB - c.sinA*c.sinB * (obj->right.dx * obj->up.dx + obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
	c.Qi = c.cosB*c.sinA * obj->up.dx + c.cosA*c.sinB * obj->right.dx + c.sinA*c.sinB*(obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
	c.Qj = c.cosB*c.sinA * obj->up.dy + c.cosA*c.sinB * obj->right.dy + c.sinA*c.sinB*(obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
	c.Qk = c.cosB*c.sinA * obj->up.dz + c.cosA*c.sinB * obj->right.dz + c.sinA*c.sinB*(obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);

	rotated->dx = obj->view.dx * (c.Qw * c.Qw + c.Qi * c.Qi - c.Qj * c.Qj - c.Qk * c.Qk) + 2*obj->view.dy * (c.Qi*c.Qj - c.Qw*c.Qk) + 2*obj->view.dz * (c.Qi*c.Qk + c.Qw*c.Qj);
	rotated->dy = obj->view.dy * (c.Qw * c.Qw + c.Qj * c.Qj - c.Qi * c.Qi - c.Qk * c.Qk) + 2*obj->view.dz * (c.Qj*c.Qk - c.Qw*c.Qi) + 2*obj->view.dx * (c.Qi*c.Qj + c.Qw*c.Qk);
	rotated->dz = obj->view.dz * (c.Qw * c.Qw + c.Qk * c.Qk - c.Qi * c.Qi - c.Qj * c.Qj) + 2*obj->view.dx * (c.Qi*c.Qk - c.Qw*c.Qj) + 2*obj->view.dy * (c.Qj*c.Qk + c.Qw*c.Qi);

	ft_normalize_vect(rotated);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the camera (eye) vector of fixed angle (with vector operation)
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg)
{
	t_vect *c = &data->eye.c->O.view;
	
	double Vx2 = axis_rota->dx * axis_rota->dx;
	double Vy2 = axis_rota->dy * axis_rota->dy;
	double Vz2 = axis_rota->dz * axis_rota->dz;


	double resultx = COS_ROTA2 * c->dx + SIN_ROTA2 * (c->dx * (Vx2 - Vy2 - Vz2) + 2 * axis_rota->dx * (axis_rota->dy * c->dy + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dy * c->dz - axis_rota->dz * c->dy);
	double resulty = COS_ROTA2 * c->dy + SIN_ROTA2 * (c->dy * (Vy2 - Vx2 - Vz2) + 2 * axis_rota->dy * (axis_rota->dx * c->dx + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dz * c->dx - axis_rota->dx * c->dz);
	double resultz = COS_ROTA2 * c->dz + SIN_ROTA2 * (c->dz * (Vz2 - Vx2 - Vy2) + 2 * axis_rota->dz * (axis_rota->dx * c->dx + axis_rota->dy * c->dy)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dx * c->dy - axis_rota->dy * c->dx);
	c->dx = resultx;
	c->dy = resulty;
	c->dz = resultz;

	ft_normalize_vect(c);
	create_vector_space(&data->eye.c->O);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the obj (eye) vector of fixed angle (with vector operation)
void	rotation_obj_v2(t_obj *obj, t_vect *axis_rota, int posi_neg)
{
	t_vect	cross;
	double	dot;

	cross = ft_cross_product(axis_rota, &obj->view);
	dot = ft_dot_product(axis_rota, &obj->view);
	ft_normalize_vect(&cross);
	t_vect	new_view = {
		obj->view.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
		obj->view.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
		obj->view.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
	};
	ft_normalize_vect(&new_view);
	
	cross = ft_cross_product(axis_rota, &obj->up);
	dot = ft_dot_product(axis_rota, &obj->up);
	ft_normalize_vect(&cross);

	t_vect	new_up = {
		obj->up.dx * COS_ROTA + cross.dx * (posi_neg) * SIN_ROTA + axis_rota->dx * dot * (1 - COS_ROTA),
		obj->up.dy * COS_ROTA + cross.dy * (posi_neg) * SIN_ROTA + axis_rota->dy * dot * (1 - COS_ROTA),
		obj->up.dz * COS_ROTA + cross.dz * (posi_neg) * SIN_ROTA + axis_rota->dz * dot * (1 - COS_ROTA)
	};
	ft_normalize_vect(&new_up);
	
	cross = ft_cross_product(&new_up, &new_view);
	// cross = ft_vect_cross_product(&new_view, &new_up);
	ft_normalize_vect(&cross);
	// cross = (t_vect){-cross.dx, -cross.dy, -cross.dz};
	*obj = (t_obj){obj->c0, new_view, new_up, cross};
}