/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 20:43:45 by livieira          #+#    #+#             */
/*   Updated: 2024/10/14 19:30:57 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_copy_env(void)
{
	char	**copy;
	int		i;

	i = 0;
	while (__environ[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (__environ[i])
	{
		copy[i] = ft_strdup(__environ[i]);
		if (!copy[i])
		{
			perror("ft_strdup failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	ft_add_env(char *string)
{
	char	**new_var;
	char	**env;
	int		i;

	env = *ft_get_env();
	i = 0;
	while (env[i])
		i++;
	new_var = malloc(sizeof(char *) * (i + 2));
	if (!new_var)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (env[i])
	{
		new_var[i] = env[i];
		i++;
	}
	new_var[i++] = ft_strdup(string);
	new_var[i] = NULL;
	free(env);
	*ft_get_env() = new_var;
	__environ = new_var;
}

void	ft_update_env(char *new_str, char *key)
{
	int		i;
	char	*env_key;
	char	*aux;
	char	**env;

	env = *ft_get_env();
	i = -1;
	while (env[++i])
	{
		env_key = ft_get_key(env[i]);
		if (!ft_strcmp(key, env_key))
		{
			aux = env[i];
			env[i] = ft_strdup_calloc(new_str);
			free(aux);
			free(env_key);
			return ;
		}
		free(env_key);
	}
	ft_add_env(new_str);
}

void	ft_env_command(t_ast_node *command)
{
	int		i;
	char	**env;

	env = *ft_get_env();
	if (command->right)
	{
		write(STDERR_FILENO, "env: too many arguments\n", 24);
		ft_update_status_error(1);
		return ;
	}
	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	ft_update_status_error(0);
	return ;
}

void	ft_set_env(char *new_str, char *key, char *value)
{
	char	*env_var;

	env_var = getenv(key);
	if ((env_var || ft_is_key_without_value(key)) && value != NULL)
		ft_update_env(new_str, key);
	else if (!ft_is_env_key_present(key))
		ft_add_env(new_str);
}
