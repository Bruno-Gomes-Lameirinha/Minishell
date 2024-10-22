/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
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
			(**input)++;
		**type = SINGLE_QUOTES;
	}
	else if (***input == '\"')
	{
		(**input)++;
		while (***input && ***input != '\"')
			*(**i_token)++ = *(**input)++;
		if (***input == '\"')
			(**input)++;
		**type = DOUBLE_QUOTES;
	}
}
