/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:42:47 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/08 17:17:51 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		len;
	int		i;
	char	*new_string;

	i = 0;
	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	new_string = (char *)malloc((len + 1) * sizeof(char));
	if (new_string == NULL)
		return (NULL);
	while (i < len)
	{
		new_string[i] = f(i, s[i]);
		i++;
	}
	new_string[len] = '\0';
	return (new_string);
}
