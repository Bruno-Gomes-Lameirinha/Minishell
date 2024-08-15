# include "../include/minishell.h"

int main()
{
	char *input;
	t_env	*env;
	int i = 0;

	env = (t_env*)malloc(sizeof(t_env));
	while(i <3)
	{
		input = readline("Minishell$ ");
		add_history(input);
		if (input) 
		{
			printf("Você digitou: %s\n", input);
			free(input);
			i++;
		}
	}
	free(env);
	return 0;
}
