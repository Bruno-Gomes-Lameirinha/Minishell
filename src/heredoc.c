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

int ft_collect_heredocs(t_ast_node *root)
{
	t_redirection *redir;
	int heredoc_fd;

	if (!root)
		return 0;
	if (root->type == NODE_COMMAND)
	{
		redir = root->redirections;
		while (redir)
		{
			if (redir->type_token == R_HDOC)
			{
				heredoc_fd = ft_handle_heredoc(redir->filename);
				if (heredoc_fd == -1)
					return -1;
				redir->heredoc_fd = heredoc_fd;
			}
			redir = redir->next;
		}
	}
	if (ft_collect_heredocs(root->left) == -1)
		return -1;
	if (ft_collect_heredocs(root->right) == -1)
		return -1;
	return 0;
}

void ft_free_ast(t_ast_node *root)
{
	if (!root)
		return;

	ft_free_ast(root->left);
	ft_free_ast(root->right);

	if (root->type == NODE_COMMAND)
	{
		t_redirection *redir = root->redirections;
		while (redir)
		{
			t_redirection *tmp = redir;
			redir = redir->next;
			if (tmp->filename)
				free(tmp->filename);
			if (tmp->heredoc_fd != -1)
				close(tmp->heredoc_fd);
			free(tmp);
		}
	}
	if (root->value)
		free(root->value);
	free(root);
}
