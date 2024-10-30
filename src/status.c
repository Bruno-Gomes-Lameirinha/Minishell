/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 15:37:59 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	ft_cleanup_and_exit(t_ast_node *command, long status)
{
	ft_free_ast(command);
	ft_status(status);
	ft_update_status_error(0);
	ft_free_env(*ft_get_env());
	exit(*ft_get_exit_status_env());
}
