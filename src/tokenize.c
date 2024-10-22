/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_state_command(char **input, t_state *state, \
char **i_token, int *type)
{
	if (ft_is_space(**input))
	{
		**i_token = '\0';
		*state = TOKEN_STATE_END;
		(*input)++;
	}
	else if (**input == '|' || (**input == '&' && *(*input + 1) == '&') \
	|| **input == '>' || **input == '<' )
		*state = TOKEN_STATE_END;
	else if (**input == '\'' || **input == '\"')
	{
		ft_handle_quotes(&input, &i_token, &type);
		*state = TOKEN_STATE_COMMAND;
	}
	else
		*(*i_token)++ = *(*input)++;
}

void	ft_last_token(char *current_token, t_token **lexeme, int *type)
{
	char	*last_char;

	last_char = NULL;
	last_char = ft_strchr(current_token, '\n');
	if (last_char != NULL)
		if (*(last_char + 1) == '\0' && *last_char == '\n')
			*last_char = '\0';
	ft_add_token(lexeme, ft_strdup(current_token), *type);
	free(current_token);
	free(type);
}

void	ft_tokenize(char *input, t_token **lexeme)
{
	t_state	state;
	char	*current_token;
	char	*i_token;
	int		*type;

	type = malloc(sizeof(int));
	state = TOKEN_STATE_START;
	current_token = ft_mem_token(input);
	i_token = current_token;
	while (*input)
	{
		if (state == TOKEN_STATE_START)
			ft_state_start(&input, &state, &i_token, type);
		else if (state == TOKEN_STATE_COMMAND)
			ft_state_command(&input, &state, &i_token, type);
		else
		{
			ft_add_token(lexeme, ft_strdup(current_token), *type);
			ft_reset_token_state(current_token, &i_token, &state);
		}
	}
	if (*current_token)
		ft_last_token(current_token, lexeme, type);
	else
		ft_free_token(current_token, type);
}

void	ft_reset_token_state(char *current_token, char **i_token, \
t_state *state)
{
	ft_memset(current_token, '\0', ft_strlen(current_token));
	*i_token = current_token;
	*state = TOKEN_STATE_START;
}

void	ft_add_token(t_token **lexeme, char *node, int type)
{
	t_token	*new_node;
	t_token	*current;

	new_node = ft_list_new_token();
	if (!new_node)
	{
		perror("Failed to allocate memory for new token");
		exit(EXIT_FAILURE);
	}
	new_node->token_node = node;
	new_node->type_token = type;
	if (*lexeme == NULL)
		*lexeme = new_node;
	else
	{
		current = *lexeme;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}
