/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/08 19:58:51 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pid_last_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		update_status_error(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		update_status_error(128 + WTERMSIG(status));  // Código de saída para terminação por sinal
	else
		update_status_error(1);  // Código de saída genérico
}

int	update_status_error(int exit_status)
{
	static int	status;

	if (exit_status != -1)
		status = exit_status;
	return (status);
}

int	main(void)
{
	char	*input;
	t_env	**env;
	t_token	**lexeme;
	t_ast_node *ast;
	char	*prompt;
	
	env = (t_env**)malloc(sizeof(t_env*));
	lexeme = (t_token**)malloc(sizeof(t_token*));
	if (!lexeme)
	{
		perror("Failed to allocate memory for lexeme");
		exit(EXIT_FAILURE);
	}
	*lexeme = NULL;
	while (1)
	{
		prompt = ft_get_prompt();
		input = readline(prompt);
		add_history(input);
		if (input)
		{
			input = ft_expand_variables_input(input);
			ft_tokenize(input, lexeme);
			//ft_print_linked_list(lexeme);
			ast = ft_build_ast(lexeme);
			ft_collect_heredocs(ast);
			ft_execute_ast(ast);
			ft_clean_token_list(lexeme);
			free(input);
			free(prompt);
			ft_free_ast(ast);
		}
	}
	free(lexeme);
	free(env);
	return (0);
}

int	get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}

int	*get_exit_status_env(void)
{
	static int	exit_status;

	return (&exit_status);
}
