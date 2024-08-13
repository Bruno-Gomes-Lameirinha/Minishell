/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:43:11 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 16:50:56 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_string;
	size_t	i;
	size_t	j;
	size_t	s_len;

	j = 0;
	i = start;
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		sub_string = (char *)malloc(1);
		if (sub_string == NULL || s == NULL)
			return (NULL);
		sub_string[0] = '\0';
		return (sub_string);
	}
	if (len > s_len - start)
		len = s_len - start;
	sub_string = (char *)malloc(len + 1);
	if (sub_string == NULL)
		return (NULL);
	while (i < start + len)
		sub_string[j++] = s[i++];
	sub_string[j] = '\0';
	return (sub_string);
}
