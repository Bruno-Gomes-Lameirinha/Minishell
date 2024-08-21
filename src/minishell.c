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


void	ft_tokenize(char *input, t_token **lexeme)
{
	State state = TOKEN_STATE_START;
	char	current_token[128] = "";
	int i;
	
	i = 0;
	while(*input)
	{
		if (state == TOKEN_STATE_START)
		{
		if (ft_is_space(*input))
			input++;
		else if (*input == '|' || *input == '"' || *input == '&'|| *input == '>'\
		|| *input == '<')
		{
			state = TOKEN_STATE_OPERATOR;
			current_token[i++] = *input++;
			if (*input == '&' || *input == '>' || *input == '<' ) 
				current_token[i++] = *input++;
		}
		else if (*input == '\'')
		{
			input++;
			while (*input && *input != '\'')
				current_token[i++] = *input++;
			if (*input == '\'')
				input++;
			state = TOKEN_STATE_COMMAND;
		}
		else
		{
			state = TOKEN_STATE_COMMAND;
			current_token[i++] = *input++;
		}
		}
		if (state == TOKEN_STATE_COMMAND)
		{
			if (ft_is_space(*input))
			{
				current_token[i] = '\0';
				ft_add_token(lexeme, ft_strdup(current_token));
				i = 0;
				state = TOKEN_STATE_START;
				input++;
			}
			else if (*input == '|' || *input == '"' || *input == '&'|| *input == '>'\
			|| *input == '<' )
			{
				current_token[i] = '\0';
				ft_add_token(lexeme, ft_strdup(current_token));
				i= 0;
				state = TOKEN_STATE_OPERATOR;
			}
			else if (*input == '\'')
			{
				input++;
				while (*input && *input != '\'')
					current_token[i++] = *input++;
				if (*input == '\'')
					input++;
			}
			else
				current_token[i++] = *input++;
		}
		if (state == TOKEN_STATE_OPERATOR)
		{
			current_token[i] = '\0';
			ft_add_token(lexeme, ft_strdup(current_token));
			i = 0;
			state = TOKEN_STATE_START;
		}
	}
	if (i > 0) 
	{
    	current_token[i] = '\0';
  		ft_add_token(lexeme, ft_strdup(current_token));
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