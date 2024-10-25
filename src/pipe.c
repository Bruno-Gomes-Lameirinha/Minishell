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
    int		left_status;
    int		right_status;

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
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);
    
    // Atualize o exit code com base no processo que terminou por último
    if (WIFEXITED(right_status))
        ft_update_status_error(WEXITSTATUS(right_status));
    else if (WIFEXITED(left_status))
        ft_update_status_error(WEXITSTATUS(left_status));
    else
        ft_update_status_error(1); // Defina como 1 se nenhum dos processos terminou normalmente
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
