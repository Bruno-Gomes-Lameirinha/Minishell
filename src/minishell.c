# include "../include/minishell.h"

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
			free(input);
		}
	}
	free(env);
	return 0;
}