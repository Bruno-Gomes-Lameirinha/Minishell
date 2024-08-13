/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:41:09 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/15 17:09:52 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_set_zero_str(char *string)
{
	if (string != NULL)
	{
		string[0] = '0';
		string[1] = '\0';
	}
	return (string);
}

static int	ft_extract_int_to_char(int n, char *c)
{
	long	nl;
	int		i;

	i = 0;
	nl = n;
	if (nl < 0)
		nl *= -1;
	while (nl > 0)
	{
		c[i++] = (nl % 10) + 48;
		nl /= 10;
	}
	if (n < 0)
		c[i++] = '-';
	c[i++] = '\0';
	return (i - 1);
}

static void	ft_write_string(int i, int j, char *c, char *string)
{
	while (j < i)
	{
		string[j] = c[i - 1 - j];
		j++;
	}
}

char	*ft_itoa(int n)
{
	int		i;
	int		j;
	long	nl;
	char	c [11];
	char	*string;

	nl = n;
	j = 0;
	if (nl == 0)
	{
		string = ft_set_zero_str((char *)malloc(2 * sizeof(char)));
		if (string == NULL)
			return (NULL);
		return (string);
	}
	i = ft_extract_int_to_char(n, c);
	string = (char *)malloc((i + 1) * sizeof(char));
	if (string == NULL)
		return (NULL);
	string[i] = '\0';
	ft_write_string(i, j, c, string);
	return (string);
}
