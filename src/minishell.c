/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/29 15:07:39 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

int	main(int argc, char **env)
{
	char	*input;
	//t_env	*env;
	t_token	**lexeme;
	t_ast_node *ast;
	t_pipex		*pipex;
	
	printf("%d\n", argc);

	pipex = NULL;
	//env = (t_env*)malloc(sizeof(t_env));
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
		input = readline("Minishell$ ");
		add_history(input);
		if (input)
		{
			//printf("Você digitou: %s\n", input);
			ft_tokenize(input, lexeme);
			//ft_print_linked_list(lexeme);
			//ft_echo_command(lexeme);
			ast = ft_build_ast(lexeme);
			ft_execute_ast(ast, pipex, env);

			//ft_cd_command(lexeme);
			ft_clean_token_list(lexeme);
			free(input);
		}
	}
	free(lexeme);
	free(env);
	return (0);
}

t_ast_node *ft_build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_token		*current;
	t_ast_node *last_arg_node;
	t_ast_node *cmd_node;
	t_ast_node *arg_node;
	t_ast_node *metachar_node;
	current = *tokens;

	last_arg_node = NULL;
	root = NULL;
	cmd_node = NULL;
	arg_node = NULL;
	while (current)	
	{
		if (current->type_token == WORD)
			if (!cmd_node)
			{
				cmd_node = malloc(sizeof(t_ast_node));
				cmd_node->type = NODE_COMMAND;
				cmd_node->value = ft_strdup(current->token_node);
				cmd_node->left = NULL;
				cmd_node->right = NULL;
				cmd_node->next = NULL;
				root = cmd_node;
			}
			else 
			{
				arg_node = malloc(sizeof(t_ast_node));
				arg_node->type = NODE_ARGUMENT;
				arg_node->value = ft_strdup(current->token_node);
				arg_node->left = NULL;
				arg_node->right = NULL;
				arg_node->next = NULL;
				if (!last_arg_node) 
					cmd_node->right = arg_node;
				else
					last_arg_node->next = arg_node;
				last_arg_node = arg_node;
			}
		
		else if (current->type_token == REDIR_OUT || current->type_token == REDIR_OUTAPP)
		{
			metachar_node = malloc(sizeof(t_ast_node));
			metachar_node->type = current->type_token;
			metachar_node->value = ft_strdup(current->next->token_node);
			metachar_node->left = cmd_node;
			metachar_node->right = NULL;
			metachar_node->next = NULL;
			root = metachar_node;
			current = current->next;
		}
		else if (current->type_token == PIPE)
		{
			metachar_node = malloc(sizeof(t_ast_node));
			metachar_node->type = current->type_token;
			metachar_node->value = ft_strdup(current->next->token_node);
			metachar_node->left = cmd_node;
			metachar_node->right = NULL;
			metachar_node->next = NULL;
			root = metachar_node;
			current = current->next;
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

void ft_execute_ast(t_ast_node *root, t_pipex *pipex, char **env)
{
	if (!root)
		return ;
	if (root->type == NODE_COMMAND)
		ft_echo_command_with_ast(root);
	else if (root->type == REDIR_OUT || root->type == REDIR_OUTAPP) 
	{
		int fd;
		if (root->type == REDIR_OUT)
			fd = open(root->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(root->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0) {
			perror("open");
			return;
		}

		int saved_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		ft_execute_ast(root->left, pipex, env);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	else if (root->type == PIPE)
	{
		init_pipe(pipex, env);
		if (pipe(pipex->channel) == -1) 
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		first_child(pipex);
		second_child(pipex);
		close(pipex->channel[0]);
        close(pipex->channel[1]);
		waitpid(pipex->first_child, NULL, 0);
		waitpid(pipex->second_child, NULL, 0);
	}
}

void	first_child(t_pipex *pipex, t_ast_node	*root)
{
    pipex->first_child = fork();
    if (pipex->first_child == 0)
    {
        close(pipex->channel[0]); // Fecha o lado de leitura do pipe

        // Redireciona stdout para o descritor de escrita do pipe
        dup2(pipex->channel[1], STDOUT_FILENO);
        close(pipex->channel[1]); // Fecha o descritor de escrita do pipe após redirecionamento

        //execute(pipex, "echo abc"); // Executa o comando
        if (root->left == Node_command)
            ft_echo_comand_with_ast(root);
        exit(EXIT_SUCCESS); // Certifique-se de que o processo filho termine corretamente
    }
}
void	init_pipe(t_pipex *pipex, char **env)
{
	pipex->parent_env = env;
	//printf("pipex->parent_env é: %s\n", *pipex->parent_env);
}

void	second_child(t_pipex *pipex)
{
    int outfile;

    pipex->second_child = fork();
    if (pipex->second_child == 0)
    {
        close(pipex->channel[1]); // Fecha o lado de escrita do pipe

        // Redireciona stdin para o descritor de leitura do pipe
        dup2(pipex->channel[0], STDIN_FILENO);
        close(pipex->channel[0]); // Fecha o descritor de leitura do pipe após redirecionamento

        outfile = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (outfile == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        // Redireciona stdout para o arquivo de saída
        dup2(outfile, STDOUT_FILENO);
        close(outfile); // Fecha o arquivo após redirecionamento

        execute(pipex, "wc"); // Executa o comando
        exit(EXIT_SUCCESS); // Certifique-se de que o processo filho termine corretamente
    }
}

void ft_echo_command_with_ast(t_ast_node *node) 
{
	if (!node) return;

	int new_line = 1;
	t_ast_node *current = node->right;

	if (current && !strcmp(current->value, "-n")) 
	{
		new_line = 0;
		current = current->next;
	}

	while (current)
	{
		write(1, current->value, strlen(current->value));
		if (current->next)
			write(1, " ", 1);
		current = current->next;
	}

	if (new_line)
		write(1, "\n", 1);
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