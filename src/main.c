#include "../minilibx-linux/mlx.h"
#include "fractol.h"

double get_delta_time(t_data *fractol)
{
	double delta;

	if (fractol->last_tick == 0)
		fractol->last_tick = micros();

	size_t current = micros();

	delta = ((double)current - fractol->last_tick) / 1000000.;
	fractol->last_tick = current;
	return (delta);
}

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
	data->c_real = 0;
	data->c_imag = 0;
	data->scale = 300;
	data->zoom_in = 0;
	data->zoom_out = 0;
	data->max_iter = 180;
	data->last_tick = micros();
	generate_colors(data);
	new_image(&data->img, WIN_WIDTH, WIN_HEIGHT, data);
}

size_t micros(void)
{
	struct timeval current;

	gettimeofday(&current, NULL);
	return (current.tv_sec * 1000000 + current.tv_usec);
}

int main(void)
{
    t_data data;

	init_data(&data);
	get_delta_time(&data);
	mlx_mouse_hook(data.window, wheel_handler, &data);
	mlx_loop_hook(data.mlx, update, &data);
	mlx_hook(data.window, 2, 1, key_press_hook, &data);
	mlx_hook(data.window, 3, 2, key_release_hook, &data);
	mlx_loop(data.mlx);
}
