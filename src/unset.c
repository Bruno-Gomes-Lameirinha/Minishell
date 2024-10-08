/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:16:26 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 17:23:57 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	delete_env_key(char *key_to_delete)
{
	int		i;
	int		j;
	char	*env_key;
	char	**new_env;
	char	**current_env;

	current_env = *ft_get_env();
	i = 0;
	while (current_env[i])
		i++;
	new_env = (char **)ft_calloc(i + 1, (sizeof(char *)));
	i = -1;
	j = -1;
	while (current_env[++i])
	{
		env_key = get_key(current_env[i]);
		if (ft_strcmp(key_to_delete, env_key))
			new_env[++j] = current_env[i];
		else
			free(current_env[i]);
	}
	free(current_env);
	new_env[++j] = NULL;
	*ft_get_env() = new_env;
	__environ = new_env;
}

int	ft_unset_command(t_token *token_node)
{
	int		i;
	char    **args;
	int		status;

	i = 0;
	status = 0;
	args = convert_tokens_to_args(token_node);;
	if (!args[1])
		return (set_exit_status(status));
	while (args[++i])
	{
		if (!is_valid_identifier(args[i], args[0]) && ++status)
			continue ;
		delete_env_key(get_key(args[i]));
	}
	return (set_exit_status(!!status));
}
