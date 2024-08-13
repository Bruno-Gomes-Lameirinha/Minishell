/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:43:01 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 16:50:27 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*char_to_find;

	char_to_find = NULL;
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			char_to_find = s;
		}
		s++;
	}
	if (*s == (char)c)
	{
		char_to_find = s;
	}
	if (char_to_find == NULL)
	{
		return (NULL);
	}
	return ((char *)char_to_find);
}
