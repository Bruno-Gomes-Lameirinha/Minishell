/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:05:53 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 18:16:44 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt --suppressions=.suppress_readline_error.sup ./minishell

void	ft_cd_command_with_ast(t_ast_node *node_ast)
{
	t_ast_node	*current_token;
	char	*path;
	char	*pwd;

	current_token = node_ast;
	if (current_token->right == NULL && !strcmp(current_token->value, "cd"))
		path = getenv("HOME");
	else if (current_token->right && current_token->right->right == NULL)
		path = current_token->right->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		ft_update_status_error(1);
		return;
	}
	else
		ft_update_status_error(0);
	pwd  = getcwd(NULL, 1024);
	if (pwd)	
		free(pwd);
	else
	{
		perror("getcwd");
		ft_update_status_error(1);
	}
}

void ft_echo_command_with_ast(t_ast_node *node)
{
	int			new_line;
	t_ast_node	*current;

	if (!node) 
	{
		ft_update_status_error(1);
		return;
	}
	new_line = 1;
	current = node->right;
	if (current && !strcmp(current->value, "-n")) 
	{
		new_line = 0;
		current = current->right;
	}
	while (current)
	{
		if (write(1, current->value, strlen(current->value)) == -1)
		{
			perror("write");
			ft_update_status_error(1);
			return;
		}
		if (current->right)
			if (write(1, " ", 1) == -1)
			{
				perror("write");
				ft_update_status_error(1);
				return;
			}
		current = current->right;

	}
	if (new_line)
	{
		if (write(1, "\n", 1) == -1)
		{
			perror("write");
			ft_update_status_error(1);
			return;
		}
	}
	ft_update_status_error(0);
}
