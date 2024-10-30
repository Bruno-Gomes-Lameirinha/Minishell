/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 13:53:29 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_handle_pipe(t_ast_node *root)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status[2];

	if (pipe(fd) == -1)
		ft_perror_exit("pipe");
	left_pid = fork();
	if (left_pid == -1)
		ft_perror_exit("fork");
	if (left_pid == 0)
		ft_handle_child_left(fd, root);
	right_pid = fork();
	if (right_pid == -1)
		ft_perror_exit("fork");
	if (right_pid == 0)
		ft_handle_child_right(fd, root);
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &status[0], 0);
	waitpid(right_pid, &status[1], 0);
	ft_update_status_from_children(status);
}

void	ft_handle_child_left(int *fd, t_ast_node *root)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_execute_ast(root->left);
	ft_free_ast(root->head);
	exit(ft_update_status_error(-1));
}

void	ft_handle_child_right(int *fd, t_ast_node *root)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_execute_ast(root->right);
	ft_free_ast(root->head);
	exit(ft_update_status_error(-1));
}

void	ft_perror_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	ft_update_status_from_children(int status[2])
{
	if (WIFEXITED(status[1]))
		ft_update_status_error(WEXITSTATUS(status[1]));
	else if (WIFEXITED(status[0]))
		ft_update_status_error(WEXITSTATUS(status[0]));
	else
		ft_update_status_error(1);
}
