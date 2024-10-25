/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/09 23:27:20 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_pid_last_exit_status(pid_t pid)
{
    int status;

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
