/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
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
		return (ft_handle_command_not_found(args));
	command_node->execve_child = fork();
	if (command_node->execve_child == -1)
		ft_handle_fork_error(args);
	if (command_node->execve_child == 0)
		ft_execute_child_process(command_node, executable, args);
	else
		ft_pid_last_exit_status(command_node->execve_child);
	ft_free_args(args);
	free(executable);
}

void	ft_execute_child_process(t_ast_node *command_node, \
char *executable, char **args)
{
	ft_free_ast(command_node->head);
	execve(executable, args, NULL);
	perror("execve");
	ft_free_args(args);
	ft_printf(2, "%s", command_node->head);
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
