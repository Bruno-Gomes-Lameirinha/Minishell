/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:42:07 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/15 17:16:03 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest [i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest [i] = '\0';
		i++;
	}
	dest [i] = '\0';
	return (dest);
}

static int	ft_st(char const *s, char c)
{
	const char	*index = s;
	int			how_many_strings;

	how_many_strings = 0;
	while (*index != '\0')
	{
		while (*index == c)
		{
			index++;
		}
		if (*index != '\0')
		{
			how_many_strings++;
			while (*index != c && *index != '\0')
			{
				index++;
			}
		}
	}
	return (how_many_strings);
}

static void	ft_start_s(char const **s, const char	**start, char c)
{
	while (**s == c)
		(*s)++;
	*start = *s;
	while (**s != c && **s != '\0')
		(*s)++;
}

static char	**ft_free_malloc(int i, char **n_s_array)
{
	while (i >= 0)
		free(n_s_array[i--]);
	free(n_s_array);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char		**n_s_array;
	int			i;
	int			n_str;
	const char	*start;

	i = 0;
	n_s_array = (char **)malloc(((n_str = ft_st (s, c)) + 1) * sizeof(char *));
	if (n_s_array == NULL)
		return (NULL);
	while (i < n_str)
	{
		ft_start_s(&s, &start, c);
		n_s_array[i] = (char *)malloc((s - start + 1) * sizeof(char));
		if (n_s_array[i] == NULL)
		{
			return (ft_free_malloc(i, n_s_array));
		}
		ft_strncpy(n_s_array[i++], ((char *)start), s - start);
	}
	n_s_array[i] = NULL;
	return (n_s_array);
}
