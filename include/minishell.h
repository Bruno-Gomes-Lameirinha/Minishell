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

typedef struct s_pipex
{
	int		channel[2];
	pid_t	first_child;
	pid_t	second_child;
	pid_t	third_child;
	char	**parent_argv;
	char	**parent_env;
	char	**argv_childs;
	char	**path;
}		t_pipex;

typedef struct s_redirection {
	int type_token;
	char *filename;
	int heredoc_fd;
	struct s_redirection *next;
} t_redirection;

typedef struct s_ast_node {
	int 	type;
	int		type_token;
	char 	*value;
	struct 	s_ast_node *left;
	struct 	s_ast_node *right;
	struct 	s_ast_node *next;
	pid_t	execve_child;
	t_redirection *redirections;
} t_ast_node;

typedef enum {
  NODE_COMMAND,
  NODE_OPERATOR,
  NODE_REDIRECTION,
  NODE_ARGUMENT,
  NODE_PIPE,
} NodeType;

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

void	ft_strcpy(char *dst, const char *src);
//tokenizer
int			ft_is_space(char c);
int			get_exit_status(int exit_status);
void		ft_add_token(t_token **lexeme, char *node, int type);
void		ft_print_linked_list(t_token **lexeme);
void		ft_tokenize(char *input, t_token **lexeme);
void		ft_state_start(char **input, State *state, char **index_token, int *type);
void		ft_state_command(char **input, State *state, char **index_token, int *type);
void		ft_handle_quotes(char ***input, char ***index_token, int **type);
void		ft_last_token(char *current_token, t_token **lexeme, int *type);
void		ft_clean_token_list(t_token **lst);
void    ft_handle_operators(char ***input, char ***i_token, int **type);
char    *ft_get_prompt(void);

//parser
void      ft_execute_ast(t_ast_node *root);
void      ft_echo_command_with_ast(t_ast_node *node);
void	    ft_cd_command_with_ast(t_ast_node *node_ast);


//pipe
pid_t	first_child(int *channel, t_ast_node	*root);
pid_t	second_child(int *channel, t_ast_node *root, int index);

void		open_channel(t_pipex *pipex);
void		close_channel(t_pipex *pipex);
void		search_path(t_pipex *pipex);
void		handle_error(t_pipex *pipex, int exit_status, char *msg);
void		ft_free_split(char **split);
void		ft_free_split(char **split);
void		init_pipe(t_pipex *pipex);
void		ft_execute_command_ast(t_ast_node *root);
char		*ft_search_executable_ast(char *command);
void 		ft_execute_command_ast_pipe(t_ast_node *command_node);
void		ft_handle_pipe(t_ast_node *root);
pid_t		first_pipeline(int *channel, t_ast_node *root, int index);
int 		ft_handle_heredoc(const char *delimiter);
void 		ft_free_ast(t_ast_node *root);
int 		ft_collect_heredocs(t_ast_node *root);
t_ast_node *ft_build_ast(t_token **tokens);
void	ft_creat_cmd_node(t_token *current, t_ast_node **root, t_ast_node **current_node, t_ast_node **last_arg_node);
void	ft_creat_arg_node(t_token *current, t_ast_node **current_node, t_ast_node **last_arg_node);
void	ft_creat_pipe_node(t_ast_node **root, t_ast_node **current_node);
void	ft_creat_redir_node(t_token **current, t_ast_node **current_node);





//builtin
void	ft_pwd_command(t_ast_node *cmd_tokens);

t_token     *ft_list_new_token(void);
t_ast_node  *ft_build_ast(t_token **tokens);


#endif
