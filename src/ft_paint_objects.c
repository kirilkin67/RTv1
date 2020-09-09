#include "rtv1.h"

int		ft_intersect_obj(t_rtv *p, t_vector *ray, t_vector *start, double *min_dist)
{
	t_object	tmp;
	int			id;
	int			n;
	double		len_dist;

	id = -1;
	*min_dist = INT_MAX;
	n = 0;
	while (n < p->num)
	{
		tmp = *p->object[n];
		if (start != NULL)
			object_data(&tmp, start);
		len_dist = ft_raytrace_objects(ray, &tmp);
		if (len_dist != -1 && len_dist > 0.001 && len_dist < *min_dist)
		{
			*min_dist = len_dist;
			id = n;
		}
		n += 1;
	}
	return (id);
}

int	*reflection_transparency(t_rtv *p, t_vector *intersect, t_vector *norm, int *id)
{
	// int		reflect_color;
	double	reflection;
	int		id_r;
	// int		refract_color;
	double	refraction;
	static int		color[2];


	id_r = *id;
	color[0] = 0;
	color[1] = 0;
	// reflect_color = 0;
	reflection = p->object[id_r]->reflection;
	if (reflection > 0)
		color[0] = ft_calculate_reflection(p, intersect, norm, id);
	// color[0] = reflect_color;
	// refract_color = 0;
	refraction = p->object[id_r]->refraction;
	if (refraction > 0)
		color[1] = ft_calculate_refraction(p, intersect, norm, &id_r);
	// color[1] = refract_color;
	return color;
}

int		ft_light_object(t_rtv *p, t_vector *ray, int *id, double *min_dist)
{
	t_vector	intersect;
	t_vector	norm;
	int			local_color;
	int			reflect_color;
	double		reflection;
	double		refraction;

	*id = ft_intersect_obj(p, ray, NULL, min_dist);
	if (*id == NO_INTERSECT)
		return (COLOR_BG1);
	intersect = ft_multiply_vector_num(ray, *min_dist);
	norm = ft_calculate_vector_norm(p->object[*id], &intersect);
	local_color = ft_calculate_lighting(p, &intersect, &norm, *id);

	// int id_r = *id;
	
	reflect_color = 0;
	reflection = p->object[*id]->reflection;
	refraction = p->object[*id]->refraction;

	// if (reflection > 0)
	// 	reflect_color = ft_calculate_reflection(p, &intersect, &norm, id);
	// local_color = reflection_color(local_color, reflect_color, reflection);

	// int refract_color = 0;
	// refraction = p->object[id_r]->refraction;
	// if (refraction > 0)
	// 	refract_color = ft_calculate_refraction(p, &intersect, &norm, &id_r);
	// local_color = reflection_color(local_color, refract_color, refraction);

	int *color = reflection_transparency(p, &intersect, &norm, id);
	local_color = reflection_color(local_color, color[0], reflection);
	local_color = reflection_color(local_color, color[1], refraction);
	return (local_color);
}

void	*thread_paint_object(void *param)
{
	t_data		*data;
	t_vector	ray;
	int			color;
	int			id;
	double		min_dist;

	data = (t_data *)param;
	while (data->y_start < data->y_end)
	{
		data->x = 0;
		while (data->x < data->width)
		{
			data->camera.dir.x = (double)data->x - (double)data->x0;
			data->camera.dir.y = (double)data->y0 - (double)data->y_start;
			ray = data->camera.dir;
			ray = ft_rotation_vector(&data->all->camera->angle, &ray);
			ft_unit_vector(&ray);
			color = ft_light_object(data->all, &ray, &id, &min_dist);
			data->all->draw[data->x + data->y_start * data->width] = color;
			data->x += 1;
		}
		data->y_start += 1;
	}
	return (NULL);
}

void	ft_multi_thread_paint(t_rtv *paint)
{
	pthread_t	id[NUM_THREAD];
	t_data		data[NUM_THREAD];
	size_t		n;

	n = 0;
	while (n < NUM_THREAD)
	{
		data[n].all = paint;
		data[n].camera.dir.z = paint->fov;
		data[n].y_start = n * paint->height / NUM_THREAD;
		data[n].y_end = (n + 1) * paint->height / NUM_THREAD;
		data[n].x0 = (paint->width - 1) / 2.0;
		data[n].y0 = (paint->height - 1) / 2.0;
		data[n].width = paint->width;
		pthread_create(&id[n], NULL, thread_paint_object, &data[n]);
		n += 1;
	}
	n = 0;
	while (n < NUM_THREAD)
	{
		pthread_join(id[n], NULL);
		n += 1;
	}
}

// void		ft_paint_object(t_rtv *p)
// {
// 	int			x;
// 	int			y;
// 	int			pixel_color;
// 	t_vector	ray;

// 	y = 0;
// 	while (y < HIGHT)
// 	{
// 		x = 0;
// 		while (x < WIDHT)
// 		{
// 			p->camera->dir.x = (float)(x - p->x0);
// 			p->camera->dir.y = (float)(p->y0 - y);
// 			// ray = p->camera->dir;
// 			// ft_rotat_vector(&p->camera->angle, &ray);
// 			ray = ft_rotation_vector(&p->camera->angle, &p->camera->dir);
// 			ft_unit_vector(&ray);
// 			pixel_color = ft_light_object(p, &ray);
// 			p->draw[x + y * WIDHT] = pixel_color;
// 			x += 1;
// 		}
// 		y += 1;
// 	}
// }