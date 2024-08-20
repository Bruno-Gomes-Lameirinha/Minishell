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
} State;

void	ft_tokenize(char *input);
int		ft_is_space(char c);

#endif
