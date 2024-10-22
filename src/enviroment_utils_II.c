/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:23:20 by livieira          #+#    #+#             */
/*   Updated: 2024/10/15 18:48:02 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_is_valid_identifier(char *str, char *cmd_name)
{
	if (*str != '=' && !ft_isdigit(*str) && *str != '\0')
	{
		while (*str && *str != '=' && (ft_isalnum(*str) || *str == '_'))
			str++;
		if (*str == '=' || !*str)
			return (1);
	}
	ft_printf(STDERR_FILENO, "%s: not a valid identifier\n", cmd_name);
	ft_set_exit_status(FAILURE);
	return (0);
}

int	ft_set_exit_status(int status)
{
	int	exit_status;

	exit_status = *ft_get_exit_status_env();
	exit_status = status;
	return (exit_status);
}

int	ft_is_env_key_present(char *key)
{
	int		i;
	char	*env_key;
	char	**env;

	env = *ft_get_env();
	i = -1;
	while (env[++i])
	{
		env_key = ft_get_key(env[i]);
		if (!ft_strcmp(key, env_key))
		{
			free(env_key);
			return (1);
		}
		free(env_key);
	}
	return (0);
}

int	ft_is_key_without_value(char *key)
{
	int		i;
	char	*env_key;
	char	**env;

	env = *ft_get_env();
	i = -1;
	while (env[++i])
	{
		if (!ft_strchr(env[i], '='))
		{
			env_key = ft_get_key(env[i]);
			if (!ft_strcmp(key, env_key))
				return (1);
		}
	}
	return (0);
}

char	*ft_get_key(char *arg)
{
	char	*equal_sign;

	if (!arg)
		return (NULL);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (ft_strdup(arg));
	else
		return (ft_strndup(arg, equal_sign - arg));
}
