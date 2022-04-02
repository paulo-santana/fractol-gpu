NAME = fractol

MLX_DIR = ./minilibx-linux/
MLX = $(MLX_DIR)/libmlx.a

OBJ_DIR = ./obj

SRC = main.c
SRC += hooks.c

CUDA_SRC = mandelbrot.cu

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJ += $(addprefix $(OBJ_DIR)/, $(CUDA_SRC:.cu=.o))

CC = nvcc
CFLAGS = -g
LFLAGS = -L $(MLX_DIR) -lmlx -lXext -lX11 -lm

RM = rm -rf

all: $(NAME)

$(NAME): $(MLX) $(OBJ_DIR) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cu
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MLX):
	make -C $(MLX_DIR)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)
