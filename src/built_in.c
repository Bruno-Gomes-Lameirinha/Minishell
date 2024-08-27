#include "../include/minishell.h"

void	ft_echo_command(t_token **cmd_tokens) 
{
    int         new_line;
    int         len;
    t_token    *current_token;

    new_line = 1;
    current_token = *cmd_tokens;

	current_token = current_token->next;
    if (current_token->next && !strcmp(current_token->token_node, "-n"))
	{
        new_line = 0;
		current_token = current_token->next;
	}

    while (current_token!= NULL)
    {
        len = strlen(current_token->token_node);
        write(1, current_token->token_node, len);
		if (current_token->next != NULL)
			write(1, " ", 1);
        current_token = current_token->next;
    }
    if (new_line)
        write(1, "\n", 1);

}