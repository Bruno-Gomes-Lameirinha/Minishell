#include "../include/minishell.h"

int ft_handle_heredoc(const char *delimiter, int type)
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
		{
			fprintf(stderr, "bash: aviso: here-document delimitado pelo fim do arquivo (desejava `%s')\n", delimiter);
			break;
		}
		if (strcmp(line, delimiter) == 0) 
		{
			free(line);
			break;
		}
		if (type != 10 && type != 11)
			line = ft_expand_variables_input(line);
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
				heredoc_fd = ft_handle_heredoc(redir->filename, redir->type_filename);
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
	t_redirection *redir;
	t_redirection *tmp_redir;

	redir = NULL;
	tmp_redir = NULL;
	if (!root)
		return;
	ft_free_ast(root->left);
	ft_free_ast(root->right);
	if (root->redirections != NULL)
		redir = root->redirections;
	if (redir)
	{
		while (redir)
		{
			tmp_redir = redir;
			redir = redir->next;
			if (tmp_redir->filename)
				free(tmp_redir->filename);
			if (tmp_redir->heredoc_fd != -1)
				close(tmp_redir->heredoc_fd);
			free(tmp_redir);
		}
	}
	if (root->value)
		free(root->value);
	free(root);
}
