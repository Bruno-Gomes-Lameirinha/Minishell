/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd_command_with_ast(t_ast_node *node_ast)
{
	t_ast_node	*current_token;
	char		*path;
	char		*pwd;

	current_token = node_ast;
	if (current_token->right == NULL && !strcmp(current_token->value, "cd"))
		path = getenv("HOME");
	else if (current_token->right && current_token->right->right == NULL)
		path = current_token->right->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		ft_update_status_error(1);
		return ;
	}
	else
		ft_update_status_error(0);
	pwd = getcwd(NULL, 1024);
	if (pwd)
		free(pwd);
	else
	{
		perror("getcwd");
		ft_update_status_error(1);
	}
}
