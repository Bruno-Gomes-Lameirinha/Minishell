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