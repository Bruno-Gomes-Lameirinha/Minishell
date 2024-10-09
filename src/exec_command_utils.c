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

char	*ft_check_executable(char *executable)
{
	if (access(executable, X_OK) == 0)
		return (executable);
	free(executable);
	return (NULL);
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
		executable = ft_check_executable(executable);
		if (executable)
			return (executable);
		i++;
	}
	return (NULL);
}

char	*ft_search_executable_ast(char *command)
{
	char	**paths;
	char	*executable;

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	paths = ft_get_paths();
	if (!paths)
		return (NULL);
	executable = ft_search_in_paths(paths, command);
	ft_free_split(paths);
	return (executable);
}
