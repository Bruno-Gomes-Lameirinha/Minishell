/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 14:10:37 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/10 14:51:07 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*node_to_del;
	t_list	*current;

	if (*lst == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		node_to_del = current;
		current = current->next;
		del(node_to_del->content);
		free (node_to_del);
	}
	*lst = NULL;
}
