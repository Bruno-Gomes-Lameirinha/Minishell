# include "../include/minishell.h"

int main() {
    char *input;
    int i = 0;
    while(i<3)
    {
        input = readline("$");
        if (input) 
        {
            printf("Você digitou: %s\n", input);
            free(input);
            i++;
        }
    }
    return(0);
}