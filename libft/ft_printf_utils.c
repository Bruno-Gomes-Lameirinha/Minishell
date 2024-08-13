/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 18:45:42 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/10/04 18:46:08 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_char(int fd, int c)
{
	return (write(fd, &c, 1));
}

int	ft_print_str(int fd, char *str)
{
	int	count;

	count = 0;
	if (str == NULL)
		return ((ft_print_str(fd, "(null)")));
	while (*str)
	{
		ft_print_char(fd, (int)*str);
		++count;
		++str;
	}
	return (count);
}

int	ft_print_pointer(int fd, unsigned long num)
{
	char	*symbols;
	int		count;

	symbols = "0123456789abcdef";
	count = 0;
	if (num == 0)
		return (ft_print_str(fd, "(nil)"));
	if (num >= 16)
		count += ft_print_pointer(fd, num / 16);
	else
		count += ft_print_str(fd, "0x");
	count += ft_print_char(fd, symbols[num % 16]);
	return (count);
}

int	ft_print_d(int fd, long n, int base, char *symbols)
{
	int		count;
	char	*symbols_f;

	count = 0;
	symbols_f = symbols;
	if (n < 0)
	{
		write(fd, "-", 1);
		return (ft_print_d(fd, -n, base, symbols_f) + 1);
	}
	else if (n < base)
		return (ft_print_char(fd, symbols[n]));
	else
	{
		count = ft_print_d(fd, n / base, base, symbols_f);
		return (count + ft_print_d(fd, n % base, base, symbols));
	}
}
