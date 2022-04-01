NAME = fractol

MLX_DIR = ./minilibx-linux/
MLX = $(MLX_DIR)/libmlx.a

OBJ_DIR = ./obj

SRC = main.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC = nvcc
LFLAGS = -L $(MLX_DIR) -lmlx -lXext -lX11

all: $(NAME)

$(NAME): $(MLX) $(OBJ_DIR) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c $< -o $@

$(MLX):
	make -C $(MLX_DIR)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
