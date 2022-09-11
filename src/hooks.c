#include "fractol.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../minilibx-linux/mlx.h"

#define ESC 0xff1b

static void screen_to_world(
		t_data *fractol,
		int screen_x,
		int screen_y,
		long double *world_x,
		long double *world_y
		)
{
	*world_x = (screen_x / fractol->scale) + fractol->offset_x;
	*world_y = (screen_y / fractol->scale) + fractol->offset_y;
}

void apply_zoom(t_data *fractol, double factor) {
	int x, y;

	long double mouse_world_x_before_zoom;
	long double mouse_world_y_before_zoom;
	long double mouse_world_x_after_zoom;
	long double mouse_world_y_after_zoom;

	mlx_mouse_get_pos(fractol->mlx, fractol->window, &x, &y);
	screen_to_world(fractol, x, y, &mouse_world_x_before_zoom, &mouse_world_y_before_zoom);
	fractol->scale *= factor;
	if (fractol->scale < 250)
		fractol->scale = 250;
	screen_to_world(fractol, x, y, &mouse_world_x_after_zoom, &mouse_world_y_after_zoom);
	fractol->offset_x += mouse_world_x_before_zoom - mouse_world_x_after_zoom;
	fractol->offset_y += mouse_world_y_before_zoom - mouse_world_y_after_zoom;
}

int wheel_handler(int button, int x, int y, void *param)
{
	t_data *fractol;

	(void)x;
	(void)y;
	fractol = param;
	if (button == 5)
		apply_zoom(fractol, 0.9);
	else if (button == 4)
		apply_zoom(fractol, 1.1);
	else
		printf("button: %d\n", button);
	return (0);
}

void keyboard_zoom(t_data *fractol, float factor)
{
	double delta_factor;
	double delta_time;

	delta_time = get_delta_time(fractol);
	delta_factor = factor * delta_time;
	apply_zoom(fractol, delta_factor);
}

int key_release_hook(int key, t_data *fractol)
{
	if (key == 'j')
		fractol->zoom_in = 0;
	else if (key == 'k')
		fractol->zoom_out = 0;
	return 0;
}

int key_press_hook(int key, t_data *fractol)
{
	if (key == 'j')
		fractol->zoom_in = 1;
	else if (key == 'k')
		fractol->zoom_out = 1;
	else if (key == 'q')
		exit(0);
	return 0;
}

#define BUFF_SIZE 300

void printfps(t_data *data)
{
	size_t current;
	double fps;
	double delta_time;
	static size_t last_tick;
	char buff[BUFF_SIZE];

	delta_time = get_delta_time(data);
	fps = 1 / delta_time;
	snprintf(buff, BUFF_SIZE, "fps: %f - zoom: %f", fps, data->scale);
	last_tick = current;
	mlx_string_put(data->mlx, data->window, 10, 10, 0xffffff, buff);
}

void update_mouse_coords(t_data *data)
{
	int x, y;
	long double wx, wy;

	mlx_mouse_get_pos(data->mlx, data->window, &x, &y);

	screen_to_world(data, x, y, &wx, &wy);
	data->c_real = wx;
	data->c_imag = wy;
}

int update(t_data *data)
{
	double factor;

	factor = 0;
	if (data->zoom_in)
		factor = -100;
	else if (data->zoom_out)
		factor = 100;
	factor = factor * get_delta_time(data);
	update_mouse_coords(data);
	apply_zoom(data, 1 + factor);
	mandelbrot(data);
	mlx_put_image_to_window(data->mlx, data->window, data->img.ptr, 0, 0);
	printfps(data);
	return (0);
}
