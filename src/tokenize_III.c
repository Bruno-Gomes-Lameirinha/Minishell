/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_III.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/22 00:24:29 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_handle_redirect_in_operator(char ***input, char ***i_token, \
int **type)
{
	*(**i_token)++ = *(**input)++;
	if (***input == '<')
	{
		*(**i_token)++ = *(**input)++;
		**type = R_HDOC;
	}
	else
		**type = R_IN;
}

void	ft_handle_quotes(char ***input, char ***i_token, int **type)
{
	if (***input == '\'')
	{
		(**input)++;
		while (***input && ***input != '\'')
			*(**i_token)++ = *(**input)++;
		if (***input == '\'')
		{
			(**input)++;
			**type = SINGLE_QUOTES;
		}
		else
		{
			*type = UNCLOSED_QUOTES;
		}
	}
	else if (***input == '\"')
	{
		(**input)++;
		while (***input && ***input != '\"')
			*(**i_token)++ = *(**input)++;
		if (***input == '\"')
		{
			(**input)++;
			**type = DOUBLE_QUOTES;
		}
		else
		{
			*type = UNCLOSED_QUOTES;
		}
	}
}
