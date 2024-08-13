/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:41:55 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/10 17:02:06 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_write(char *c, int i, int fd)
{
	while (c[--i])
		write(fd, &c[i], 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	int		i;
	long	nl;
	char	c [10];

	nl = n;
	i = 0;
	if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	if (nl < 0)
	{
		nl *= -1;
		write(fd, "-", 1);
	}
	while (nl > 0)
	{
		c[i++] = (nl % 10) + 48;
		nl /= 10;
	}
	ft_write(c, i, fd);
}
