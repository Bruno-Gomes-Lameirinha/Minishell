# include "../include/minishell.h"

int main()
{
	char *input;
	t_env	*env;
	int i = 0;

	env = (t_env*)malloc(sizeof(t_env));
	i = 0;
	env->pwd = getenv("PWD");
	printf("pwd: %s\n", env->pwd);
	while(1)
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
