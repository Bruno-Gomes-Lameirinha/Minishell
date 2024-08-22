# include "../include/minishell.h"


 int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

t_token	*ft_list_new_token(void)
{
	t_token	*new_node;

	new_node = calloc(1 , sizeof (t_token));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	new_node->token_node = NULL;
	return (new_node);
}

void ft_state_start(char **input, State *state, char **current_token)
{
	if (ft_is_space(**input))
			(*input)++;
		else if (**input == '|' || **input == '"' || **input == '&'|| **input == '>'\
		|| **input == '<')
		{
			*state = TOKEN_STATE_OPERATOR;
			**current_token = **input;
			(*current_token)++;
			(*input)++;
			if (**input == '&' || **input == '>' || **input == '<' ) 
				*(*current_token)++ = *(*input)++;
		}
		else if (**input == '\'')
		{
			(*input)++;
			while (**input && **input != '\'')
			{
				**current_token = **input;
				(*current_token)++;
				(*input)++;
			}
			if (**input == '\'')
				(*input)++;
			*state = TOKEN_STATE_COMMAND;
		}
		else
		{
			*state = TOKEN_STATE_COMMAND;
			**current_token = **input;
			(*current_token)++;
			(*input)++;
		}
}

void ft_state_command(char **input, State *state, char **current_token)
{
	if (ft_is_space(**input))
	{
		**current_token = '\0';
		*state = TOKEN_STATE_END;
		(*input)++;
	}
	else if (**input == '|' || **input == '"' || **input == '&'|| **input == '>'\
	|| **input == '<' )
	{
		**current_token = '\0';
		*state = TOKEN_STATE_END;
	}
	else if (**input == '\'')
	{
		(*input)++;
		while (**input && **input != '\'')
		{	
			**current_token = **input;
			(*current_token)++;
			(*input)++;
		}
		if (**input == '\'')
			(*input)++;
	}
	else
	{
		**current_token = **input;
		(*current_token)++;
		(*input)++;
	}
}

char	*ft_mem_token(char *input)
{
	int 	len;
	char 	*memset_token;

	len = ft_strlen(input);
	memset_token = (char *)ft_calloc(len + 1, sizeof(char *));
	if (!memset_token)
		return NULL; // fazer tratativa de erros dos tokens
	return (memset_token);

}

void	ft_tokenize(char *input, t_token **lexeme)
{
	State	state;
	char	*current_token;
	char 	*token_index;
	
	state = TOKEN_STATE_START;
	current_token = ft_mem_token(input);
	token_index = current_token;
	while(*input)
	{
		if (state == TOKEN_STATE_START)
			ft_state_start(&input, &state, &token_index);
		if (state == TOKEN_STATE_COMMAND)
			ft_state_command(&input, &state, &token_index);
		if (state == TOKEN_STATE_OPERATOR)
		{
			*token_index = '\0';
			ft_add_token(lexeme, ft_strdup(current_token));
			ft_memset(current_token, '\0', ft_strlen(input));
			token_index = current_token;
			state = TOKEN_STATE_START;
		}
		if (state == TOKEN_STATE_END)
		{
			ft_add_token(lexeme, ft_strdup(current_token));
			ft_memset(current_token, '\0', ft_strlen(input));
			token_index = current_token;
			state = TOKEN_STATE_START;
		}
	}
	if (*current_token) 
	{
    	*token_index = '\0'; // me pare dispensavel pois o menset ja fez isso
  		ft_add_token(lexeme, ft_strdup(current_token));
		free(current_token);
    }
}

int main()
{
	char *input;
	t_env	*env;
	t_token **lexeme;

	env = (t_env*)malloc(sizeof(t_env));
	lexeme = (t_token**)malloc(sizeof(t_token));
	while(1)
	{
		// precisa liberar o lexeme antes da proxima readline
		input = readline("Minishell$ ");
		add_history(input);
		if (input) 
		{
			printf("Você digitou: %s\n", input);
			ft_tokenize(input, lexeme);
			ft_print_linked_list(lexeme);
			free(input);
		}
	}
	free(env);
	return 0;
}

void	ft_add_token(t_token **lexeme, char *node)
{
	t_token	*new_node;
	t_token *current;
	new_node = ft_list_new_token();
	new_node->token_node = node;
	current = *lexeme;
	if (*lexeme != NULL)
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	else
		*lexeme = new_node;
}

void	ft_print_linked_list(t_token **lexeme)
{
	t_token *current;

	current = *lexeme;
	while (current != NULL)
	{
		printf("%s\n", current->token_node);
		current = current->next;
	}
}
