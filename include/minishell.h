/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:00:47 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/15 18:45:47 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SUCCESS 0
# define FAILURE 1
# define SYNTAX_ERROR 2

# define INVALID_QUOTE "Invalid syntax,check for open quotes or brackets.\n"

typedef struct s_token
{
	char			*token_node;
	int				type_token;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection {
	int						type_token;
	char					*filename;
	int						type_filename;
	int						heredoc_fd;
	struct s_redirection	*next;
}	t_redir;

typedef struct s_ast_node {
	int					type;
	int					type_token;
	char				*value;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*next;
	struct s_ast_node	*head;
	pid_t				execve_child;
	t_redir				*redirections;
}	t_ast_node;

typedef struct s_ast_builder_context
{
	t_ast_node		*root;
	t_ast_node		*current_node;
	t_ast_node		*last_arg_node;
	t_redir			*pending_redir;
}	t_ast_builder;

typedef enum t_node_type {
	NODE_COMMAND,
	NODE_OPERATOR,
	NODE_REDIRECTION,
	NODE_ARGUMENT,
	NODE_PIPE,
}	t_NodeType;

typedef enum t_state_token {
	TOKEN_STATE_START,
	TOKEN_STATE_COMMAND,
	TOKEN_STATE_OPERATOR,
	TOKEN_STATE_END,
}	t_state;

enum e_token
{
	WORD,
	PIPE,
	R_IN,
	R_HDOC,
	R_OUT,
	R_OUTAPP,
	OR,
	AND,
	SUBSHELL,
	ARCHIVE,
	DOUBLE_QUOTES,
	SINGLE_QUOTES
};

int			ft_is_space(char c);
int			ft_get_exit_status(int exit_status);
int			*ft_get_exit_status_env(void);
void		ft_add_token(t_token **lexeme, char *node, int type);
void		ft_tokenize(char *input, t_token **lexeme);
void		ft_state_start(char **input, t_state *state, char **index_token, \
int *type);
void		ft_state_command(char **input, t_state *state, char **index_token, \
int *type);
void		ft_handle_quotes(char ***input, char ***index_token, int **type);
void		ft_last_token(char *current_token, t_token **lexeme, int *type);
void		ft_clean_token_list(t_token **lst);
void		ft_handle_operators(char ***input, char ***i_token, int **type);
char		*ft_get_prompt(void);
void		ft_execute_ast(t_ast_node *root);
void		ft_echo_command_with_ast(t_ast_node *node);
void		ft_cd_command_with_ast(t_ast_node *node_ast);
void		ft_free_split(char **split);
void		ft_execute_command_ast(t_ast_node *root);
char		*ft_search_executable_ast(char *command);
void		ft_handle_pipe(t_ast_node *root);
int			ft_handle_heredoc(const char *delimiter, int type);
void		ft_free_ast(t_ast_node *root);
int			ft_collect_heredocs(t_ast_node *root);
t_ast_node	*ft_build_ast(t_token **tokens);
void		ft_creat_cmd_node(t_token *current, t_ast_builder *ctx);
void		ft_creat_arg_node(t_token *current, t_ast_builder *ctx);
void		ft_creat_pipe_node(t_ast_builder *ctx);
void		ft_creat_redir_node(t_token **current, t_ast_builder *ctx);
int			ft_handle_redirection(t_redir *redir, int *saved_stdin, \
int *saved_stdout);
void		ft_restore_stdin_stdout(int saved_stdin, int saved_stdout);
void		ft_execute_command(t_ast_node *root);
int			ft_handle_redirection_in(t_redir *redir, int *saved_stdin);
int			ft_handle_redirection_out(t_redir *redir, int *saved_stdout);
int			ft_handle_redirection_out_append(t_redir *redir, int *saved_stdout);
int			ft_handle_heredoc_redirection(t_redir *redir, int *saved_stdin);
void		ft_pwd_command(t_ast_node *cmd_tokens);
t_token		*ft_list_new_token(void);
t_ast_node	*ft_build_ast(t_token **tokens);
char		**ft_generate_args(t_ast_node *command_node);
void		ft_fill_args(char **args, t_ast_node *command_node);
char		**ft_allocate_args(t_ast_node *command_node);
void		ft_free_args(char **args);
char		**ft_get_paths(void);
char		*ft_build_executable_path(char *dir, char *command);
char		*ft_search_in_paths(char **paths, char *command);
int			ft_check_limits(char *arg, char sign);
void		ft_status(int status);
int			ft_validate_argument(char *arg);
int			ft_exit_command(t_ast_node *command);
void		ft_format_and_print(const char *env_var);
int			ft_print_smallest_unprinted(char **env, size_t env_size, \
char *printed);
void		ft_print_env_sort(void);
int			ft_export_command(t_ast_node *command);
char		***ft_get_env(void);
char		*ft_get_key(char *arg);
char		**ft_copy_env(void);
void		ft_add_env(char *string);
void		ft_update_env(char *new_str, char *key);
int			ft_unset_command(t_ast_node *command);
int			ft_is_key_without_value(char *key);
int			ft_is_env_key_present(char *key);
void		ft_set_env(char *new_str, char *key, char *value);
void		ft_env_command(t_ast_node *command);
int			ft_set_exit_status(int status);
int			ft_is_valid_identifier(char *str, char *cmd_name);
char		**ft_convert_tokens_to_args(t_ast_node *command);
int			ft_count_tokens(t_ast_node *command);
int			ft_strlen_var(char *str);
char		*ft_expand_variables_input(char *input);
char		*ft_get_env_value(char *key);
char		*ft_strjoin_free(char *s1, char *s2);
void		ft_pid_last_exit_status(pid_t pid);
int			ft_update_status_error(int exit_status);
void		ft_setup_signal_handlers(void);
void		ft_handle_eof(void);
void		ft_sigint_handler(int signum);
void		ft_set_head_lst(t_ast_node *root);
char		*ft_skip_until_special_char(char *input);
int			ft_handle_special_char(char **input, char **start, \
char **expanded_input);
char		*ft_skip_single_quotes(char *input);
int			ft_should_skip_dollar(char *input);
char		*ft_append_text(char *start, char *end, char *expanded_input);
char		*ft_process_variable(char *input, char **expanded_input);
void		ft_handle_word_token(t_token *cur, t_ast_builder *ctx);
void		ft_free_env(char **env);
void		ft_print_arguments(t_ast_node *current);
void		ft_write_space(void);
void		ft_write_value(const char *value);
void		ft_print_newline(int new_line);
void		ft_handle_command_not_found(char **args);
void		ft_handle_fork_error(char **args);
void		ft_execute_child_process(t_ast_node *command_node, \
char *executable, char **args);
void		ft_handle_exit_argument(t_ast_node *command);
void		ft_process_export_arg(char *arg, int *status);
int			ft_handle_no_args_export(char **args, int status);
void		ft_process_heredoc_line(int *pipe_fd, char *line, int type);
void		ft_perror_exit(const char *msg);
void		ft_handle_child_left(int *fd, t_ast_node *root);
void		ft_handle_child_right(int *fd, t_ast_node *root);
int			ft_perror_close_exit(const char *msg, int fd);
void		ft_handle_pipe_operator(char ***input, char ***i_token, int **type);
void		ft_handle_and_operator(char ***input, char ***i_token, int **type);
void		ft_handle_redirect_out_operator(char ***input, char ***i_token, \
int **type);
void		ft_handle_redirect_in_operator(char ***input, char ***i_token, \
int **type);
void		ft_reset_token_state(char *current_token, char **i_token, \
t_state *state);
void		ft_free_token(char	*current_token, int	*type);
char		*ft_mem_token(char *input);
void		ft_delete_env_key(char *key_to_delete);
int			ft_copy_env_except_key(char **current_env, char **new_env, \
char *key_to_delete);
char		**ft_allocate_new_env(char **current_env);
char		*get_input(void);
void		process_input(char *input);
char		*ft_handle_command_without_slash(char *command);
char		*ft_handle_command_with_slash(char *command);
int			ft_check_unclosed_quotes(const char *input);
void		ft_add_redirection_to_pending(t_redir **pending_redir, \
t_redir *redir);
void		ft_add_redirection_to_command(t_ast_node *current_node, \
t_redir *redir);
t_redir		*create_redirection(t_token **current);
void		ft_update_status_from_children(int status[2]);
t_ast_node	*ft_initialize_cmd_node(t_token *current);

#endif
