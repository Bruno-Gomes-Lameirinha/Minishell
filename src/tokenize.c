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

int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

t_token	*ft_list_new_token(void)
{
	t_token	*new_node;

	new_node = calloc(1, sizeof (t_token));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	new_node->token_node = NULL;
	return (new_node);
}

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
	{
		*(**i_token)++ = *(**input)++;
		if (***input == '|')
		{
			*(**i_token)++ = *(**input)++;
			**type = OR;
		}
		else
			**type = PIPE;
	}
	else if (***input == '&' && *(**input + 1) == '&')
	{
		*(**i_token)++ = *(**input)++;
		*(**i_token)++ = *(**input)++;
		**type = AND;
	}
	else if (***input == '>')
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
	else if (***input == '<')
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

char	*ft_mem_token(char *input)
{
	int		len;
	char	*memset_token;

	memset_token = NULL;
	len = ft_strlen(input);
	memset_token = (char *)ft_calloc(len + 1, sizeof(char *));
	if (!memset_token)
		return (NULL);
	return (memset_token);
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
		if (state == TOKEN_STATE_COMMAND)
			ft_state_command(&input, &state, &i_token, type);
		if (state == TOKEN_STATE_OPERATOR || state == TOKEN_STATE_END)
		{
			ft_add_token(lexeme, ft_strdup(current_token), *type);
			ft_memset(current_token, '\0', ft_strlen(current_token));
			i_token = current_token;
			state = TOKEN_STATE_START;
		}
	}
	if (*current_token)
		ft_last_token(current_token, lexeme, type);
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

void	ft_clean_token_list(t_token **lst)
{
	t_token	*node_to_del;
	t_token	*current;

	if (*lst == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		node_to_del = current;
		current = current->next;
		free(node_to_del->token_node);
		free (node_to_del);
	}
	*lst = NULL;
}
