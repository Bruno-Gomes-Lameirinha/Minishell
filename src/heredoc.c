/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:05:53 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 18:16:44 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_handle_heredoc(const char *delimiter, int type)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			fprintf(stderr, "bash: aviso: here-document delimitado pelo \
			fim do arquivo (desejava `%s')\n", delimiter);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_process_heredoc_line(pipe_fd, line, type);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	ft_process_heredoc_line(int *pipe_fd, char *line, int type)
{
	if (type != 10 && type != 11)
		line = ft_expand_variables_input(line);
	write(pipe_fd[1], line, strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
}

int	ft_collect_heredocs(t_ast_node *root)
{
	t_redirection	*redir;
	int				heredoc_fd;

	if (!root)
		return (0);
	if (root->type == NODE_COMMAND)
	{
		redir = root->redirections;
		while (redir)
		{
			if (redir->type_token == R_HDOC)
			{
				heredoc_fd = ft_handle_heredoc(redir->filename, \
				redir->type_filename);
				if (heredoc_fd == -1)
					return (-1);
				redir->heredoc_fd = heredoc_fd;
			}
			redir = redir->next;
		}
	}
	if (ft_collect_heredocs(root->left) == -1 || \
	ft_collect_heredocs(root->right) == -1)
		return (-1);
	return (0);
}

void	ft_free_ast(t_ast_node *root)
{
	t_redirection	*redir;
	t_redirection	*tmp;

	if (!root)
		return ;
	ft_free_ast(root->left);
	ft_free_ast(root->right);
	ft_free_ast(root->next);
	if (root->type == NODE_COMMAND)
	{
		redir = root->redirections;
		while (redir)
		{
			tmp = redir;
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
