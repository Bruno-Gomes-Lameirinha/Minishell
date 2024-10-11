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

int	ft_handle_redirection(t_redirection *redir, int *saved_stdin, \
int *saved_stdout)
{
	int	ret;

	while (redir)
	{
		ret = 0;
		if (redir->type_token == R_IN)
			ret = ft_handle_redirection_in(redir, saved_stdin);
		else if (redir->type_token == R_OUT)
			ret = ft_handle_redirection_out(redir, saved_stdout);
		else if (redir->type_token == R_OUTAPP)
			ret = ft_handle_redirection_out_append(redir, saved_stdout);
		else if (redir->type_token == R_HDOC)
			ret = ft_handle_heredoc_redirection(redir, saved_stdin);
		if (ret == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

int	ft_handle_heredoc_redirection(t_redirection *redir, int *saved_stdin)
{
	if (redir->heredoc_fd == -1)
	{
		update_status_error(1);
		return (-1);
	}
	if (*saved_stdin == -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		if (*saved_stdin == -1)
		{
			perror("dup");
			update_status_error(1);
			return (-1);
		}
	}
	if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(redir->heredoc_fd);
		update_status_error(1);
		return (-1);
	}
	if (close(redir->heredoc_fd) == -1)
	{
		perror("close");
		update_status_error(1);
		return (-1);
	}
	redir->heredoc_fd = -1;
	return (0);
}

int	ft_handle_redirection_out_append(t_redirection *redir, int *saved_stdout)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
		if (*saved_stdout == -1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		if (*saved_stdout == -1)
		{
			perror("dup");
			close(fd);
			update_status_error(1);
			return (-1);
		}
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		update_status_error(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	ft_handle_redirection_out(t_redirection *redir, int *saved_stdout)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		update_status_error(1);
		return (-1);
	}
	if (*saved_stdout == -1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		if (*saved_stdout == -1)
		{
			perror("dup");
			close(fd);
			update_status_error(1);
			return (-1);
		}
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		update_status_error(1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	ft_handle_redirection_in(t_redirection *redir, int *saved_stdin)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		update_status_error(1);
		return (-1);
	}
	if (*saved_stdin == -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		if (*saved_stdin == -1)
		{
			perror("dup");
			close(fd);
			update_status_error(1);
			return (-1);
		}
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		update_status_error(1);
		return (-1);
	}
	close(fd);
	return (0);
}
