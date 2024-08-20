# include "../include/minishell.h"


 int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

void	ft_tokenize(char *input)
{
	State state = STATE_START;
	char	token[128] = "";;
	int i;
	
	i = 0;
	while(*input)
	{
		if (state == STATE_START)
		{
		if (ft_is_space(*input))
			input++;
		else if (ft_isalpha(*input) || ft_isdigit(*input))
		{
			state = STATE_COMMAND;
			token[i] = *input;
			i++;
			input++;
		}
		else if (*input == '|' || *input == '"' || *input == '&')
		{
			state = STATE_OPERATOR;
			input++;
		}
		}
		if (state == STATE_COMMAND)
		{
			if (ft_is_space(*input))
			{
				token[i] = '\0';
				int j = 0;
				printf("o token é:");
				while (token[j] != '\0')
				{
					printf("%c", token[j]);
					j++;
				}
				printf("\n");
				j = 0;
				// adicionar um nó na lista ligada;
				// usar strdup no char token para passar ele no heap;
				// t_token->token_node = a string duplicada pelo strdup
				i = 0;
				state = STATE_START;
				input++;
			}
			else if (*input == '|' || *input == '"' || *input == '&')
			{
				token[i] = '\0';
				printf("o token é:");
				int j = 0;
				while (token[j] != '\0')
				{
					printf("%c", token[j]);
					j++;
				}
				printf("\n");
				j = 0;
				// adicionar o token a lista ligada 
				// a mudança para State Operator vai fazer com que o metacaractere seja processado;
				i= 0;
				state = STATE_OPERATOR;
			}
			else
			{
				token[i] = *input;
				i++;
				input++;
			}
		}
		if (state == STATE_OPERATOR)
		{
			token[i] = '\0';
			printf("o token é:");
			int j = 0;
			while (token[j] != '\0')
			{
				printf("%c", token[j]);
				j++;
			}
			printf("\n");
			j = 0;
			//incluir o token na lista ligada
			i = 0;
			state = STATE_START;
		}
	}
	if (i > 0) 
	{
    token[i] = '\0';
  	//incluir o token na lista ligada
	printf("o token é:");
	int j = 0;
	while (token[j] != '\0')
	{
		printf("%c", token[j]);
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
