/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:42:16 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/07/28 19:23:25 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	int		i;
	char	*pointer;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	pointer = malloc((i + 1) * sizeof(char));
	if (pointer == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		pointer[i] = src[i];
		i++;
	}
	pointer[i] = '\0';
	return (pointer);
}
