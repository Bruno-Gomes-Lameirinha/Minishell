NAME        := minishell
BONUS_NAME  := minishell_bonus
LIBFT       := libft/libft.a
LIBFT_PATH  := ./libft
SRC_PATH    := ./src
BONUS_PATH  := ./src_bonus
CFLAGS      := -Wall -Wextra -Werror -g3
OBJECTS_PATH := ./obj
HEADERS     := -I $(LIBFT_PATH) -I ./include
LDFLAGS     := -lreadline

SRC_MINISHELL = 
            
SRC_MINISHELL_MAIN = $(SRC_PATH)/minishell.c built_in.c tokenize.c pwd.c

SRC_MINISHELL_BONUS = $(BONUS_PATH)/minishell_bonus.c 

MINISHELL_OBJS := $(addprefix $(OBJECTS_PATH)/, $(notdir $(SRC_MINISHELL:.c=.o)))
MINISHELL_MAIN_OBJ := $(addprefix $(OBJECTS_PATH)/, $(notdir $(SRC_MINISHELL_MAIN:.c=.o)))
MINISHELL_BONUS_OBJS := $(addprefix $(OBJECTS_PATH)/, $(notdir $(SRC_MINISHELL_BONUS:.c=.o))) $(MINISHELL_OBJS)

all: $(LIBFT) $(NAME)

bonus: $(LIBFT) $(BONUS_NAME)

$(LIBFT):
	@echo "Making Libft..."
	@make -C libft --no-print-directory

$(OBJECTS_PATH):
	@mkdir -p $(OBJECTS_PATH)

$(OBJECTS_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJECTS_PATH)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(OBJECTS_PATH)/%.o: $(BONUS_PATH)/%.c | $(OBJECTS_PATH)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(MINISHELL_OBJS) $(MINISHELL_MAIN_OBJ) $(LIBFT)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) -o $@ $(MINISHELL_OBJS) $(MINISHELL_MAIN_OBJ) -L$(LIBFT_PATH) -lft $(LDFLAGS)

$(BONUS_NAME): $(MINISHELL_BONUS_OBJS) $(LIBFT)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) -o $@ $(MINISHELL_BONUS_OBJS) -L$(LIBFT_PATH) -lft

clean:
	@echo "Removing object files..."
	@rm -f $(LIBFT_PATH)/*.o 
	@rm -f $(OBJECTS_PATH)/*.o
	@rm -rf $(OBJECTS_PATH)

fclean: clean
	@echo "Removing executables and libraries..."
	@rm -f $(NAME) $(BONUS_NAME) $(LIBFT)

re: fclean all

re_bonus: fclean all bonus

.PHONY: all clean fclean re bonus
