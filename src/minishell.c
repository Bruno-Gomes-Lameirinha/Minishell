/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 14:41:29 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_input(char *input)
{
	t_token		**lexeme;
	t_ast_node	*ast;
	char		*x;

	lexeme = (t_token **)malloc(sizeof(t_token *));
	if (lexeme == NULL)
		return ;
	*lexeme = NULL;
	x = ft_strchr(input, '$');
	if (x != NULL)
		input = ft_expand_variables_input(input);
	ft_tokenize(input, lexeme);
	free(input);
	ast = ft_build_ast(lexeme);
	if (ast && ast->status == 1)
	{
		ft_clean_token_list(lexeme);
		ft_set_head_lst(ast);
	}
	else
	{
		ft_clean_token_list(lexeme);
		ft_set_head_lst(ast);
		ft_collect_heredocs(ast);
		ft_execute_ast(ast);
	}
	if (ast)
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
		{
			if (ft_check_unclosed_quotes(input))
				free(input);
			else
				process_input(input);
		}
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

int	ft_check_unclosed_quotes(const char *input)
{
	int	i;
	int	single_quote_open;
	int	double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'' && double_quote_open == 0)
			single_quote_open = !single_quote_open;
		else if (input[i] == '\"' && single_quote_open == 0)
			double_quote_open = !double_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
	{
		fprintf(stderr, "Error: This shell does not handle unclosed quotes\n");
		return (1);
	}
	return (0);
}
