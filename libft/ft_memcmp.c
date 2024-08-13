/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:41:20 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 16:47:11 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char		*s1_ptr;
	const unsigned char		*s2_ptr;

	s1_ptr = (unsigned char *)s1;
	s2_ptr = (unsigned char *)s2;
	while (n > 0)
	{
		if (*s1_ptr != *s2_ptr)
		{
			return (*s1_ptr - *s2_ptr);
		}
		s1_ptr++;
		s2_ptr++;
		n--;
	}
	return (0);
}
