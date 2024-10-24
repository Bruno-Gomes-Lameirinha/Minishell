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

void	ft_creat_cmd_node(t_token *current, t_ast_node **root, \
t_ast_node **current_node, t_ast_node **last_arg_node)
{
	t_ast_node	*cmd_node;

	cmd_node = malloc(sizeof(t_ast_node));
	cmd_node->type = NODE_COMMAND;
	cmd_node->value = ft_strdup(current->token_node);
	cmd_node->left = NULL;
	cmd_node->head = NULL;
	cmd_node->right = NULL;
	cmd_node->next = NULL;
	cmd_node->redirections = NULL;
	if (*root == NULL)
	{
		*root = cmd_node;
	}
	else if ((*current_node) && ((*current_node)->type == NODE_PIPE || \
	(*current_node)->type == R_OUT || (*current_node)->type \
	== R_OUTAPP || current->type_token == R_IN))
	{
		(*current_node)->right = cmd_node;
	}
	*current_node = cmd_node;
	(*last_arg_node) = NULL;
}

void	ft_creat_arg_node(t_token *current, t_ast_node **current_node, \
t_ast_node **last_arg_node)
{
	t_ast_node	*arg_node;

	arg_node = malloc(sizeof(t_ast_node));
	arg_node->type = NODE_ARGUMENT;
	arg_node->value = ft_strdup(current->token_node);
	arg_node->left = NULL;
	arg_node->head = NULL;
	arg_node->right = NULL;
	arg_node->redirections = NULL;
	arg_node->next = NULL;
	if (!(*last_arg_node))
	{
		(*current_node)->right = arg_node;
	}
	else
	{
		(*last_arg_node)->right = arg_node;
	}
	*last_arg_node = arg_node;
}

void	ft_creat_pipe_node(t_ast_node **root, t_ast_node **current_node)
{
	t_ast_node	*pipe_node;

	pipe_node = malloc(sizeof(t_ast_node));
	pipe_node->type = NODE_PIPE;
	pipe_node->value = NULL;
	pipe_node->left = *root;
	pipe_node->next = NULL;
	pipe_node->right = NULL;
	pipe_node->head = NULL;
	pipe_node->redirections = NULL;
	*root = pipe_node;
	*current_node = pipe_node;
}

void	ft_creat_redir_node(t_token **current, t_ast_node **current_node)
{
	t_redirection	*redir;
	t_redirection	*last_redir;
	t_ast_node		*cmd_node;

	redir = malloc(sizeof(t_redirection));
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

	// Verifique se o current_node é um pipe ou outro tipo que não seja comando
	if ((*current_node) && ((*current_node)->type == NODE_PIPE || (*current_node)->type == NODE_ARGUMENT))
	{
		// Navega para o próximo nó de comando à direita do pipe
		cmd_node = (*current_node)->left;
		while (cmd_node && cmd_node->type != NODE_COMMAND)
			cmd_node = cmd_node->left;
		if (cmd_node)
			*current_node = cmd_node; // Define current_node como o comando correto
		else
		{
			// Erro: não encontrou um comando onde o redirecionamento deveria estar
			perror("Erro: não há comando para associar o redirecionamento");
			free(redir);
			return;
		}
	}

	// Adiciona o redirecionamento ao nó de comando encontrado
	if ((*current_node)->redirections == NULL)
		(*current_node)->redirections = redir;
	else
	{
		last_redir = (*current_node)->redirections;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = redir;
	}
	*current = (*current)->next;  // Avança o ponteiro de token
}
