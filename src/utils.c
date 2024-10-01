/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:23:20 by livieira          #+#    #+#             */
/*   Updated: 2024/10/01 19:45:11 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Extrai a chave de uma variável de ambiente a partir de uma string no formato "nome=valor"
char	*get_key(char *arg)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (ft_strdup(arg));
	else
		return (ft_strndup(arg, equal_sign - arg));
}

//Verifica se uma string é um identificador válido para uma variável de ambiente.
int	is_valid_identifier(char *str, char *cmd_name)
{
	if (*str != '=' && !ft_isdigit(*str) && *str != '\0')
	{
		while (*str && *str != '=' && (ft_isalnum(*str) || *str == '_'))
			str++;
		if (*str == '=' || !*str)
			return (1);
	}
	ft_fprintf(STDERR_FILENO, "%s: not a valid identifier\n", cmd_name);
	set_exit_status(FAILURE);
	return (0);
}

//Verifica se uma chave de variável de ambiente está presente no conjunto de variáveis.
int	is_env_key_present(char *key)
{
	int		i;
	char	*env_key;
	char	**env;

	env = *ft_get_env();
	i = -1;
	while (env[++i])
	{
		env_key = get_key(env[i]);
		if (!ft_strcmp(key, env_key))
			return (1);
	}
	return (0);
}

//Verifica se uma chave de variável de ambiente está presente e não possui valor.
int	is_key_without_value(char *key)
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
			env_key = get_key(env[i]);
			if (!ft_strcmp(key, env_key))
				return (1);
		}
	}
	return (0);
}
