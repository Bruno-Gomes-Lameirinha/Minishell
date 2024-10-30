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
	t_ast_builder	ctx;
	t_token			*cur;

	ctx.root = NULL;
	ctx.current_node = NULL;
	ctx.last_arg_node = NULL;
	ctx.pending_redir = NULL;
	cur = *tokens;
	while (cur)
	{
		if (cur->type_token == WORD || cur->type_token == SINGLE_QUOTES || \
			cur->type_token == DOUBLE_QUOTES)
			ft_handle_word_token(cur, &ctx);
		else if (cur->type_token == PIPE)
			ft_creat_pipe_node(&ctx);
		else if (cur->type_token == R_OUT || cur->type_token == R_OUTAPP || \
				cur->type_token == R_IN || cur->type_token == R_HDOC)
			if (ft_creat_redir_node(&cur, &ctx) == 1)
			{
				ctx.root->status = 1;
				return (ctx.root);
			}
		cur = cur->next;
	}
	if (ctx.root)
		ctx.root->status = 0;
	return (ctx.root);
}

void	ft_handle_word_token(t_token *cur, t_ast_builder *ctx)
{
	if (!ctx->current_node || ctx->current_node->type != NODE_COMMAND)
		ft_creat_cmd_node(cur, ctx);
	else
		ft_creat_arg_node(cur, ctx);
}

void	ft_creat_cmd_node(t_token *current, t_ast_builder *ctx)
{
	t_ast_node	*cmd_node;

	cmd_node = ft_initialize_cmd_node(current);
	if (ctx->pending_redir)
	{
		cmd_node->redirections = ctx->pending_redir;
		ctx->pending_redir = NULL;
	}
	if (ctx->root == NULL)
		ctx->root = cmd_node;
	else if (ctx->current_node && (ctx->current_node->type == NODE_PIPE || \
			ctx->current_node->type == R_OUT || \
			ctx->current_node->type == R_OUTAPP || current->type_token == R_IN))
		ctx->current_node->right = cmd_node;
	ctx->current_node = cmd_node;
	ctx->last_arg_node = NULL;
}

t_ast_node	*ft_initialize_cmd_node(t_token *current)
{
	t_ast_node	*cmd_node;

	cmd_node = malloc(sizeof(t_ast_node));
	if (!cmd_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd_node->type = NODE_COMMAND;
	cmd_node->value = ft_strdup(current->token_node);
	cmd_node->left = NULL;
	cmd_node->head = NULL;
	cmd_node->right = NULL;
	cmd_node->next = NULL;
	cmd_node->head = NULL;
	cmd_node->redirections = NULL;
	return (cmd_node);
}

void	ft_creat_arg_node(t_token *current, t_ast_builder *ctx)
{
	t_ast_node	*arg_node;

	arg_node = malloc(sizeof(t_ast_node));
	if (!arg_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	arg_node->type = NODE_ARGUMENT;
	arg_node->value = ft_strdup(current->token_node);
	arg_node->left = NULL;
	arg_node->head = NULL;
	arg_node->head = NULL;
	arg_node->right = NULL;
	arg_node->redirections = NULL;
	arg_node->next = NULL;
	if (!ctx->last_arg_node)
		ctx->current_node->right = arg_node;
	else
		ctx->last_arg_node->right = arg_node;
	ctx->last_arg_node = arg_node;
}
