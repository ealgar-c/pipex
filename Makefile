CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -f

NAME = pipex
SRC_DIR = src/
SRC = pipex.c
OBJ_DIR = objs/
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

all: libft $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) libft/*.o
	@ echo " ...	Pipex compiled	..."

libft:
	@make -C libft/

clean:
	@$(RM) $(OBJ) $(BONUS_OBJ)
	@make -C libft/ clean
	@ echo " ...	clean finished	..."

fclean: clean
	@$(RM) $(NAME) $(BONUS_NAME)
	@make -C libft/ fclean

re: fclean all

.PHONY: all libft clean fclean re bonus