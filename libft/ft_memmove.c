/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:41:40 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 16:49:00 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*dest_ptr;
	const char	*src_ptr;

	dest_ptr = (char *)dest;
	src_ptr = (const char *)src;
	if (src_ptr < dest_ptr)
	{
		src_ptr += n;
		dest_ptr += n;
		while (n-- > 0)
			*(--dest_ptr) = *(--src_ptr);
	}
	else if (src_ptr > dest_ptr)
	{
		while (n-- > 0)
			*dest_ptr++ = *src_ptr++;
	}
	return (dest);
}
