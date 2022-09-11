#include "fractol.h"
#include <stdio.h>

void world_to_screen(
		t_data *fractol,
		long double world_x,
		long double world_y,
		int *screen_x,
		int *screen_y
		)
{
	*screen_x = (world_x - fractol->offset_x) * fractol->scale;
	*screen_y = (world_y - fractol->offset_y) * fractol->scale;
}

__device__ static void screen_to_world(
		t_data *fractol,
		int screen_x,
		int screen_y,
		double *world_x,
		double *world_y
		)
{
	*world_x = (screen_x / fractol->scale) + fractol->offset_x;
	*world_y = (screen_y / fractol->scale) + fractol->offset_y;
}

__global__ void calculate_mandelbrot(int *pixels, t_data *fractol) {
    size_t x = threadIdx.x;
    size_t y = blockIdx.x;
    int i = 0;
    int max_iter = fractol->max_iter;
    double x2 = 0, y2 = 0;
    double real = 0, imag = 0;
    double c_real, c_imag;
    size_t position = x + y * fractol->img.width;

    screen_to_world(fractol, x, y, &c_real, &c_imag);
    while (x2 + y2 <= 4 && i < max_iter)
    {
        imag = (real + real) * imag + c_imag;
        real = x2 - y2 + c_real;
        x2 = real * real;
        y2 = imag * imag;
        i++;
    }
    if (i == max_iter)
        pixels[position] = 0;
    else
        pixels[position] = fractol->colors[i];
}

__global__ void calculate_julia(int *pixels, t_data *fractol) {
    double real = 0,
           imag = 0,
           x2 = 0,
           y2 = 0;
    size_t x = threadIdx.x;
    size_t y = blockIdx.x;
    int position = y * fractol->img.width + x;
    int i = 0;

    real = x / fractol->scale + fractol->offset_x;
    imag = y / fractol->scale + fractol->offset_y;
    x2 = real * real;
    y2 = imag * imag;
    while (x2 + y2 <= 4 && i < fractol->max_iter)
    {
        imag = (real + real) * imag + fractol->c_imag;
        real = x2 - y2 + fractol->c_real;
        x2 = real * real;
        y2 = imag * imag;
        i++;
    }
    if (i == fractol->max_iter)
        pixels[position] = 0;
    else
        pixels[position] = fractol->colors[i];
}

extern "C"
void mandelbrot(t_data *fractol) {
    int *d_data;
    t_data *d_fractol;

    int size = fractol->img.width * fractol->img.height;
    int data_size = size * sizeof(int);
    cudaMalloc((void **)&d_data, data_size);
    cudaMalloc((void **)&d_fractol, sizeof(t_data));

    cudaMemcpy(d_fractol, fractol, sizeof(t_data), cudaMemcpyHostToDevice);

    /* calculate_mandelbrot<<<fractol->img.height, fractol->img.width>>>(d_data, d_fractol); */
    calculate_mandelbrot<<<fractol->img.height, fractol->img.width>>>(d_data, d_fractol);

    cudaMemcpy(fractol->img.data, d_data, data_size, cudaMemcpyDeviceToHost);

    cudaFree(d_data);
    cudaFree(d_fractol);
    cudaDeviceSynchronize();
}
