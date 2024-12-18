/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:35:42 by livieira          #+#    #+#             */
/*   Updated: 2024/10/30 17:58:57 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_check_limits(char *arg, char sign)
{
	if (*arg == '+' || *arg == '-')
		arg++;
	if (ft_strlen(arg) > 19)
		return (FAILURE);
	if ((ft_strcmp(arg, "9223372036854775807") > 0 && sign == '+')
		|| (ft_strcmp(arg, "9223372036854775808") > 0 && sign == '-'))
		return (FAILURE);
	return (SUCCESS);
}

void	ft_status(int status)
{
	if (status)
		exit(status % 256);
}

int	ft_validate_argument(char *arg)
{
	int		i;
	char	sign;

	i = 0;
	sign = '+';
	if (arg[i] == '-' || arg[i] == '+')
	{
		sign = arg[i];
		i++;
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (FAILURE);
		i++;
	}
	if (ft_check_limits(arg, sign) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	ft_exit_command(t_ast_node *command)
{
	long	status;

	status = 0;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_printf(1, "exit\n");
	if (command->right)
	{
		status = ft_atol(command->right->value);
		if (ft_validate_argument(command->right->value) || \
		*(command->right->value) == '\0')
		{
			ft_printf(STDERR_FILENO, \
			"exit:%s: numeric argument required\n", command->right->value);
			ft_cleanup_and_exit(command, status, 1);
			exit(SYNTAX_ERROR);
		}
		if (command->right->right)
		{
			ft_handle_exit_argument(command);
			status = FAILURE;
			return (FAILURE);
		}
	}
	ft_cleanup_and_exit(command, status, *ft_get_exit_status_env());
	exit(SUCCESS);
}

void	ft_handle_exit_argument(t_ast_node *command)
{
	if (command->right->right)
	{
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		ft_update_status_error(1);
	}
}
