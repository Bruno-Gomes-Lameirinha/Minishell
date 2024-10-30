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

void	ft_format_and_print(const char *env_var)
{
	char	*name;
	char	*equal;
	char	*value;
	char	*copy;

	copy = ft_strdup(env_var);
	equal = ft_strchr(copy, '=');
	if (!equal)
		ft_printf(1, "declare -x %s\n", copy);
	else
	{
		*equal = '\0';
		name = copy;
		value = equal + 1;
		ft_printf(1, "declare -x %s=\"%s\"\n", name, value);
	}
	free(copy);
}

int	ft_print_smallest_unprinted(char **env, size_t env_size, char *printed)
{
	int	small_pos;

	small_pos = -1;
	while (env_size-- > 0)
	{
		if (printed[env_size])
			continue ;
		if (small_pos == -1)
			small_pos = env_size;
		else
		{
			if (ft_strcmp(env[small_pos], env[env_size]) > 0)
				small_pos = env_size;
		}
	}
	if (small_pos != -1)
	{
		ft_format_and_print(env[small_pos]);
		printed[small_pos]++;
		return (1);
	}
	return (SUCCESS);
}

void	ft_print_env_sort(void)
{
	char	*printed;
	size_t	size;
	char	**env;

	env = *ft_get_env();
	size = 0;
	while (env[size])
		size++;
	printed = ft_calloc(size + 1, sizeof(char));
	if (!printed)
	{
		ft_update_status_error(1);
		return ;
	}
	while (ft_print_smallest_unprinted(env, size, printed))
		;
	free(printed);
}
