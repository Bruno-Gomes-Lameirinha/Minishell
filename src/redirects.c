/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_redirection(t_redirection *redir, int *saved_stdin, \
int *saved_stdout)
{
	int	ret;

	while (redir)
	{
		ret = 0;
		if (redir->type_token == R_IN)
			ret = handle_redirection_in(redir, saved_stdin);
		else if (redir->type_token == R_OUT)
			ret = handle_redirection_out(redir, saved_stdout);
		else if (redir->type_token == R_OUTAPP)
			ret = handle_redirection_out_append(redir, saved_stdout);
		else if (redir->type_token == R_HDOC)
			ret = handle_heredoc_redirection(redir, saved_stdin);
		if (ret == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

int	handle_heredoc_redirection(t_redirection *redir, int *saved_stdin)
{
	if (redir->heredoc_fd == -1)
		return (-1);
	if (*saved_stdin == -1)
		*saved_stdin = dup(STDIN_FILENO);
	dup2(redir->heredoc_fd, STDIN_FILENO);
	close(redir->heredoc_fd);
	redir->heredoc_fd = -1;
	return (0);
}

int	handle_redirection_out_append(t_redirection *redir, int *saved_stdout)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (*saved_stdout == -1)
		*saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirection_out(t_redirection *redir, int *saved_stdout)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (*saved_stdout == -1)
		*saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirection_in(t_redirection *redir, int *saved_stdin)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (*saved_stdin == -1)
		*saved_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
