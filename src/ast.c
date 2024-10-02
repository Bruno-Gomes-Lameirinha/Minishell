#include "../include/minishell.h"

void	ft_creat_cmd_node(t_token *current, t_ast_node **root, \
t_ast_node **current_node, t_ast_node **last_arg_node)
{
	t_ast_node	*cmd_node;

	cmd_node = malloc(sizeof(t_ast_node));
	cmd_node->type = NODE_COMMAND;
	cmd_node->value = ft_strdup(current->token_node);
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->next = NULL;
	cmd_node->redirections = NULL;
	if (*root == NULL)
	{
		*root = cmd_node;
	}
	else if ((*current_node) && ((*current_node)->type == NODE_PIPE || \
	(*current_node)->type == REDIR_OUT || (*current_node)->type \
	== REDIR_OUTAPP || current->type_token == REDIR_IN))
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
	arg_node->right = NULL;
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
	pipe_node->right = NULL;
	*root = pipe_node;
	*current_node = pipe_node;
}

void	ft_creat_redir_node(t_token **current, t_ast_node **current_node)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	redir->type_token = (*current)->type_token;
	redir->filename = ft_strdup((*current)->next->token_node);
	redir->heredoc_fd = -1;
	redir->next = (*current_node)->redirections;
	(*current_node)->redirections = redir;
	*current = (*current)->next;
}

t_ast_node	*ft_build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_ast_node	*current_node;
	t_ast_node	*last_arg_node;
	t_token		*current;

	current = *tokens;
	root = NULL;
	while (current)
	{
		if (current->type_token == WORD || current->type_token == SINGLE_QUOTES \
		|| current->type_token == DOUBLE_QUOTES)
		{
			if (!current_node || current_node->type != NODE_COMMAND)
				ft_creat_cmd_node(current, &root, &current_node, &last_arg_node);
			else
				ft_creat_arg_node(current, &current_node, &last_arg_node);
		}
		else if (current->type_token == PIPE)
			ft_creat_pipe_node(&root, &current_node);
		else if (current->type_token == REDIR_OUT || current->type_token == REDIR_OUTAPP || \
		current->type_token == REDIR_IN ||  current->type_token == REDIR_HDOC)
			ft_creat_redir_node(&current, &current_node);
		current = current->next;
	}
	return (root);
}
