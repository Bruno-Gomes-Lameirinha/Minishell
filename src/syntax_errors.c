/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:15 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 15:30:19 by livieira         ###   ########.fr       */
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
		fprintf(stderr, "Error: syntax error near unexpected token `newline`\n");
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
		fprintf(stderr, "Error: syntax error near unexpected token `||`\n");
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
		ft_printf(stderr, "Error: syntax error near unexpected token \
		`%c`\n", input[j]);
		return (1);
	}
	return (0);
}

int	ft_check_syntax_errors(const char *input)
{
	int	i = 0;
	int	single_quote_open = 0;
	int	double_quote_open = 0;

	while (input[i] != '\0')
	{
		i = ft_skip_quotes(input, i, &single_quote_open, &double_quote_open);
		if (!single_quote_open && !double_quote_open)
		{
			if (input[i] == '&' && input[i + 1] == '&')
			{
				fprintf(stderr, "Error: this bash does not handle and operator\n");
				return (1);
			}
			if (input[i] == '|')
			{
				if (ft_check_consecutive_pipes(input, i))
					return (1);
			}
			if (input[i] == '>' || input[i] == '<')
			{
				if (ft_check_redirection(input, i))
					return (1);
			}
			if (is_metachar(input[i]) && ft_check_consecutive_metachars(input, i))
				return (1);
		}
		i++;
	}
	return (0);
}
