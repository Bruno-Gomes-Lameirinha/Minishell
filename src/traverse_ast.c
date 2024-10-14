/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/08 18:52:20 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_execute_ast(t_ast_node *root, char *prompt)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (!root)
		return ;
	if (root->type == NODE_PIPE)
	{
		ft_handle_pipe(root);
		while (wait(NULL) > 0);
	}
	else if (root->type == NODE_COMMAND)
	{
		if (ft_handle_redirection(root->redirections, &saved_stdin, \
		&saved_stdout) == -1)
		{
			ft_restore_stdin_stdout(saved_stdin, saved_stdout);
			return ;
		}
		ft_execute_command(root, prompt);
		ft_restore_stdin_stdout(saved_stdin, saved_stdout);
	}
}

void	ft_execute_command(t_ast_node *root, char *prompt)
{
	if (!strcmp(root->value, "cd"))
		ft_cd_command_with_ast(root);
	else if (!strcmp(root->value, "echo"))
		ft_echo_command_with_ast(root);
	else if (!strcmp(root->value, "pwd"))
		ft_pwd_command(root);
	else if (!strcmp(root->value, "env"))
		ft_env_command(root);
	else if (!strcmp(root->value, "export"))
		ft_export_command(root);
	else if (!strcmp(root->value, "exit"))
		ft_exit_command(root, prompt);
	else if (!strcmp(root->value, "unset"))
		ft_unset_command(root);
	else
		ft_execute_command_ast(root);
}

void	ft_restore_stdin_stdout(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
