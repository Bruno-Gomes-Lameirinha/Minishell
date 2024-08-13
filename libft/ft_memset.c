/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:51:25 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 17:03:23 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	char_to_set;

	ptr = (unsigned char *)s;
	char_to_set = (unsigned char)c;
	while (n > 0)
	{
		*ptr = char_to_set;
		ptr++;
		n--;
	}
	return (s);
}
