/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:02:24 by livieira          #+#    #+#             */
/*   Updated: 2024/10/11 15:12:56 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_count_tokens(t_ast_node *command)
{
	int			count;
	t_ast_node	*current;

	count = 0;
	current = command;
	while (current != NULL)
	{
		count++;
		current = current->right;
	}
	return (count);
}

char	**ft_convert_tokens_to_args(t_ast_node *command)
{
	char		**args;
	int			i;
	t_ast_node	*current;
	int			num_tokens;

	i = 0;
	current = command;
	num_tokens = ft_count_tokens(command);
	args = malloc(sizeof(char *) * (num_tokens + 1));
	if (!args)
		return (NULL);
	while (current)
	{
		args[i++] = current->value;
		current = current->right;
	}
	args[i] = NULL;
	return (args);
}

int	ft_export_command(t_ast_node *command)
{
	int		i;
	char	**args;
	int		status;

	i = 0;
	status = 0;
	args = ft_convert_tokens_to_args(command);
	if (!args[1])
		return (ft_handle_no_args_export(args, status));
	while (args[++i])
		ft_process_export_arg(args[i], &status);
	ft_update_status_error(status);
	free(args);
	return (ft_set_exit_status(status));
}

void	ft_process_export_arg(char *arg, int *status)
{
	char	*key;
	char	*equal_sign;

	if (!ft_is_valid_identifier(arg, arg))
	{
		(*status)++;
		return ;
	}
	key = ft_get_key(arg);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		ft_set_env(arg, key, equal_sign + 1);
	else
		ft_set_env(arg, key, NULL);
	free(key);
}

int	ft_handle_no_args_export(char **args, int status)
{
	ft_print_env_sort();
	ft_update_status_error(0);
	free(args);
	return (ft_set_exit_status(status));
}
