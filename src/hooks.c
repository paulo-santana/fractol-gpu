#include "fractol.h"
#include <stdlib.h>
#include <stdio.h>
#include "../minilibx-linux/mlx.h"

#define ESC 0xff1b

int key_handler(int key, t_data *data)
{
	printf("key: %x\n", key);
	if (key == 'q' || key == ESC)
		exit(0);
	if (key == 'a')
		mlx_put_image_to_window(data->mlx, data->window, data->img.ptr, 0, 0);
}
