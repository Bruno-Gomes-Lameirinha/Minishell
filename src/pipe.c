/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_handle_pipe(t_ast_node *root)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

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
	waitpid(left_pid, NULL, 0);
	ft_pid_last_exit_status(right_pid);
}

void	ft_handle_child_left(int *fd, t_ast_node *root)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_execute_ast(root->left);
	ft_free_ast(root->head);
	exit(ft_update_status_error(-1));
}

void	ft_handle_child_right(int *fd, t_ast_node *root)
{
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
