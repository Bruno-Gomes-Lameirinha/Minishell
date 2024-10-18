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
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_free_args(args);
		ft_update_status_error(127);
		return;
	}
	command_node->execve_child = fork();
	if (command_node->execve_child == -1)
	{
		perror("fork");
		ft_free_args(args);
		ft_update_status_error(1);
		exit(EXIT_FAILURE);
	}
	if (command_node->execve_child == 0)
	{
		ft_free_ast(command_node->head);
		execve(executable, args, NULL);
		perror("execve");
		ft_free_args(args);
		ft_printf(2, "%s", command_node->head);
		exit(126);
	}
	else
	{
		ft_pid_last_exit_status(command_node->execve_child);
		ft_free_args(args);
		free(executable);
	}
}

char	**ft_generate_args(t_ast_node *command_node)
{
	char	**args;

	args = ft_allocate_args(command_node);
	ft_fill_args(args, command_node);
	return (args);
}

void	ft_fill_args(char **args, t_ast_node *command_node)
{
	t_ast_node	*current;
	int			i;

	current = command_node;
	i = 0;
	while (current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
		{
			perror("ft_strdup");
			exit(EXIT_FAILURE);
		}
		i++;
		current = current->right;
	}
	args[i] = NULL;
}

char	**ft_allocate_args(t_ast_node *command_node)
{
	int			n_args;
	t_ast_node	*current;
	char		**args;

	n_args = 0;
	current = command_node;
	while (current)
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
	return (args);
}

void	ft_free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
