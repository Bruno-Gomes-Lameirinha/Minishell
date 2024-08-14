# include "../include/minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	
	char *input;
	int i;

	if (argc == 1 && argv[0] != NULL)
	{
	i = 0;

	while(i < 3)
	{
		input = readline("$");
		if (input) 
		{
			printf("Você digitou: %s\n", input);
			free(input);
			i++;
		}
	}

    while (envp[i] != NULL) {
		if(!strncmp((envp[i]), "OLDPWD", 6))
        	printf("%s\n", envp[i]);
        i++;
    }
	}
    return 0;
}