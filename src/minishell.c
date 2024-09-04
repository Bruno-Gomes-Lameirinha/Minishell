/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/30 21:37:23 by livieira         ###   ########.fr       */
/*   Updated: 2024/08/30 20:57:30 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_strcpy(char *dst, const char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char *ft_get_prompt(void)
{
	char cwd[1024];
	char *home_dir = getenv("HOME");
	char *relative_cwd;
	size_t cwd_len;
	size_t prompt_len;
	char *prompt;

	if (getcwd(cwd, sizeof(cwd)) != NULL) 
	{
		relative_cwd = cwd;

		if (home_dir && strncmp(cwd, home_dir, strlen(home_dir)) == 0) 
		{
			relative_cwd = cwd + strlen(home_dir);
			if (*relative_cwd == '/')
				relative_cwd++;
		}
		cwd_len = strlen(relative_cwd) + 1;
		prompt_len = strlen("Minishell $ ");
		prompt = malloc(cwd_len + prompt_len + 3);
		if (prompt == NULL) 
		{
			perror("Failed to allocate memory for prompt");
			exit(EXIT_FAILURE);
		}
        ft_strcpy(prompt, "Minishell ");
        if (relative_cwd == cwd) 
            ft_strlcat(prompt, cwd, cwd_len + prompt_len + 3);
        else 
		{
            ft_strlcat(prompt, "~", cwd_len + prompt_len + 3);
            ft_strlcat(prompt, "/", cwd_len + prompt_len + 3);
            ft_strlcat(prompt, relative_cwd, cwd_len + prompt_len + 3); 
        }
        ft_strlcat(prompt, "$ ", cwd_len + prompt_len + 3); 
		return prompt;
	} 
	else 
	{
		perror("getcwd() error");
		exit(EXIT_FAILURE);
	}
}


int	main(void)
{
	char	*input;
	t_env	*env;
	t_token	**lexeme;
	t_ast_node *ast;
	t_pipex		*pipex;
	char	*prompt;
	
	pipex = NULL;
	env = (t_env*)malloc(sizeof(t_env));
	lexeme = (t_token**)malloc(sizeof(t_token*));
	pipex = (t_pipex*)malloc(sizeof(t_pipex));
	
	if (!lexeme)
	{
		perror("Failed to allocate memory for lexeme");
		exit(EXIT_FAILURE);
	}
	*lexeme = NULL;
	while (1)
	{
		prompt = ft_get_prompt();
		input = readline(prompt);
		add_history(input);
		if (input)
		{
			ft_tokenize(input, lexeme);
			//ft_print_linked_list(lexeme);
			ast = ft_build_ast(lexeme);
			ft_execute_ast(ast, pipex);
			ft_clean_token_list(lexeme);
			free(input);
			free(prompt);
			free(ast);
		}
	}
	free(lexeme);
	free(env);
	return (0);
}

t_ast_node *ft_build_ast(t_token **tokens)
{
	t_ast_node *root;
	t_ast_node *current_node = NULL;
	t_ast_node *last_arg_node = NULL;
	t_token *current = *tokens;

	current = *tokens;
	current_node = NULL;
	last_arg_node = NULL;
	root = NULL;
	while (current)
	{
		if (current->type_token == WORD)
		{
			if (!current_node || current_node->type != NODE_COMMAND)
			{
				t_ast_node *cmd_node = malloc(sizeof(t_ast_node));
				cmd_node->type = NODE_COMMAND;
				cmd_node->value = ft_strdup(current->token_node);
				cmd_node->left = NULL;
				cmd_node->right = NULL;
				cmd_node->next = NULL;

				if (root == NULL)
				{
					root = cmd_node;
				}
				else if (current_node && (current_node->type == NODE_PIPE || current_node->type == REDIR_OUT || current_node->type == REDIR_OUTAPP))
				{
					current_node->right = cmd_node;
				}

				current_node = cmd_node;
				last_arg_node = NULL; // Reinicia o último argumento, pois é um novo comando
			}
			else
			{
				// Adicionando argumentos ao comando
				t_ast_node *arg_node = malloc(sizeof(t_ast_node));
				arg_node->type = NODE_ARGUMENT;
				arg_node->value = ft_strdup(current->token_node);
				arg_node->left = NULL;
				arg_node->right = NULL;
				arg_node->next = NULL;

				if (!last_arg_node)
				{
					current_node->right = arg_node;
				}
				else
				{
					last_arg_node->next = arg_node;
				}
				last_arg_node = arg_node;
			}
		}
		else if (current->type_token == PIPE)
		{
			t_ast_node *pipe_node = malloc(sizeof(t_ast_node));
			pipe_node->type = NODE_PIPE;
			pipe_node->value = NULL;
			pipe_node->left = root;
			pipe_node->right = NULL;
			root = pipe_node;
			current_node = pipe_node;
		}
		else if (current->type_token == REDIR_OUT || current->type_token == REDIR_OUTAPP)
		{
			t_ast_node *redir_node = malloc(sizeof(t_ast_node));
			redir_node->type = NODE_REDIRECTION;
			redir_node->type_token = current->type_token;
			redir_node->value = ft_strdup(current->next->token_node);
			redir_node->left = root;
			redir_node->right = NULL;
			root = redir_node;
			current_node = redir_node;
			current = current->next; // Pula o token do nome do arquivo
		}
		current = current->next;
	}

	return root;
}


void	execute(t_pipex *pipex, char *command)
{
	//char	*executable;

	pipex->argv_childs = ft_split(command, ' ');
	if (pipex->argv_childs == NULL)
		handle_error(pipex, 2, NULL);
	//executable = search_executable(pipex);
	// if (executable == NULL)
	// 	handle_error(pipex, 127, NULL);

	execve("/usr/bin/wc", pipex->argv_childs, pipex->parent_env);
}

void ft_execute_ast(t_ast_node *root, t_pipex *pipex)
{
	if (!root)
		return;

	if (root->type == NODE_PIPE)
	{
		init_pipe(pipex);
		if (pipe(pipex->channel) == -1) 
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		first_child(pipex, root->left); 
		second_child(pipex);
		close(pipex->channel[0]);
		close(pipex->channel[1]);
		waitpid(pipex->first_child, NULL, 0);
		waitpid(pipex->second_child, NULL, 0);
		//ft_execute_ast(root->left, pipex);
	}
	else if (root->type == NODE_REDIRECTION) 
	{
		int fd;
		if (root->type_token == REDIR_OUT)
			fd = open(root->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(root->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0) 
		{
			perror("open");
			return;
		}

		int saved_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		ft_execute_ast(root->left, pipex); 
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	else if (root->type == NODE_COMMAND)
	{
		if (!strcmp(root->value, "cd"))
			ft_cd_command_with_ast(root);
		else if (!strcmp(root->value, "echo"))
			ft_echo_command_with_ast(root);
		else if (!strcmp(root->value, "pwd"))
			ft_pwd_command(root);
		//else
			// aqui vamos chamar o execv para rodar os comandos que não precisamos implementar
	}
}

void	first_child(t_pipex *pipex, t_ast_node	*root)
{
	pipex->first_child = fork();
	if (pipex->first_child == -1)
	{
		perror("fork second child");
		exit(EXIT_FAILURE);
	}
	if (pipex->first_child == 0)
	{
		close(pipex->channel[0]); 
		dup2(pipex->channel[1], STDOUT_FILENO);
		close(pipex->channel[1]);
		if (root->type == NODE_COMMAND)
			ft_echo_command_with_ast(root);
		exit(EXIT_SUCCESS); 
	}
}
void	init_pipe(t_pipex *pipex)
{
	pipex->parent_env = malloc(sizeof(char*) * 2); 
	if (pipex->parent_env == NULL) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	pipex->parent_argv = malloc(sizeof(char*) * 2); 
	if (pipex->parent_env == NULL) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	pipex->path = malloc(sizeof(char*) * 2);
	if (pipex->parent_env == NULL) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	pipex->parent_env[0] = strdup("SHELL=/bin/bash");
	pipex->parent_env[1] = NULL; 
	pipex->parent_argv[0] = strdup("./minishell"); 
	pipex->parent_argv[1] = NULL; 
	pipex->path[0] = strdup("/home/bruno/.local/bin");
	pipex->path[1] = NULL; 

   
}

void	second_child(t_pipex *pipex)
{
	pipex->second_child = fork();
	if (pipex->second_child == -1)
	{
		perror("fork second child");
		exit(EXIT_FAILURE);
	}
	if (pipex->second_child == 0)
	{
		close(pipex->channel[1]);
		dup2(pipex->channel[0], STDIN_FILENO);
		close(pipex->channel[0]);
		execute(pipex, "wc");
		exit(EXIT_SUCCESS);
	}

}

void	*search_executable(t_pipex *pipex)
{
	char	*executable;
	char	*command;
	char	*temp;
	int		i;

	i = 0;
	command = pipex->argv_childs[0];
	// if (access(command, X_OK) == 0)
	// 	return (command);
	while (pipex->path[i])
	{
		temp = ft_strjoin(pipex->path[i], "/");
		executable = ft_strjoin(temp, command);
		// if (access(executable, X_OK) == 0)
		// 	return (executable);
		free(temp);
		free(executable);
		i++;
	}
	return (NULL);
}

void	handle_error(t_pipex *pipex, int exit_status, char *msg)
{
	if (exit_status == 1)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(msg);
	}
	else if (exit_status == 2)
		ft_putstr_fd("pipex: failed to free split\n", 2);
	else if (exit_status == 3)
		ft_putstr_fd("Invalid arguments\n", 2);
	else if (exit_status == 127)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(pipex->argv_childs[0], 2);
		ft_putstr_fd(" command not found\n", 2);
		ft_free_split(pipex->argv_childs);
		ft_free_split(pipex->path);
	}
	exit(exit_status);
}

void	open_channel(t_pipex *pipex)
{
	if (pipe(pipex->channel) == -1)
		handle_error(pipex, 1, NULL);
}

void	close_channel(t_pipex *pipex)
{
	close(pipex->channel[0]);
	close(pipex->channel[1]);
}

void	search_path(t_pipex *pipex)
{
	char	**path;
	char	**env;

	env = pipex->parent_env;
	while (ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	*env += 5;
	path = ft_split(*env, ':');
	if (path == NULL)
		handle_error(pipex, 2, NULL);
	pipex->path = path;
}

int	get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}