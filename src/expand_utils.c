/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_should_skip_dollar(char *input)
{
	return (*input == '$' && (*(input + 1) == ' ' || *(input + 1) == '\"' || *(input + 1) == '\0'));
}

char *ft_append_text(char *start, char *end, char *expanded_input)
{
	char *temp;

	temp = ft_substr(start, 0, end - start);
	expanded_input = ft_strjoin_free(expanded_input, temp);
	free(temp);
	return expanded_input;
}

int	ft_strlen_var(char *str)
{
	int i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return i;
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char *result;

	result = ft_strjoin(s1, s2);
	if (s1)
		free(s1);
	return result;
}

char *ft_get_env_value(char *key)
{
	char **env;
	char *env_key;
	int i;

	env = *ft_get_env();
	i = 0;
	while (env[i])
	{
		env_key = ft_get_key(env[i]);
		if (ft_strcmp(env_key, key) == 0)
		{
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	return (NULL);
}