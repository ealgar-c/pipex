CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf

NAME = pipex
SRC_DIR = src/
SRC = pipex.c
OBJ_DIR = objs/
OBJ = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

NAME_B = pipex_bonus
SRC_DIR_B = src/bonus/
SRC_B = pipex_bonus.c utils_bonus.c
OBJ_B = $(addprefix $(OBJ_DIR), $(SRC_B:.c=.o))


all: libft $(NAME)

bonus: libft $(NAME_B)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) libft/*.o
	@echo " ... Pipex compiled ..."

$(NAME_B): $(OBJ_B)
	@$(CC) $(FLAGS) -o $(NAME_B) $(OBJ_B) libft/*.o
	@echo " ... Pipex bonus compiled ..."

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR_B)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -c $< -o $@

libft:
	@make -C libft/

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -C libft/ clean
	@echo " ... Clean finished ..."

fclean: clean
	@$(RM) $(NAME) $(NAME_B)
	@make -C libft/ fclean

re: fclean all

.PHONY: all bonus libft clean fclean re