/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/08 20:24:32 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_expand_variables_input(char *input)
{
	char	*start;
	char	*var_key;
	char	*var_value;
	char 	*expanded_input;
	char 	*temp;

	start = input;
	expanded_input = ft_strdup("");
	while (*input)
	{
		while (*input != '$' && *input != '\'' && *input != '\0')
			input++;
		if (*input == '\'')
		{
			input++;
			while (*input != '\'' && *input != '\0')
				input++;
			if (*input == '\'')
				input++;
			continue;
		}
		if (*input == '$')
		{
			temp = ft_substr(start, 0, input - start);
			expanded_input = ft_strjoin_free(expanded_input, temp);
			input++;
			var_key = ft_substr(input, 0, ft_strlen_var(input));
			var_value = ft_get_env_value(var_key);
			if (var_value)
    			expanded_input = ft_strjoin_free(expanded_input, var_value);
			else
    			expanded_input = ft_strjoin_free(expanded_input, "");
			free(var_key);
			input += ft_strlen_var(input);
			start = input;
		}
	}
	if (*start != '\0')
	{
		temp = ft_strdup(start);
		expanded_input = ft_strjoin_free(expanded_input, temp);
		free(temp);
	}
	return expanded_input;
}

int	ft_strlen_var(char *str)
{
	int i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))  // Variáveis podem ter letras, números e '_'
		i++;
	return i;
}

char *ft_strjoin_free(char *s1, char *s2)
{
	char *result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return result;
}

char *ft_get_env_value(char *key)
{
    char **env;
    char *env_key;
    int i;

    env = *ft_get_env(); // Usa o ambiente clonado
    i = 0;
    while (env[i])
    {
        env_key = get_key(env[i]); // Extrai a chave da variável do formato "key=value"
        if (ft_strcmp(env_key, key) == 0) // Compara a chave com a variável procurada
        {
            return (ft_strchr(env[i], '=') + 1); // Retorna o valor após o '='
        }
        i++;
    }
    return (NULL); // Se não encontrar, retorna NULL
}
