/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/21 23:26:59 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char		*input;
	char		*x;
	t_token		**lexeme;
	t_ast_node	*ast;

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
				lexeme = (t_token **)malloc(sizeof(t_token *));
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
