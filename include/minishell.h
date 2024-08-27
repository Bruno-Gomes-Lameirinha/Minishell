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
	int				type_token;
	struct s_token	*next;
}	t_token;



typedef enum { 
	TOKEN_STATE_START,
	TOKEN_STATE_COMMAND,
	TOKEN_STATE_OPERATOR,
	TOKEN_STATE_END,
} State;

enum e_token
{
    WORD,
    PIPE,// |
    REDIR_IN, // <
    REDIR_HDOC, // <<
    REDIR_OUT, // >
    REDIR_OUTAPP, //>>
    OR,// ||
    AND,// &&
    SUBSHELL, // ()
    ARCHIVE, 
    DOUBLE_QUOTES, // ""
    SINGLE_QUOTES // ''
    //$?
}; 

t_token		*ft_list_new_token(void);
int			ft_is_space(char c);
void		ft_add_token(t_token **lexeme, char *node, int type);
void		ft_print_linked_list(t_token **lexeme);
void		ft_tokenize(char *input, t_token **lexeme);
void		ft_state_start(char **input, State *state, char **index_token, int *type);
void		ft_state_command(char **input, State *state, char **index_token, int *type);
void		ft_handle_quotes(char ***input, char ***index_token, int **type);
void		ft_last_token(char *current_token, t_token **lexeme, int *type);
void		ft_clean_token_list(t_token **lst);
void	    ft_handle_operators(char ***input, char ***i_token, int **type);
void	    ft_echo_command(t_token **cmd_tokens);
void	    ft_cd_command(t_token **cmd_tokens);

#endif
