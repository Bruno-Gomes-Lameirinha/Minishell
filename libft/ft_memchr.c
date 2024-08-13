/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:41:15 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/10 13:35:28 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_ptr;
	unsigned char	char_to_find;

	s_ptr = (unsigned char *)s;
	char_to_find = (unsigned char)c;
	if (s == NULL && !n)
		return (NULL);
	while (n > 0)
	{
		if (*s_ptr == char_to_find)
			return (s_ptr);
		s_ptr++;
		n--;
	}
	return (NULL);
}
