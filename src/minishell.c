# include "../include/minishell.h"

int main()
{
	char *input;
	int i;
	t_env	*env;

	env = (t_env*)malloc(sizeof(t_env));
	i = 0;
	env->pwd = getenv("PWD");
	printf("pwd: %s\n", env->pwd);
	while(i < 3)
	{
		input = readline("$");
		add_history(input);
		if (input) 
		{
			printf("Você digitou: %s\n", input);
			if(strncmp(input, "cd", 2))
			{
				env->old_pwd = getenv("PWD");
				if (chdir("/home/bruno/42sp/minishell/include") == 0)
					env->pwd = getcwd(NULL, 0);
			}
			else
				perror("chdir");
			free(input);
			i++;
		}
	}
	env->pwd = getcwd(NULL, 0);
	printf("pwd: %s\n", env->pwd); 
	
    return 0;
}