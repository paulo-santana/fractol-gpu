#ifndef FRACTOL_H
# define FRACTOL_H

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

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
	unsigned int colors[1024];
	double scale;
	double offset_x;
	double offset_y;
    t_image img;
};

int key_handler(int key, t_data *data);

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
