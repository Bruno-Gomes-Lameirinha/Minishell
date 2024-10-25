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

t_ast_node	*ft_build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_ast_node	*cur_node;
	t_ast_node	*last_arg;
	t_redirection *pending_redir;
	t_token		*cur;

	root = NULL;
	cur_node = NULL;
	last_arg = NULL;
	pending_redir = NULL;  // Inicializa a lista de redirecionamentos pendentes
	cur = *tokens;
	while (cur)
	{
		if (cur->type_token == WORD || cur->type_token == SINGLE_QUOTES || \
		cur->type_token == DOUBLE_QUOTES)
			ft_handle_word_token(cur, &root, &cur_node, &last_arg, &pending_redir);
		else if (cur->type_token == PIPE)
			ft_creat_pipe_node(&root, &cur_node);
		else if (cur->type_token == R_OUT || cur->type_token == R_OUTAPP || \
		cur->type_token == R_IN || cur->type_token == R_HDOC)
			ft_creat_redir_node(&cur, &cur_node, &pending_redir);
		cur = cur->next;
	}
	return (root);
}

void	ft_handle_word_token(t_token *cur, t_ast_node **root, \
t_ast_node **cur_node, t_ast_node **last_arg, t_redirection **pending_redir)
{
	if (!(*cur_node) || (*cur_node)->type != NODE_COMMAND)
		ft_creat_cmd_node(cur, root, cur_node, last_arg, pending_redir);
	else
		ft_creat_arg_node(cur, cur_node, last_arg);
}
