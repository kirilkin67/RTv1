#include "rtv1.h"

float		ft_ray_trace_object(t_vector *ray, t_object *obj)
{
	float		len_dist;

		if (obj->id == 'S')
			len_dist = ft_intersect_ray_sphere(ray, obj);
		if (obj->id == 'P' && obj->pos_cam < 0.001)
			len_dist = ft_intersect_ray_plane(ray, obj);
		if (obj->id == 'C')
			len_dist = ft_intersect_ray_cilinder(ray, obj);
		if (obj->id == 'K')
			len_dist = ft_intersect_ray_cone(ray, obj);
		return (len_dist);
}

void	ft_intersection_object(t_vector *ray, t_object **obj, int *id, float *min_dist)
{
	int		n;
	float	len_dist;

	*id = -1;
	*min_dist = INT_MAX;
	n = 0;
	while (obj[n] != NULL)
	{
		len_dist = ft_ray_trace_object(ray, obj[n]);
		if (len_dist != -1 && len_dist > 0.001f && len_dist < *min_dist)
		{
			*min_dist = len_dist;
			*id = n;
		}
		n += 1;
	}
}

int		ft_light_object(t_rtv *p, t_vector *ray)
{
	t_vector	interset;
	t_vector	v_norm;
	float		len_ray;
	int			color;
	int			id;
	float		min_dist;

	ft_intersection_object(ray, p->object, &id, &min_dist);
	if (id == -1)
		return (0x0);
	interset = ft_multiply_vector_num(ray, min_dist);
	if (p->object[id]->id == 'S' || p->object[id]->id == 'C'|| p->object[id]->id == 'K')
	{
		p->object[id]->norm = ft_subtraction_vector(&interset, &p->object[id]->pos);
		if (p->object[id]->id == 'C'|| p->object[id]->id == 'K')
		{
			len_ray = ft_vector_projection_on_ray(&p->object[id]->norm, &p->object[id]->norm_p);
			if (p->object[id]->id == 'K')
				len_ray = len_ray / powf(cosf(0.5 * p->object[id]->angle), 2);
			v_norm = ft_multiply_vector_num(&p->object[id]->norm_p, len_ray);
			p->object[id]->norm = ft_subtraction_vector(&p->object[id]->norm, &v_norm);
		}
		ft_unit_vector(&p->object[id]->norm);
	}
	color = illuminat_point(p->light, p->object, &interset, id);
	return (color);
}

void	paint_object(t_rtv *p)
{
	int	x;
	int	y;
	int	pixel_color;
	t_vector	ray;

	y = 0;
	while (y < HIGHT)
	{
		x = 0;
		while (x < WIDHT)
		{
			p->camera->dir.x = (float)(x - p->x0);
			p->camera->dir.y = (float)(p->y0 - y);
			ray = p->camera->dir;
			ft_rotat_vector(&p->angle, &ray);
			// ray = ft_rotation_vector(&p->angle, &cam->dir);
			ft_unit_vector(&ray);
			pixel_color = ft_light_object(p, &ray);
			p->draw[x + y * WIDHT] = pixel_color;
			x += 1;
		}
		y += 1;
	}
}

void	ft_put_pixel(char *img_data, int x, int y, int color)
{
	((int*)img_data)[x + 1200 * y] = color;
}


void	*thread_paint_object(void *param)
{
	int			x;
	t_data		*data;
	t_vector	ray;

	data = (t_data *)param;
	while (data->y_start < data->y_end)
	{
		data->x = 0;
		while (data->x < WIDHT)
		{
			data->camera.dir.x = (data->x - WIDHT / 2);
			data->camera.dir.y = (HIGHT / 2 - data->y_start);
			// data->camera.dir.z = data->all->width;
			ray = data->camera.dir;
			ray = ft_rotation_vector(&data->all->angle, &ray);
			ft_unit_vector(&ray);
			data->color = ft_light_object(data->all, &ray);
			data->all->draw[data->x + data->y_start * WIDHT] = data->color;
			data->x += 1;
		}
		data->y_start += 1;
	}
	return (NULL);
}

void	ft_multi_thread_paint(t_rtv *p)
{
	pthread_t	id[NUM_THREAD];
	t_data		data[NUM_THREAD];
	size_t		n;

	n = 0;
	while (n < NUM_THREAD)
	{
		data[n].all = p;
		data[n].camera.dir.z = p->width;
		data[n].y_start = n * HIGHT / NUM_THREAD;
		data[n].y_end = (n + 1) * HIGHT / NUM_THREAD;
		pthread_create(&id[n], NULL, thread_paint_object, &data[n]);
		n += 1;
	}
	n = 0;
	while (n < NUM_THREAD)
	{
		pthread_join(id[n], NULL);
		n += 1;
	}
	// mlx_put_image_to_window(p->mlx_ptr, p->win_ptr, p->img_ptr, 0, 0);
}
