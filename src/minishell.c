/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/09/24 15:35:58 by livieira         ###   ########.fr       */
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
	char	*prompt;
	
	env = (t_env*)malloc(sizeof(t_env));
	lexeme = (t_token**)malloc(sizeof(t_token*));
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
			ft_collect_heredocs(ast);
			ft_execute_ast(ast);
			ft_clean_token_list(lexeme);
			free(input);
			free(prompt);
			ft_free_ast(ast);
		}
	}
	free(lexeme);
	free(env);
	return (0);
}




char	*ft_search_executable_ast(char *command)
{
	char	*executable;
	char	*temp;
	char	*path_var;
	char	**paths;
	int		i;

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	path_var = getenv("PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		executable = ft_strjoin(temp, command);
		free(temp);
		if (access(executable, X_OK) == 0)
		{
			ft_free_split(paths);
			return (executable);
		}
		free(executable);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}


void ft_execute_command_ast(t_ast_node *command_node)
{
	t_ast_node *current;
	char	**args;
	char	*executable;
	int		n_args;
	int i;

	i = 0;
	n_args = 0;
	current = command_node;
	while(current)
	{
		n_args++;
		current = current->right;
	}
	args = malloc(sizeof(char *) * (n_args + 1));
	if (!args)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	current = command_node;
	while (current)
	{
		args[i] = ft_strdup(current->value);
		i++;
		current = current->right;
	}
	args[i] = NULL;
	executable = ft_search_executable_ast(args[0]);
	command_node->execve_child = fork();
	if (command_node->execve_child == -1)
	{
		perror("fork second child");
		exit(EXIT_FAILURE);
	}
	if (command_node->execve_child == 0)
		execve(executable, args, NULL);
	else
		waitpid(command_node->execve_child, NULL, 0);
}

void ft_execute_ast(t_ast_node *root)
{
	int saved_stdin;
	int saved_stdout;
	t_redirection *redir;

    if (!root)
        return;    

    if (root->type == NODE_PIPE)
    {
        ft_handle_pipe(root);
        while (wait(NULL) > 0);
    }
    else if (root->type == NODE_COMMAND)
    {
        saved_stdin = -1;
        saved_stdout = -1;
        redir = root->redirections;
        while (redir)
        {
            if (redir->type_token == REDIR_IN)
            {
                int fd = open(redir->filename, O_RDONLY);
                if (fd < 0)
                {
                    perror("open");
                    return;
                }
                if (saved_stdin == -1)
                    saved_stdin = dup(STDIN_FILENO);
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (redir->type_token == REDIR_OUT)
            {
                int fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0)
                {
                    perror("open");
                    return;
                }
                if (saved_stdout == -1)
                    saved_stdout = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (redir->type_token == REDIR_OUTAPP)
            {
                int fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0)
                {
                    perror("open");
                    return;
                }
                if (saved_stdout == -1)
                    saved_stdout = dup(STDOUT_FILENO);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (redir->type_token == REDIR_HDOC)
            {
                if (redir->heredoc_fd == -1)
                    return;
                if (saved_stdin == -1)
                    saved_stdin = dup(STDIN_FILENO);
                dup2(redir->heredoc_fd, STDIN_FILENO);
                close(redir->heredoc_fd);
                redir->heredoc_fd = -1;
            }
            redir = redir->next;
        }
        if (!strcmp(root->value, "cd"))
            ft_cd_command_with_ast(root);
        else if (!strcmp(root->value, "echo"))
            ft_echo_command_with_ast(root);
        else if (!strcmp(root->value, "pwd"))
            ft_pwd_command(root);
        else
            ft_execute_command_ast(root);
        if (saved_stdin != -1)
        {
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
        if (saved_stdout != -1)
        {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
    }
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

int	get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}