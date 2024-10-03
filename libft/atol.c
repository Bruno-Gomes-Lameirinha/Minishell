/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atol.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: livieira < livieira@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 00:16:36 by livieira          #+#    #+#             */
/*   Updated: 2024/10/03 00:16:39 by livieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(char *str)
{
	char	*c;
	int		i;
	int		sign;
	long	sum;

	i = 0;
	sign = 1;
	c = str;
	sum = 0;
	while ((c[i] >= '\t' && c[i] <= '\r') || c[i] == ' ')
		i++;
	if (c[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (c[i] == '+')
		i++;
	while (ft_isdigit(c[i]))
	{
		sum = (sum * 10) + (c[i] - '0');
		i++;
	}
	return (sum * sign);
}
