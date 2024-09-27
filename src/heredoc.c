#include "../include/minishell.h"

int ft_handle_heredoc(const char *delimiter)
{
	int pipe_fd[2];
	char *line;

	if (pipe(pipe_fd) == -1) 
	{
		perror("pipe");
		return -1;
	}
	while (1) {
		line = readline("> ");
		if (!line) 
			break;
		if (strcmp(line, delimiter) == 0) 
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]); 
	return pipe_fd[0];
}

void ft_collect_heredocs(t_ast_node *root)
{
	int heredoc_fd;

	if (!root)
		return;
	if (root->type == NODE_REDIRECTION && root->type_token == REDIR_HDOC) 
	{
		heredoc_fd = ft_handle_heredoc(root->value);
		if (heredoc_fd == -1)
			return;
		root->heredoc_fd = heredoc_fd;
	}
	ft_collect_heredocs(root->left);
	ft_collect_heredocs(root->right);
}

void ft_free_ast(t_ast_node *root)
{
	if (!root)
		return;
	ft_free_ast(root->left);
	ft_free_ast(root->right);
	free(root->value);
	free(root);
}
