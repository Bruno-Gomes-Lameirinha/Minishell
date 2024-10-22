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

void	process_input(char *input)
{
	t_token		**lexeme;
	t_ast_node	*ast;
	char		*x;

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
	free(input);
	ast = ft_build_ast(lexeme);
	ft_clean_token_list(lexeme);
	ft_set_head_lst(ast);
	ft_collect_heredocs(ast);
	ft_execute_ast(ast);
	ft_free_ast(ast->head);
}

char	*get_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
		ft_handle_eof();
	add_history(input);
	return (input);
}

int	main(void)
{
	char	*input;

	ft_setup_signal_handlers();
	while (1)
	{
		input = get_input();
		if (*input != '\0')
			process_input(input);
	}
	return (0);
}

void	ft_set_head_lst(t_ast_node *root)
{
	t_ast_node	*head;
	t_ast_node	*current;

	if (!root)
		return ;
	head = root;
	current = root;
	current->head = root;
	if (current->right)
		current = current->right;
	while (current)
	{
		current->head = head;
		current = current->right;
	}
	current = head;
	if (current->left)
		current = current->left;
	while (current)
	{
		current->head = head;
		current = current->left;
	}
}
