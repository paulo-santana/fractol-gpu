#include "../minilibx-linux/mlx.h"
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "fractol.h"

void new_image(t_image *save_to, int width, int height, t_data *data)
{
	void *mlx;
	t_image *image;

	mlx = data->mlx;
	image = &data->img;
	image->ptr = mlx_new_image(mlx, width, height);
	image->data = (int *)mlx_get_data_addr(
			image->ptr,
			&image->bpp,
			&image->line_size,
			&image->endian);
	image->width = width;
	image->height = height;
}

void generate_colors(t_data *fractol) {
	int iter = 0;
	float factor = 0;
	int r = 0;
	int g = 0;
	int b = 0;

	while (iter < fractol->max_iter) {
		factor = (float)iter / fractol->max_iter;
		float diff = 1. - factor;
		r = 9 * diff * pow(factor, 3) * 255;
		g = 15 * pow(diff, 2) * pow(factor, 2) * 255;
		b = 8.5 * pow(diff, 3) * factor * 255;
		fractol->colors[iter] = (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff);
		iter++;
	}
}

void init_data(t_data *data)
{
    data->mlx = mlx_init();
	data->window = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "fractol gpu");
	data->win_width = WIN_WIDTH;
	data->win_height = WIN_HEIGHT;
	data->offset_x = -2;
	data->offset_y = -1;
	data->scale = 300;
	data->max_iter = 80;
	generate_colors(data);
	new_image(&data->img, WIN_WIDTH, WIN_HEIGHT, data);
}

size_t micros(void)
{
	struct timeval current;

	gettimeofday(&current, NULL);
	return (current.tv_sec * 1000000 + round(current.tv_usec));
}

void printfps(t_data *data)
{
	size_t current;
	double fps;
	double delta_time;
	static size_t last_tick;
	char buff[200];

	current = micros();
	delta_time = ((double) current - last_tick) / 1000000.;
	fps = 1 / delta_time;
	snprintf(buff, 200, "fps: %f", fps);
	last_tick = current;
	mlx_string_put(data->mlx, data->window, 10, 10, 0xffffff, buff);
}

int update(t_data *data)
{
	mandelbrot(data);
	mlx_put_image_to_window(data->mlx, data->window, data->img.ptr, 0, 0);
	printfps(data);
	return (0);
}

int main(void)
{
    t_data data;

	init_data(&data);
	mlx_key_hook(data.window, key_handler, &data);
	mlx_loop_hook(data.mlx, update, &data);
	mlx_loop(data.mlx);
}
