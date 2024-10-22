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
	if (ft_strchr(command, '/') != NULL)
		return (ft_handle_command_with_slash(command));
	else
		return (ft_handle_command_without_slash(command));
}

char	*ft_handle_command_with_slash(char *command)
{
	if (access(command, F_OK) != 0)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": Arquivo ou diretório não encontrado\n", STDERR_FILENO);
		ft_update_status_error(127);
		return (NULL);
	}
	if (access(command, X_OK) != 0)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": Permissão negada\n", STDERR_FILENO);
		ft_update_status_error(126);
		return (NULL);
	}
	return (ft_strdup(command));
}

char	*ft_handle_command_without_slash(char *command)
{
	char	**paths;
	char	*executable;

	paths = ft_get_paths();
	if (!paths)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_update_status_error(127);
		return (NULL);
	}
	executable = ft_search_in_paths(paths, command);
	ft_free_split(paths);
	if (executable)
		return (executable);
	else
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_update_status_error(127);
		return (NULL);
	}
}

char	*ft_build_executable_path(char *dir, char *command)
{
	char	*temp;
	char	*executable;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	executable = ft_strjoin(temp, command);
	free(temp);
	return (executable);
}

char	*ft_search_in_paths(char **paths, char *command)
{
	char	*executable;
	int		i;

	i = 0;
	while (paths[i])
	{
		executable = ft_build_executable_path(paths[i], command);
		if (!executable)
		{
			i++;
			continue ;
		}
		if (access(executable, X_OK) == 0)
			return (executable);
		free(executable);
		i++;
	}
	return (NULL);
}
