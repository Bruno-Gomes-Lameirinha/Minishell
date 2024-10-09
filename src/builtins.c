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

/* #include "../include/minishell.h"

void	ft_cd_command_with_ast(t_ast_node *node_ast)
{
	t_ast_node	*current_token;
	char	*path;
	char	*pwd;

	current_token = node_ast;
	if (current_token->right == NULL && !strcmp(current_token->value, "cd"))
		path = getenv("HOME");
	else if (current_token->right)
		path = current_token->right->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		return;
	}
	pwd  = getcwd(NULL, 1024);
	free(pwd);
}

void ft_echo_command_with_ast(t_ast_node *node) 
{
	if (!node) return;

	int new_line = 1;
	t_ast_node *current = node->right;

	if (current && !strcmp(current->value, "-n")) 
	{
		new_line = 0;
		current = current->right;
	}

	while (current)
	{
		write(1, current->value, strlen(current->value));
		if (current->right)
			write(1, " ", 1);
		current = current->right;
	}

	if (new_line)
		write(1, "\n", 1);
}

void	ft_pwd_command(t_ast_node *cmd_tokens)
{
    t_ast_node *current_token;
    char    *path;

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
 */

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
	else if (current_token->right)
		path = current_token->right->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		return;
	}
	pwd  = getcwd(NULL, 1024);
	free(pwd);
}

void ft_echo_command_with_ast(t_ast_node *node)
{
	if (!node) return;

	int new_line = 1;
	t_ast_node *current = node->right;

	if (current && !strcmp(current->value, "-n")) 
	{
		new_line = 0;
		current = current->right;
	}

	while (current)
	{
		write(1, current->value, strlen(current->value));
		if (current->right)
			write(1, " ", 1);
		current = current->right;
	}

	if (new_line)
		write(1, "\n", 1);
}