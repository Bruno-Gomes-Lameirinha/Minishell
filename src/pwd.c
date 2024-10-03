/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:02:24 by livieira          #+#    #+#             */
/*   Updated: 2024/09/03 14:32:45 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pwd_command(t_ast_node *cmd_tokens)
{
	t_ast_node	*current_token;
	char		*path;

	current_token = cmd_tokens;
	path = NULL;
	if (strcmp(current_token->value, "pwd") == 0)
	{
		path = getcwd(NULL, 1024);
		if (path == NULL)
			perror("pwd");
		else
			printf("%s\n", path);
		free(path);
	}
}
