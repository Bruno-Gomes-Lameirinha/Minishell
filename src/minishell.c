# include "../include/minishell.h"


 int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

void	ft_tokenize(char *input)
{
	State state = TOKEN_STATE_START;
	char	current_token[128] = "";;
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
			current_token[i] = *input;
			i++;
			input++;
			if (*input == '&' || *input == '>' || *input == '<' ) 
			{
				current_token[i++] = *input;
				input++;
			}
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
			current_token[i] = *input;
			i++;
			input++;
		}
		}
		if (state == TOKEN_STATE_COMMAND)
		{
			if (ft_is_space(*input))
			{
				current_token[i] = '\0';
				int j = 0;
				printf("o token é:");
				while (current_token[j] != '\0')
				{
					printf("%c", current_token[j]);
					j++;
				}
				printf("\n");
				j = 0;
				// adicionar um nó na lista ligada;
				// usar strdup no char token para passar ele no heap;
				// t_token->token_node = a string duplicada pelo strdup
				i = 0;
				state = TOKEN_STATE_START;
				input++;
			}
			else if (*input == '|' || *input == '"' || *input == '&'|| *input == '>'\
			|| *input == '<' )
			{
				current_token[i] = '\0';
				printf("o token é:");
				int j = 0;
				while (current_token[j] != '\0')
				{
					printf("%c", current_token[j]);
					j++;
				}
				printf("\n");
				j = 0;
				// adicionar o token a lista ligada 
				// a mudança para State Operator vai fazer com que o metacaractere seja processado;
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
			{
				current_token[i] = *input;
				i++;
				input++;
			}
		}
		if (state == TOKEN_STATE_OPERATOR)
		{
			current_token[i] = '\0';
			printf("o token é:");
			int j = 0;
			while (current_token[j] != '\0')
			{
				printf("%c", current_token[j]);
				j++;
			}
			printf("\n");
			j = 0;
			//incluir o token na lista ligada
			i = 0;
			state = TOKEN_STATE_START;
		}
	}
	if (i > 0) 
	{
    current_token[i] = '\0';
  	//incluir o token na lista ligada
	printf("o token é:");
	int j = 0;
	while (current_token[j] != '\0')
	{
		printf("%c", current_token[j]);
		j++;
	}
	printf("\n");
	j = 0;
    }
	
}

int main()
{
	char *input;
	t_env	*env;

	env = (t_env*)malloc(sizeof(t_env));
	while(1)
	{
		input = readline("Minishell$ ");
		add_history(input);
		if (input) 
		{
			printf("Você digitou: %s\n", input);
			ft_tokenize(input);
			free(input);
		}
	}
	free(env);
	return 0;
}
