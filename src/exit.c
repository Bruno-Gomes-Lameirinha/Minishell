/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:35:42 by livieira          #+#    #+#             */
/*   Updated: 2024/10/03 00:25:51 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Verifica se um número está dentro dos limites de um inteiro longo.
int	check_limits(char *arg, char sign)
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
//futuramente adicionar free
void	ft_status(int status)
{
	if (status)
		exit(status % 256);
}

//Verifica se uma string representa um número inteiro válido.
int	validate_argument(char *arg)
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
	if (check_limits(arg, sign) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
//Implementa a lógica do comando exit
int	ft_exit_command(t_token *tokens)
{
	long	status;

	status = 0;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		printf("exit\n");
	if (tokens->next)
	{
		status = ft_atol(tokens->next->token_node);
		if (validate_argument(tokens->next->token_node) || \
				*(tokens->next->token_node) == '\0')
		{
			ft_printf(STDERR_FILENO, "exit:%s: numeric argument required\n", \
			tokens->next->token_node);
			exit(SYNTAX_ERROR);
		}
		if (tokens->next->next)
		{
			write(STDERR_FILENO, "exit: too many arguments\n", 25);
			exit(FAILURE);
		}
	}
	ft_status(status);
	exit(*get_exit_status_env());
}