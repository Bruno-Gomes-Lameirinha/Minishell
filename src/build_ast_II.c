/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast_II.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 15:24:49 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_creat_pipe_node(t_ast_builder *ctx)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	if (pipe_node == NULL)
		return ;
	pipe_node->type = NODE_PIPE;
	pipe_node->value = NULL;
	pipe_node->left = ctx->root;
	pipe_node->next = NULL;
	pipe_node->right = NULL;
	pipe_node->head = NULL;
	pipe_node->status = 0;
	pipe_node->redirections = NULL;
	ctx->root = pipe_node;
	ctx->current_node = pipe_node;
}

int	ft_creat_redir_node(t_token **current, t_ast_builder *ctx)
{
	t_redir	*redir;

	redir = create_redirection(current);
	if (redir == NULL)
		return (1);
	if (ctx->current_node && ctx->current_node->type == NODE_COMMAND)
		ft_add_redirection_to_command(ctx->current_node, redir);
	else
		ft_add_redirection_to_pending(&ctx->pending_redir, redir);
	*current = (*current)->next;
	return (0);
}

t_redir	*create_redirection(t_token **current)
{
	t_redir	*redir;

	if (!(*current)->next)
	{
		ft_putstr_fd("Minishell: syntax error near unexpected token `newline'\n", 2);
		redir = NULL;
		return (redir);
	}
	redir = malloc(sizeof(t_redir));
	if (redir == NULL)
		return (NULL);
	if ((*current)->next)
	{
		redir->type_filename = (*current)->next->type_token;
		redir->type_token = (*current)->type_token;
		redir->filename = ft_strdup((*current)->next->token_node);
		redir->heredoc_fd = -1;
		redir->next = NULL;
	}
	return (redir);
}

void	ft_add_redirection_to_command(t_ast_node *current_node, t_redir *redir)
{
	t_redir	*last_redir;

	if (current_node->redirections == NULL)
		current_node->redirections = redir;
	else
	{
		last_redir = current_node->redirections;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = redir;
	}
}

void	ft_add_redirection_to_pending(t_redir **pending_redir, t_redir *redir)
{
	t_redir	*last_redir;

	if (*pending_redir == NULL)
		*pending_redir = redir;
	else
	{
		last_redir = *pending_redir;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = redir;
	}
}
