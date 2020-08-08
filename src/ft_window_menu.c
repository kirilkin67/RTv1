#include "rtv1.h"

static void	ft_drawing_fon(t_rtv *p)
{
	int	x;
	int	y;

	y = 0;
	while (y < p->height)
	{
		x = 0;
		while (x < 400)
		{
			mlx_pixel_put(p->mlx_ptr, p->str_ptr, x, y, COLOR_BG);
			x += 1;
		}
		y += 1;
	}
	// y = 0;
	// while (y < p->height)
	// {
	// 	x = 0;
	// 	while (x < 400)
	// 		mlx_pixel_put(p->mlx_ptr, p->str_ptr, x++, y, COLOR_BG);
	// 	y += 1;
	// }
}

static void	print_navigation_menu(t_rtv *p)
{
	char	*str;
	char	*coord;

	coord = ft_itoa(p->camera->dir.z);
	str = ft_strjoin(STR2, coord);
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 20, COLOR_STR, str);
	free(coord);
	free(str);
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 40, COLOR_STR, STR1);
	coord = ft_itoa(p->camera->start.x);
	str = ft_strjoin("X = ", coord);
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 60, COLOR_STR, str);
	free(coord);
	free(str);
	coord = ft_itoa(p->camera->start.y);
	str = ft_strjoin("Y = ", coord);
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 80, COLOR_STR, str);
	free(coord);
	free(str);
	coord = ft_itoa(p->camera->start.z);
	str = ft_strjoin("Z = ", coord);
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 100, COLOR_STR, str);
	free(coord);
	free(str);
}

static void	print_instructions(t_rtv *p)
{
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 170, COLOR_STR, "Q,E - move Y");
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 190, COLOR_STR, "A,D - move X");
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 210, COLOR_STR, "W,S - move Z");
	mlx_string_put(p->mlx_ptr, p->str_ptr, 20, 230, COLOR_STR, "Arrows - up, down, right, left");
}

void		ft_drawing_menu(t_rtv *p)
{
	ft_drawing_fon(p);
	print_navigation_menu(p);
	print_instructions(p);
}

void		ft_window_menu(t_rtv *p)
{
	if (p->window_menu == CLOSED)
	{
		p->str_ptr = mlx_new_window(p->mlx_ptr, 400, p->height, "MENU");
		ft_drawing_menu(p);
		// ft_drawing_fon(p);
		// print_navigation_menu(p);
		// print_instructions(p);
		p->window_menu = OPEN;
		mlx_hook(p->str_ptr, 2, (1L << 0), key_press, p);
	}
	else if (p->window_menu == OPEN)
	{
		p->window_menu = CLOSED;
		mlx_destroy_window(p->mlx_ptr, p->str_ptr);
	}
}