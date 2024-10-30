/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors_II.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:15 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 15:44:50 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_perform_syntax_checks(const char *input, int i)
{
	if (input[i] == '&' && input[i + 1] == '&')
	{
		ft_printf(2, "Error: this bash does not handle and operator\n");
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
	return (0);
}

int	ft_check_syntax_errors(const char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i] != '\0')
	{
		i = ft_skip_quotes(input, i, &single_quote_open, &double_quote_open);
		if (!single_quote_open && !double_quote_open)
		{
			if (ft_perform_syntax_checks(input, i))
				return (1);
		}
		i++;
	}
	return (0);
}
