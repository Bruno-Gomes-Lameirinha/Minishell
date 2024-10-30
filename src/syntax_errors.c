/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:15 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 15:47:53 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_metachar(char c)
{
	return (c == '|' || c == '&' || c == '>' || c == '<');
}

int	ft_skip_quotes(const char *input, int i, int *single_quote_open, \
int *double_quote_open)
{
	if (input[i] == '\'' && *double_quote_open == 0)
		*single_quote_open = !(*single_quote_open);
	else if (input[i] == '\"' && *single_quote_open == 0)
		*double_quote_open = !(*double_quote_open);
	return (i);
}

int	ft_check_redirection(const char *input, int i)
{
	int	j;

	j = i + 1;
	if (input[j] == input[i])
		j++;
	while (input[j] == ' ')
		j++;
	if (input[j] == '\0' || is_metachar(input[j]))
	{
		ft_printf(2, "Error: syntax error near unexpected token `newline`\n");
		return (1);
	}
	return (0);
}

int	ft_check_consecutive_pipes(const char *input, int i)
{
	int	j;

	j = i + 1;
	while (input[j] == ' ')
		j++;
	if (input[j] == '|')
	{
		ft_printf(2, "Error: syntax error near unexpected token `||`\n");
		return (1);
	}
	return (0);
}

int	ft_check_consecutive_metachars(const char *input, int i)
{
	int	j;

	j = i + 1;
	while (input[j] == ' ')
		j++;
	if (is_metachar(input[j]) && !(input[i] == '&' && input[j] == '&'))
	{
		if ((input[i] == '>' && input[j] == '>') || (input[i] == '<' \
		&& input[j] == '<'))
			return (0);
		if (input[i] == '|' && (input[j] == '>' || input[j] == '<'))
			return (0);
		ft_printf(2, "Error: syntax error near unexpected token \
		`%c`\n", input[j]);
		return (1);
	}
	return (0);
}
