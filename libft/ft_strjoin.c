/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:42:26 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/03 16:49:41 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*new_str;
	char	*start;

	total_len = ft_strlen(s1) + ft_strlen(s2);
	new_str = (char *)malloc(total_len + 1);
	if (new_str == NULL || s1 == NULL || s2 == NULL)
		return (NULL);
	start = new_str;
	while (*s1)
		*new_str++ = *s1++;
	while (*s2)
		*new_str++ = *s2++;
	*new_str = '\0';
	return (start);
}
