/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira <livieira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:40:09 by livieira          #+#    #+#             */
/*   Updated: 2024/10/15 17:48:02 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *s, int n)
{
	char	*ptr;
	int		i;

	i = 0;
	while (s[i])
		i++;
	if (n < i)
		i = n;
	ptr = malloc(sizeof(ptr) * (i + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s && *s && i < n)
		ptr[i++] = *s++;
	ptr[i] = '\0';
	return (ptr);
}
