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
	char	token[128];
	int i;
	
	
	i = 0;
	while(*input)
	{
		if (state = STATE_START)
		if (ft_is_space(*input))
			input++;
		else if (ft_isalpha(*input) || ft_isdigit(*input))
		{
			state = STATE_COMMAND;
			token[i] = *input;
			i++;
		}
		else if (*input == '|')
		{
			state = STATE_OPERATOR;
		}
		if (state == STATE_COMMAND)
		{
			if (ft_is_space(*input))
			{
				token[i] = '\0';
				// adicionar um nó na lista ligada;
				// usar strdup no char token para passar ele no heap;
				// t_token->token_node = a string duplicada pelo strdup
				//trocar state para START_STATE
			}
			else if (ft_isalpha(*input) || ft_isdigit(*input))
			{
				token[i] = *input;
				i++;
			}
			else if (*input == '|' || '"' || '&')
			{
				state = STATE_OPERATOR;
			}

		}
		if (state == STATE_COMMAND)
		{
			
		}
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
