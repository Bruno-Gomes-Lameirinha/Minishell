/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/14 15:04:30 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_expand_variables_input(char *input)
{
	char	*start;
	char	*expanded_input;

	start = input;
	expanded_input = ft_strdup("");
	while (*input)
	{
		input = ft_skip_until_special_char(input);
		if (*input == '\0')
			break ;
		if (ft_handle_special_char(&input, &start, &expanded_input))
			continue ;
	}
	if (*start != '\0')
		expanded_input = ft_append_text(start, input, expanded_input);
	return (expanded_input);
}

char	*ft_skip_until_special_char(char *input)
{
	while (*input != '$' && *input != '\'' && *input != '\0')
		input++;
	return (input);
}

int	ft_handle_special_char(char **input, char **start, char **expanded_input)
{
	if (**input == '\'')
	{
		*input = ft_skip_single_quotes(*input);
		return (1);
	}
	if (ft_should_skip_dollar(*input))
	{
		(*input)++;
		return (1);
	}
	if (**input == '$')
	{
		*expanded_input = ft_append_text(*start, *input, *expanded_input);
		(*input)++;
		*input = ft_process_variable(*input, expanded_input);
		*start = *input;
		return (1);
	}
	return (0);
}

char	*ft_skip_single_quotes(char *input)
{
	input++;
	while (*input != '\'' && *input != '\0')
		input++;
	if (*input == '\'')
		input++;
	return (input);
}

char	*ft_process_variable(char *input, char **expanded_input)
{
	char	*var_key;
	char	*var_value;
	int		var_len;

	if (*input == '?')
	{
		var_value = ft_itoa(ft_update_status_error(-1));
		*expanded_input = ft_strjoin_free(*expanded_input, var_value);
		free(var_value);
		input++;
	}
	else
	{
		var_len = ft_strlen_var(input);
		var_key = ft_substr(input, 0, var_len);
		var_value = ft_get_env_value(var_key);
		if (var_value)
			*expanded_input = ft_strjoin_free(*expanded_input, var_value);
		free(var_key);
		input += var_len;
	}
	return (input);
}
