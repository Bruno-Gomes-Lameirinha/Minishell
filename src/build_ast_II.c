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

void	ft_creat_pipe_node(t_ast_builder *ctx)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	if (!pipe_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	pipe_node->type = NODE_PIPE;
	pipe_node->value = NULL;
	pipe_node->left = ctx->root;
	pipe_node->next = NULL;
	pipe_node->right = NULL;
	pipe_node->head = NULL;
	pipe_node->redirections = NULL;
	ctx->root = pipe_node;
	ctx->current_node = pipe_node;
}

void	ft_creat_redir_node(t_token **current, t_ast_builder *ctx)
{
	t_redir	*redir;

	redir = create_redirection(current);
	if (ctx->current_node && ctx->current_node->type == NODE_COMMAND)
		ft_add_redirection_to_command(ctx->current_node, redir);
	else
		ft_add_redirection_to_pending(&ctx->pending_redir, redir);
	*current = (*current)->next;
}

t_redir	*create_redirection(t_token **current)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	redir->type_filename = (*current)->next->type_token;
	redir->type_token = (*current)->type_token;
	redir->filename = ft_strdup((*current)->next->token_node);
	redir->heredoc_fd = -1;
	redir->next = NULL;
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
