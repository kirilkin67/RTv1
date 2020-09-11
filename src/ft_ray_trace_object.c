#include "rtv1.h"

double		ft_intersect_ray_sphere(t_vector *ray, t_object *s)
{
	double	proection_ray;
	double	len_dir;
	double	len_dist;

	proection_ray = ft_vector_projection_on_ray(&s->pos, ray);
	len_dir = s->radius * s->radius - \
			(s->len_pos - proection_ray * proection_ray);
	if (len_dir < 0.01f)
		return (NO_INTERSECT);
	if (sqrt(s->len_pos) > s->radius)
		len_dist = proection_ray - sqrt(len_dir);
	else
		len_dist = proection_ray + sqrt(len_dir);
	return (len_dist);
}

double		ft_intersect_ray_plane(t_vector *ray, t_object *plane)
{
	double		angele;
	double		len_dist;

	angele = ft_vector_scalar(&plane->norm_p, ray);
	if (-angele <= 0.001f)
		return (NO_INTERSECT);
	len_dist = ft_vector_scalar(&plane->pos, &plane->norm_p) / angele;
	return (len_dist);
}

/*
**Check if the ray and cilinder intersect
** FIST
** V_1 = (Ray - ft_vector_scalar(Ray, Nor) * Nor)
** V_1 = ft_multiply_vector_num(Nor, ft_vector_scalar(Ray, Nor))
** V_1 = ft_subtraction_vector(Ray, V_1)
** A = ft_vector_scalar(V_1, V_1)
** V_2 = ft_vector_scalar(Pos, Nor) * Nor) - Pos
** V_2 = ft_multiply_vector_num(Nor, ft_vector_scalar(Pos, Nor))
** V_2 = ft_subtraction_vector(V_2, Pos)
** B = 2 * (Ray - ft_vector_scalar(Ray, Nor) * Nor) * -1 * (Vector_Pos * V_2)
** B = 2 * ft_vector_scalar(V_1, V_2)
** C = ft_vector_scalar(V_2, V_2) - Radius^2
** SECOND
** A = Ray|Ray - (Ray|Nor_p)^2
** B = -2 * (Ray|Pos - (Ray|Nor_p)*(Pos|Nor_p))
** C = Pos|Pos - (Pos|Nor_p)^2 - Radius * Radius
** double discr = B * B - 4 * A * C;
** void		ft_solve_discriminant(t_discr *discr)
** {
** 	discr->discr = discr->b * discr->b - 4 * discr->a * discr->c;
** }
*/

double		ft_intersect_ray_cylinder(t_vector *ray, t_object *cil)
{
	t_vector	v1;
	t_discr		cilindr;
	double		len_dist;

	v1 = ft_multiply_vector_num(&cil->norm_p,\
								ft_vector_scalar(ray, &cil->norm_p));
	v1 = ft_sub_vectors(ray, &v1);
	cilindr.a = ft_vector_scalar(&v1, &v1);
	cilindr.b = 2 * ft_vector_scalar(&v1, &cil->discr.v2);
	cilindr.c = cil->discr.c;
	len_dist = ft_solve_quadratic_equation(&cilindr);
	return (len_dist);
}

double		ft_intersect_ray_cone(t_vector *ray, t_object *cone)
{
	t_discr	conus;
	double	ray_norm;
	double	ray_ray;
	double	ray_pos;
	double	len_dist;

	ray_ray = ft_vector_scalar(ray, ray);
	ray_norm = ft_vector_scalar(ray, &cone->norm_p);
	ray_pos = ft_vector_scalar(ray, &cone->pos);
	conus.a = ray_ray - cone->discr.k_tan * (ray_norm * ray_norm);
	conus.b = 2 * (cone->discr.k_tan * ray_norm * cone->discr.pos_n_p \
							- ray_pos);
	conus.c = cone->discr.c;
	len_dist = ft_solve_quadratic_equation(&conus);
	return (len_dist);
}

double	ft_raytrace_objects(t_vector *ray, t_object *obj)
{
	double		len_dist;

	len_dist = -1;
	if (obj->id == e_sphere)
		len_dist = ft_intersect_ray_sphere(ray, obj);
	if (obj->id == e_plane) 
		len_dist = ft_intersect_ray_plane(ray, obj);
	if (obj->id == e_cylindr)
		len_dist = ft_intersect_ray_cylinder(ray, obj);
	if (obj->id == e_cone)
		len_dist = ft_intersect_ray_cone(ray, obj);
	return (len_dist);
}
