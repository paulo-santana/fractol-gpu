#include "../minilibx-linux/mlx.h"

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
    t_image *img;
};

void mandelbrot(t_data *data)
{

}

int main(void)
{
    t_data data;

    data.mlx = mlx_init();
	data.window = mlx_new_window(data.mlx, WIN_WIDTH, WIN_HEIGHT, "fractol gpu");
	data.win_width = WIN_WIDTH;
	data.win_height = WIN_HEIGHT;
	data.img->ptr = mlx_new_image(data.mlx, WIN_WIDTH, WIN_HEIGHT);
	data.img->data = (int *)mlx_get_data_addr(
			data.img->ptr,
			&data.img->bpp,
			&data.img->line_size,
			&data.img->endian);
}
