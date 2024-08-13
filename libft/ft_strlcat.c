/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:42:32 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/07/27 20:22:58 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	sourcelen;
	size_t	destlen;

	sourcelen = ft_strlen (src);
	destlen = ft_strlen (dst);
	if (size <= destlen)
		return (sourcelen + size);
	while (*dst)
	{
		if (size > 0)
			size--;
		dst++;
	}
	while (*src && size > 1)
	{
		*dst++ = *src++;
		size--;
	}
	*dst = '\0';
	return (sourcelen + destlen);
}
