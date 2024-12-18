/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 13:53:10 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_execute_command_ast(t_ast_node *command_node)
{
	char	**args;
	char	*executable;

	args = ft_generate_args(command_node);
	executable = ft_search_executable_ast(args[0]);
	if (!executable)
	{
		ft_free_args(args);
		return ;
	}
	command_node->execve_child = fork();
	if (command_node->execve_child == -1)
		ft_handle_fork_error(args);
	if (command_node->execve_child == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_execute_child_process(command_node, executable, args);
	}
	else
		ft_pid_last_exit_status(command_node->execve_child);
	ft_free_args(args);
	free(executable);
}

void	ft_execute_child_process(t_ast_node *command_node, \
char *executable, char **args)
{
	char	**envp;

	envp = *ft_get_env();
	ft_free_ast(command_node->head);
	execve(executable, args, envp);
	perror("execve");
	ft_free_args(args);
	exit(126);
}

void	ft_handle_fork_error(char **args)
{
	perror("fork");
	ft_free_args(args);
	ft_update_status_error(1);
	exit(EXIT_FAILURE);
}

void	ft_handle_command_not_found(char **args)
{
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	ft_free_args(args);
	ft_update_status_error(127);
}

char	**ft_get_paths(void)
{
	char	*path_var;
	char	**paths;

	path_var = getenv("PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	return (paths);
}
