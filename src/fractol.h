#ifndef FRACTOL_H
# define FRACTOL_H

# include <sys/time.h>
# include <stddef.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>

# define WIN_WIDTH 1024
# define WIN_HEIGHT 768

typedef struct s_image t_image;
typedef struct s_data t_data;

struct s_image {
    void *ptr;
    int *data;
    int width;
    int height;
    int bpp;
    int line_size;
    int endian;
};

struct s_data
{
    void *mlx;
    void *window;
	int win_width;
	int win_height;
	int max_iter;
	int zoom_in;
	int zoom_out;
	unsigned int colors[1024];
	double scale;
	double c_real;
	double c_imag;
	double offset_x;
	double offset_y;
	size_t last_tick;
    t_image img;
};

int update(t_data *data);
int key_press_hook(int key, t_data *fractol);
int key_release_hook(int key, t_data *fractol);
int wheel_handler(int button, int x, int y, void *param);
size_t micros(void);
double get_delta_time(t_data *fractol);

void world_to_screen(t_data *fractol, long double world_x, long double world_y, int *screen_x, int *screen_y);
// void screen_to_world(t_data *fractol, int screen_x, int screen_y, long double *world_x, long double *world_y);

#ifdef __cplusplus
extern "C" {
#endif

	// mandelbrot.cu
	void mandelbrot(t_data *fractol);

#ifdef __cplusplus
}
#endif

#endif /* !FRACTOL_H */
