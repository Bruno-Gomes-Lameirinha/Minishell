/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/09 23:27:20 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pid_last_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ft_update_status_error(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		ft_update_status_error(128 + WTERMSIG(status));  // Código de saída para terminação por sinal
	else
		ft_update_status_error(1);  // Código de saída genérico
}

int	ft_update_status_error(int exit_status)
{
	static int	status;

	if (exit_status != -1)
		status = exit_status;
	return (status);
}

int	main(void)
{
	char	*input;
	t_token	**lexeme;
	t_ast_node *ast;
	char *x;
	
	x = NULL;
	ft_setup_signal_handlers();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			ft_handle_eof();
		add_history(input);
		if (input)
		{
			if (*input != '\0')
			{
				lexeme = (t_token**)malloc(sizeof(t_token*));
				if (!lexeme)
				{
					perror("Failed to allocate memory for lexeme");
					exit(EXIT_FAILURE);
				}
				*lexeme = NULL;
				x = ft_strchr(input, '$');
				if (x != NULL)
					input = ft_expand_variables_input(input);
				ft_tokenize(input, lexeme);
				free (input);
				ast = ft_build_ast(lexeme);
				ft_clean_token_list(lexeme);
				ft_set_head_lst(ast);
				ft_collect_heredocs(ast);
				ft_execute_ast(ast);
				ft_free_ast(ast->head);
			}
		}
	}
	return (0);
}

int	ft_get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}

int	*ft_get_exit_status_env(void)
{
	static int	exit_status;

	return (&exit_status);
}

void	ft_set_head_lst(t_ast_node *root)
{
	if (!root)
		return;
	
	t_ast_node *head;
	t_ast_node *current;

	head = root;
	current = root;

	current->head = root;
	if (current->right)
		current = current->right;
	while(current)
	{
		current->head = head;
		current = current->right;
	}
	current = head;
	if (current->left)
		current = current->left;
	while(current)
	{
		current->head = head;
		current = current->left;
	}
}
