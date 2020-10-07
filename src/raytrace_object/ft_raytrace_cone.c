#include "rtv1.h"

double		ft_intersect_ray_empty_cone(t_vector *ray, t_object *cone)
{
	double	ray_norm;
	double	ray_ray;
	double	ray_pos;
	double	check;

	ray_ray = ft_vector_scalar(ray, ray);
	ray_norm = ft_vector_scalar(ray, &cone->axis);
	ray_pos = ft_vector_scalar(ray, &cone->pos);
	cone->discr.a = ray_ray - cone->discr.k_tan * (ray_norm * ray_norm);
	cone->discr.b = 2 * (cone->discr.k_tan * ray_norm * cone->discr.pos_n_p - ray_pos);
	cone->discr.c = cone->discr.c;
	ft_solve_quadratic_equation(&cone->discr);
	if (cone->discr.discr < 0)
		return (NO_INTERSECT);
	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_1);
	if (cone->min <= check && check <= cone->max)
		return (cone->discr.d_1);
	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_2);
	if (cone->min <= check && check <= cone->max)
		return (cone->discr.d_2);
	return (NO_INTERSECT);
}

double		calculate_distance_to_cone_caps(t_vector *ray, t_object *object)
{
	t_vector	position;
	t_vector	delta;
	double		angle;
	double		distance;

	angle = ft_vector_scalar(ray, &object->axis);
	if (angle > 0)
		position = ft_multiply_vector_num(&object->axis, object->min);
	else if (angle < 0)
		position = ft_multiply_vector_num(&object->axis, object->max);
	position = ft_add_vectors(&object->pos, &position);
	// растояние до плоскости крышки от камеры
	distance = ft_vector_scalar(&position, &object->axis) / angle;
	// вектор от центра крышки до пересечения с ней
	delta = ft_multiply_vector_num(ray, distance);
	delta= ft_sub_vectors(&delta, &position);
	
	if (angle > 0 && ft_vector_scalar(&delta, &delta) > (object->r_min * object->r_min))
		return (NO_INTERSECT);
	if (angle < 0 && ft_vector_scalar(&delta, &delta) > (object->r_max * object->r_max))
		return (NO_INTERSECT);
	return (distance);
}

static void	calculate_a_b_c_discr_cone(t_vector *ray, t_object *cone)
{
	double	ray_ray;
	double	ray_norm;
	double	ray_pos;

	ray_ray = ft_vector_scalar(ray, ray);
	ray_norm = ft_vector_scalar(ray, &cone->axis);
	ray_pos = ft_vector_scalar(ray, &cone->pos);
	cone->discr.a = ray_ray - cone->discr.k_tan * (ray_norm * ray_norm);
	cone->discr.b = 2 * (cone->discr.k_tan * ray_norm * cone->discr.pos_n_p - ray_pos);
	cone->discr.c = cone->discr.c;
}

double		ft_intersect_ray_cone(t_vector *ray, t_object *cone)
{
	double	check;
	double	len_caps;

	calculate_a_b_c_discr_cone(ray, cone);
	ft_solve_quadratic_equation(&cone->discr);
	len_caps = calculate_distance_to_cone_caps(ray, cone);
	if (cone->discr.discr < 0)
		return (NO_INTERSECT);

	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_1);
	if (cone->min <= check && check <= cone->max)
	{
		if (len_caps != NO_INTERSECT && len_caps <= cone->discr.d_1)
			return (len_caps);
		return (cone->discr.d_1);
	}
	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_2);
	if (cone->min <= check && check <= cone->max)
	{
		if (len_caps != NO_INTERSECT && len_caps <= cone->discr.d_2)
			return (len_caps);
		return (cone->discr.d_2);
	}
	return (len_caps);
}