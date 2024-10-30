/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_utils_II.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 14:36:19 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**ft_generate_args(t_ast_node *command_node)
{
	char	**args;

	args = ft_allocate_args(command_node);
	ft_fill_args(args, command_node);
	return (args);
}

void	ft_fill_args(char **args, t_ast_node *command_node)
{
	t_ast_node	*current;
	int			i;

	current = command_node;
	i = 0;
	while (current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
		{
			perror("ft_strdup");
			exit(EXIT_FAILURE);
		}
		i++;
		current = current->right;
	}
	args[i] = NULL;
}

char	**ft_allocate_args(t_ast_node *command_node)
{
	int			n_args;
	t_ast_node	*current;
	char		**args;

	n_args = 0;
	current = command_node;
	while (current)
	{
		n_args++;
		current = current->right;
	}
	args = malloc(sizeof(char *) * (n_args + 1));
	if (args == NULL)
		return (NULL);
	return (args);
}

void	ft_free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}
