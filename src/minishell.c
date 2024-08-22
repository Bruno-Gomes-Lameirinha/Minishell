/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/15 17:11:33 by bgomes-l         ###   ########.fr       */
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

void	ft_state_start(char **input, State *state, char **current_token)
{
	if (ft_is_space(**input))
			(*input)++;
	else if (**input == '|' || **input == '"' || **input == '&' \
	|| **input == '>' || **input == '<')
	{
		*state = TOKEN_STATE_OPERATOR;
		*(*current_token)++ = *(*input)++;
		if ((**input == '&' && *(*input - 1) == '&') || (**input == '>' && \
		*(*input - 1) == '>') || (**input == '<' && *(*input - 1) == '<'))
			*(*current_token)++ = *(*input)++;
	}
	else if (**input == '\'')
	{
		(*input)++;
		while (**input && **input != '\'')
			*(*current_token)++ = *(*input)++;
		if (**input == '\'')
			(*input)++;
		*state = TOKEN_STATE_COMMAND;
	}
	else
	{
		*state = TOKEN_STATE_COMMAND;
		*(*current_token)++ = *(*input)++;
	}
}

void	ft_state_command(char **input, State *state, char **current_token)
{
	if (ft_is_space(**input))
	{
		**current_token = '\0';
		*state = TOKEN_STATE_END;
		(*input)++;
	}
	else if (**input == '|' || **input == '"' || **input == '&' \
	|| **input == '>' || **input == '<' )
	{
		**current_token = '\0';
		*state = TOKEN_STATE_END;
	}
	else if (**input == '\'')
	{
		(*input)++;
		while (**input && **input != '\'')
			*(*current_token)++ = *(*input)++;
		if (**input == '\'')
			(*input)++;
	}
	else
		*(*current_token)++ = *(*input)++;
}

char	*ft_mem_token(char *input)
{
	int		len;
	char	*memset_token;

	memset_token = NULL;
	len = ft_strlen(input);
	memset_token = (char *)ft_calloc(len + 1, sizeof(char *));
	if (!memset_token)
		return (NULL); // fazer tratativa de erros dos tokens
	return (memset_token);
}

void	ft_last_token(char *current_token, t_token **lexeme)
{
	ft_add_token(lexeme, ft_strdup(current_token));
	free(current_token);
}

void	ft_tokenize(char *input, t_token **lexeme)
{
	State	state;
	char	*current_token;
	char	*token_index;

	state = TOKEN_STATE_START;
	current_token = ft_mem_token(input);
	token_index = current_token;
	while (*input)
	{
		if (state == TOKEN_STATE_START)
			ft_state_start(&input, &state, &token_index);
		if (state == TOKEN_STATE_COMMAND)
			ft_state_command(&input, &state, &token_index);
		if (state == TOKEN_STATE_OPERATOR || state == TOKEN_STATE_END)
		{
			ft_add_token(lexeme, ft_strdup(current_token));
			ft_memset(current_token, '\0', ft_strlen(current_token));
			token_index = current_token;
			state = TOKEN_STATE_START;
		}
	}
	if (*current_token)
		ft_last_token(current_token, lexeme);
}

int	main()
{
	char	*input;
	t_env	*env;
	t_token	**lexeme;
	int		i = 0;

	env = (t_env*)malloc(sizeof(t_env));
	lexeme = (t_token**)malloc(sizeof(t_token*));
	if (!lexeme)
	{
		perror("Failed to allocate memory for lexeme");
		exit(EXIT_FAILURE);
	}
	*lexeme = NULL;
	while (i < 3)
	{
		input = readline("Minishell$ ");
		add_history(input);
		if (input)
		{
			printf("Você digitou: %s\n", input);
			ft_tokenize(input, lexeme);
			ft_print_linked_list(lexeme);
			ft_clean_token_list(lexeme);
			free(input);
			i++;
		}
	}
	free(lexeme);
	free(env);
	return (0);
}

void	ft_add_token(t_token **lexeme, char *node)
{
	t_token	*new_node;
	t_token *current;

	new_node = ft_list_new_token();
	if (!new_node)
	{
		perror("Failed to allocate memory for new token");
		exit(EXIT_FAILURE);
	}
	new_node->token_node = node;
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

void	ft_print_linked_list(t_token **lexeme)
{
	t_token	*current;

	current = *lexeme;
	while (current != NULL)
	{
		printf("%s\n", current->token_node);
		current = current->next;
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
