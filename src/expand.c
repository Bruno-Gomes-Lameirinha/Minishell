/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/08/28 17:11:33 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_expand_variables(t_token	**lexeme)
{
	t_token	*current;
	char	*var_key;
	char	*var_value;
	char	*x;

	current = *lexeme;

	while (current != NULL)
	{
		if (current->type_token == WORD || current->type_token == DOUBLE_QUOTES)
		{
			x = ft_strchr(current->token_node, '$');
			if (x != NULL)
			{
				var_key = ft_substr(current->token_node, ft_findchr(current->token_node, '$'), ft_strlen_var(x+1));
				var_value = getenv(var_key);
				if (var_value != NULL)
				{
					free(var_key);
					var_key = ft_strjoin(var_value, (x + (ft_strlen_var(x + 1) +1 )));
					free(current->token_node);
					current->token_node = ft_strdup(var_key);
					var_value = NULL;
				}
			}
		}
		current = current->next;
	}
}

size_t	ft_strlen_var(const char *s)
{
	int	lenght;

	lenght = 0;
	while (ft_isalnum(*s++) || *s++ == '_')
	{
		lenght ++;
	}
	return (lenght);
}