/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_object_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikhail <mikhail@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/17 23:58:08 by mikhail           #+#    #+#             */
/*   Updated: 2020/04/21 01:19:43 by mikhail          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_light	*list_create(t_light *light, char **tab)
{
	light = (t_light *)malloc(sizeof(t_light));
	if (light == NULL)
		ft_exit(ERR_CREAT_TO_ARR);
	if (ft_lentab(tab) != 5)
		ft_exit(ERR_OBJECT);
	init_coordinates(&light->pos, tab[1]);
	light->intensity = ft_atof(tab[2]);
	// light->color = ft_ahextocolor(tab[3]);
	init_color(&light->color,tab[3]);
	if (ft_strcmp(tab[4], "Point") == 0)
		light->tip = 'P';
	if (ft_strcmp(tab[4], "Direct") == 0)
		light->tip = 'D';
	light->next = NULL;
	return (light);
}

t_light			*init_light(t_light *light, char **tab)
{
	t_light *tmp;

	tmp = NULL;
	if (light == NULL)
		light = list_create(light, tab);
	else
	{
		tmp = list_create(tmp, tab);
		tmp->next = light;
		light = tmp;
	}
	return (light);
}

void	data_plane(t_object *object, t_vector *start)
{
	ft_unit_vector(&object->norm_p);
	object->pos_cam = ft_vector_scalar(&object->norm_p, &object->pos)\
							- ft_vector_scalar(&object->norm_p, start);
	// object->pos = ft_subtraction_vector(&object->pos, start);
}

void	object_data(t_object *object, t_vector *start)
{
	if (object->id == 'P')
		data_plane(object, start);
	object->pos = ft_subtraction_vector(&object->pos, start);
	if (object->id == 'S')
		object->len_pos = pow(object->pos.x, 2) + pow(object->pos.y, 2) +\
						pow(object->pos.z, 2);
	if (object->id == 'C')
	{
		ft_unit_vector(&object->norm_p);
		object->discr.v2 = ft_multiply_vector_num(&object->norm_p,\
						ft_vector_scalar(&object->pos, &object->norm_p));
		object->discr.v2 = ft_subtraction_vector(&object->discr.v2, &object->pos);
		object->discr.c = ft_vector_scalar( &object->discr.v2, &object->discr.v2)\
						- pow(object->radius, 2);
	}
	if (object->id == 'K')
	{
		ft_unit_vector(&object->norm_p);
		object->discr.k_tan = 1 + pow(tan(object->angle / 2), 2);
		object->discr.pos_n_p = ft_vector_scalar(&object->pos, &object->norm_p);
		object->discr.c = ft_vector_scalar(&object->pos, &object->pos) -\
						object->discr.k_tan * pow(object->discr.pos_n_p, 2);
	}
}

void	calculate_constant(t_rtv *p, t_vector *start)
{
	t_light *tmp;
	int n;

	tmp = p->light;
	while (tmp != NULL)
	{
		tmp->pos = ft_subtraction_vector(&tmp->pos, start);
		tmp = tmp->next;
	}
	n = 0;
	// while (p->object[n] != NULL)
	while (n < p->num)
	{
		object_data(p->object[n], start);
		n += 1;
	}
}
