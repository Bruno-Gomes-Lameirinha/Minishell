/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:05:53 by livieira          #+#    #+#             */
/*   Updated: 2024/10/08 18:16:44 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_echo_command_with_ast(t_ast_node *node)
{
	int			new_line;
	t_ast_node	*current;

	if (!node)
	{
		ft_update_status_error(1);
		return ;
	}
	new_line = 1;
	current = node->right;
	if (current && !strcmp(current->value, "-n"))
	{
		new_line = 0;
		current = current->right;
	}
	ft_print_arguments(current);
	ft_print_newline(new_line);
	ft_update_status_error(0);
}

void	ft_print_arguments(t_ast_node *current)
{
	while (current)
	{
		ft_write_value(current->value);
		if (current->right)
			ft_write_space();
		current = current->right;
	}
}

void	ft_write_space(void)
{
	if (write(1, " ", 1) == -1)
	{
		perror("write");
		ft_update_status_error(1);
	}
}

void	ft_write_value(const char *value)
{
	if (write(1, value, strlen(value)) == -1)
	{
		perror("write");
		ft_update_status_error(1);
	}
}

void	ft_print_newline(int new_line)
{
	if (new_line)
	{
		if (write(1, "\n", 1) == -1)
		{
			perror("write");
			ft_update_status_error(1);
		}
	}
}
