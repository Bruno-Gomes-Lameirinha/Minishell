/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:16:26 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 20:59:05 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_delete_env_key(char *key_to_delete)
{
	char	**new_env;
	char	**current_env;

	current_env = *ft_get_env();
	new_env = ft_allocate_new_env(current_env);
	if (!new_env)
		return ;
	ft_copy_env_except_key(current_env, new_env, key_to_delete);
	free(current_env);
	*ft_get_env() = new_env;
	__environ = new_env;
}

int	ft_copy_env_except_key(char **current_env, char **new_env, \
char *key_to_delete)
{
	int		i;
	int		j;
	char	*env_key;

	i = -1;
	j = -1;
	while (current_env[++i])
	{
		env_key = ft_get_key(current_env[i]);
		if (ft_strcmp(key_to_delete, env_key) == 0)
			free(current_env[i]);
		else
			new_env[++j] = current_env[i];
		free(env_key);
	}
	new_env[++j] = NULL;
	return (j);
}

char	**ft_allocate_new_env(char **current_env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (current_env[i])
		i++;
	new_env = (char **)ft_calloc(i + 1, sizeof(char *));
	if (!new_env)
	{
		perror("malloc");
		ft_update_status_error(1);
		return (NULL);
	}
	return (new_env);
}

int	ft_unset_command(t_ast_node *command)
{
	int		i;
	char	**args;
	int		status;

	i = 0;
	status = 0;
	args = ft_convert_tokens_to_args(command);
	if (!args[1])
	{
		ft_update_status_error(0);
		free(args);
		return (0);
	}
	while (args[++i])
	{
		if (!ft_is_valid_identifier(args[i], args[0]) && ++status)
			continue ;
		ft_delete_env_key(args[i]);
	}
	ft_update_status_error(status);
	free(args);
	return (status);
}
