# ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <termios.h>

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_token
{
	char			*token_node;
	struct s_token	*next;
}	t_token;

typedef enum { 
	TOKEN_STATE_START,
	TOKEN_STATE_COMMAND,
	TOKEN_STATE_OPERATOR,
	TOKEN_STATE_END,
} State;

t_token		*ft_list_new_token(void);
int			ft_is_space(char c);
void		ft_add_token(t_token **lexeme, char *node);
void		ft_print_linked_list(t_token **lexeme);
void		ft_tokenize(char *input, t_token **lexeme);
void		ft_state_start(char **input, State *state, char **current_token);
void		ft_state_command(char **input, State *state, char **current_token);
void		ft_last_token(char *current_token, t_token **lexeme);
void		ft_clean_token_list(t_token **lst);

#endif
