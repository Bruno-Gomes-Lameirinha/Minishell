/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 23:20:51 by livieira          #+#    #+#             */
/*   Updated: 2024/10/21 23:30:11 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_perror_close_exit(const char *msg, int fd)
{
	perror(msg);
	if (fd != -1)
		close(fd);
	ft_update_status_error(1);
	return (-1);
}

int	ft_get_exit_status(int exit_status)
{
	return ((exit_status & 0xff00) >> 8);
}

int	*ft_get_exit_status_env(void)
{
	static int	exit_status;

	return (&exit_status);
}

void	ft_set_head_lst(t_ast_node *root)
{
	t_ast_node	*head;
	t_ast_node	*current;

	head = root;
	current = root;
	if (!root)
		return ;
	current->head = root;
	if (current->right)
		current = current->right;
	while (current)
	{
		current->head = head;
		current = current->right;
	}
	current = head;
	if (current->left)
		current = current->left;
	while (current)
	{
		current->head = head;
		current = current->left;
	}
}

void	ft_pid_last_exit_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ft_update_status_error(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		ft_update_status_error(128 + WTERMSIG(status));
	else
		ft_update_status_error(1);
}

int	ft_update_status_error(int exit_status)
{
	static int	status;

	if (exit_status != -1)
		status = exit_status;
	return (status);
}
