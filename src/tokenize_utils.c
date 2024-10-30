/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 18:33:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2024/10/30 16:15:00 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_clean_token_list(t_token **lst)
{
	t_token	*node_to_del;
	t_token	*current;

	if (*lst == NULL)
	{
		free(lst);
		return ;
	}
	current = *lst;
	while (current != NULL)
	{
		node_to_del = current;
		current = current->next;
		free(node_to_del->token_node);
		free (node_to_del);
	}
	*lst = NULL;
	free(lst);
}

char	*ft_mem_token(char *input)
{
	int		len;
	char	*memset_token;

	memset_token = NULL;
	len = ft_strlen(input);
	memset_token = (char *)ft_calloc(len + 1, sizeof(char *));
	if (!memset_token)
		return (NULL);
	return (memset_token);
}

int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

t_token	*ft_list_new_token(void)
{
	t_token	*new_node;

	new_node = calloc(1, sizeof (t_token));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	new_node->token_node = NULL;
	return (new_node);
}

void	ft_free_token(char	*current_token, int	*type)
{
	free(current_token);
	free(type);
}
