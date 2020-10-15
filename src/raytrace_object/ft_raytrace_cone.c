#include "rtv1.h"

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

double		ft_intersect_ray_empty_cone(t_vector *ray, t_object *cone)
{
	double	check;

	calculate_a_b_c_discr_cone(ray, cone);
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

t_cross		result_data_input(double distance, int check)
{
	t_cross	result;

	result.id = INTERSECT;
	result.len = distance;
	result.check = check;
	return (result);
}

// double		calculate_distance_to_cone_caps(t_object *object, t_vector *ray)
t_cross		calculate_distance_to_cone_caps(t_object *object, t_vector *ray)
// void		calculate_distance_to_cone_caps(t_object *object, t_vector *ray, t_cross *result)
{
	t_cross		result;
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
	distance = ft_vector_scalar(&position, &object->axis) / angle; // растояние до caps от камеры
	delta = ft_multiply_vector_num(ray, distance);
	delta= ft_sub_vectors(&delta, &position); // вектор от центра крышки до пересечения с ней
	result.id = NO_INTERSECT;
	// if (angle > 0 && ft_vector_scalar(&delta, &delta) <= (object->r_min * object->r_min))
	if (angle > 0 && object->r_min * object->r_min - ft_vector_scalar(&delta, &delta) > 0.001)
	{
		result = result_data_input(distance, e_caps);
		return (result);
	}
	// if (angle < 0 && ft_vector_scalar(&delta, &delta) <= (object->r_max * object->r_max))
	if (angle < 0 && object->r_max * object->r_max - ft_vector_scalar(&delta, &delta) > 0.001)
	{
		result = result_data_input(distance, e_caps);
		return (result);
	}
	return (result);
}

t_cross		ft_intersect_ray_cone(t_object *cone, t_vector *ray)
{
	t_cross	result;
	double	check;
	// double	len_caps;

	result.id = NO_INTERSECT;
	calculate_a_b_c_discr_cone(ray, cone);
	ft_solve_quadratic_equation(&cone->discr);
	// len_caps = calculate_distance_to_cone_caps(cone, ray);
	if (cone->discr.discr < 0)
		return (result);

	// calculate_distance_to_cone_caps(cone, ray, &result);
	result = calculate_distance_to_cone_caps(cone, ray);
	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_1);
	if (cone->min <= check && check <= cone->max)
	{
		// result.id = INTERSECT;
		// if (len_caps != NO_INTERSECT && len_caps <= cone->discr.d_1)
		if (result.id == INTERSECT && result.len <= cone->discr.d_1)
		{
			// result.len = len_caps;
			// result.check = e_caps;
			return (result);
			// return (result_data_input(len_caps, e_caps));
		}

		result.id = INTERSECT;
		result.len = cone->discr.d_1;
		result.check = e_body;
		return (result);
		// return (result_data_input(cone->discr.d_1, e_body));
	}
	check = check_intersect(ray, &cone->pos, &cone->axis, cone->discr.d_2);
	if (cone->min <= check && check <= cone->max)
	{
		// result.id = INTERSECT;
		// if (len_caps != NO_INTERSECT && len_caps <= cone->discr.d_2)
		if (result.id == INTERSECT && result.len <= cone->discr.d_2)
		{
			// result.len = len_caps;
			// result.check = e_caps;
			return (result);
			// return (result_data_input(len_caps, e_caps));
		}
		result.id = INTERSECT;
		result.len = cone->discr.d_2;
		result.check = e_body;
		return (result);
		// return (result_data_input(cone->discr.d_1, e_body));
	}
	// if (len_caps != NO_INTERSECT)
	// {
	// 	result.id = INTERSECT;
	// 	result.len = len_caps;
	// 	result.check = e_caps;
	// }
	return (result);
}

/*
// double		calculate_distance_to_cone_caps(t_object *object, t_vector *ray)
t_cross		calculate_distance_to_cone_caps(t_object *object, t_vector *ray)
// void		calculate_distance_to_cone_caps(t_object *object, t_vector *ray, t_cross *result)
{
	t_cross		result;
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
	
	result.id = NO_INTERSECT;
	if (angle > 0 && ft_vector_scalar(&delta, &delta) <= (object->r_min * object->r_min))
	{
		result = result_data_input(distance, e_caps);
		return (result);

		// result->id = INTERSECT;
		// result->len = distance;
		// result->check = e_caps;

		// return (distance);
	}
	if (angle < 0 && ft_vector_scalar(&delta, &delta) <= (object->r_max * object->r_max))
	{
		result = result_data_input(distance, e_caps);
		return (result);

		// result->id = INTERSECT;
		// result->len = distance;
		// result->check = e_caps;

		// return (distance);
	}

	return (result);
	// return (NO_INTERSECT);
} */