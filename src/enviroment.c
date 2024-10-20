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

char	***ft_get_env(void)
{
	static char	**env;

	env = ft_copy_env();
	return (&env);
}

char	**ft_copy_env(void)
{
	char	**copy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (__environ[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (__environ[i])
	{
		if (ft_strncmp(__environ[i], "COLUMNS=", 8) != 0 && 
		    ft_strncmp(__environ[i], "LINES=", 6) != 0)
		{
			copy[j] = ft_strdup(__environ[i]);
			j++;
		}
		i++;
	}
	copy[i] = NULL;
	__environ = copy;
	return (copy);
}

void	ft_add_env(char *string)
{
	char	**new_var;
	char	**env;
	int		i;

	i = 0;
	env = *ft_get_env();
	while (env[i])
		i++;
	new_var = ft_calloc(i + 2, sizeof(char *));
	i = 0;
	while (env[i])
	{
		new_var[i] = env[i];
		i++;
	}
	new_var[i++] = ft_strdup(string);
	new_var[i] = NULL;
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
		env_key = get_key(env[i]);
		if (!ft_strcmp(key, env_key))
		{
			aux = env[i];
			env[i] = ft_strdup_calloc(new_str);
			free(aux);
			break ;
		}
	}
	*ft_get_env() = env;
	__environ = env;
}

void	ft_set_env(char *new_str, char *key, char *value)
{
	char	*env_var;

	env_var = getenv(key);
	if ((env_var || is_key_without_value(key)) && value != NULL)
		ft_update_env(new_str, key);
	else if (!is_env_key_present(key))
		ft_add_env(new_str);
}

void	ft_env_command(t_ast_node *command)
{
    int     i;
    char    **env;
    

    env = *ft_get_env();
	if (command->right)
	{
		write(STDERR_FILENO, "env: too many arguments\n", 24);
		update_status_error(1);
		return ;
	}
	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	update_status_error(0);
	return ;  
}
