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

void	ft_state_start(char **input, t_state *state, char **i_token, int *type)
{
	if (ft_is_space(**input))
			(*input)++;
	else if (**input == '|' || (**input == '&' && *(*input + 1) == '&') \
	|| **input == '>' || **input == '<')
	{
		ft_handle_operators(&input, &i_token, &type);
		*state = TOKEN_STATE_OPERATOR;
	}
	else if (**input == '\'' || **input == '\"')
	{
		ft_handle_quotes(&input, &i_token, &type);
		*state = TOKEN_STATE_COMMAND;
	}
	else
	{
		*state = TOKEN_STATE_COMMAND;
		*(*i_token)++ = *(*input)++;
		*type = WORD;
	}
}

void	ft_handle_operators(char ***input, char ***i_token, int **type)
{
	if (***input == '|')
		ft_handle_pipe_operator(input, i_token, type);
	else if (***input == '&' && *(**input + 1) == '&')
		ft_handle_and_operator(input, i_token, type);
	else if (***input == '>')
		ft_handle_redirect_out_operator(input, i_token, type);
	else if (***input == '<')
		ft_handle_redirect_in_operator(input, i_token, type);
}

void	ft_handle_pipe_operator(char ***input, char ***i_token, int **type)
{
	*(**i_token)++ = *(**input)++;
	if (***input == '|')
	{
		*(**i_token)++ = *(**input)++;
		**type = OR;
		//dar mensagem de erro e liberar a lista de tokens e retornar a readline;
	}
	else
		**type = PIPE;
}

void	ft_handle_and_operator(char ***input, char ***i_token, int **type)
{
	*(**i_token)++ = *(**input)++;
	*(**i_token)++ = *(**input)++;
	**type = AND;
}

void	ft_handle_redirect_out_operator(char ***input, \
char ***i_token, int **type)
{
	*(**i_token)++ = *(**input)++;
	if (***input == '>')
	{
		*(**i_token)++ = *(**input)++;
		**type = R_OUTAPP;
	}
	else
		**type = R_OUT;
}
