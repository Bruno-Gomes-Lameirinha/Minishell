/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:32:00 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/09/19 17:04:10 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_findchr(const char *s, char c)
{
	size_t	i;

	i = 1;
	while (*s != c)
	{
		if (*s == '\0')
		{
			return (i);
		}
		s++;
		i++;
	}
	return (i);
}

char	*ft_line(char *temp, char *x, char *remainder,
		char *buffer)
{
	char	*start;
	char	*temp2;

	start = NULL;
	temp2 = NULL;
	if (temp == NULL)
	{
		ft_memset(buffer, 0, BUFFER_SIZE);
		temp = ft_strjoin(remainder, buffer);
		x = ft_strchr(temp, '\n');
	}
	ft_memset(buffer, 0, BUFFER_SIZE);
	start = x + 1;
	ft_memset(remainder, 0, BUFFER_SIZE);
	ft_strlcpy2(remainder, start, ft_findchr(start, '\0'));
	temp2 = ft_substr(temp, 0, ft_findchr(temp, '\n'));
	free(temp);
	return (temp2);
}

char	*read_check(int bytes_read, char *remainder, char *buffer, char *temp)
{
	if (bytes_read <= 0 && *remainder == '\0' && !temp)
	{
		free(temp);
		return (NULL);
	}
	if (bytes_read == 0)
	{
		if (!temp)
			temp = ft_strjoin(remainder, buffer);
		ft_memset(remainder, 0, BUFFER_SIZE);
		return (temp);
	}
	else if (bytes_read < 0)
	{
		ft_memset(remainder, 0, BUFFER_SIZE);
		return (NULL);
	}
	return (NULL);
}

void	ft_strconcat(char **temp, char **x, char *remainder, char *buffer)
{
	char	*temp2;

	temp2 = NULL;
	if (!*temp)
	{
		*temp = ft_strjoin(remainder, buffer);
		ft_memset(remainder, 0, BUFFER_SIZE);
		ft_memset(buffer, 0, BUFFER_SIZE);
		*x = ft_strchr(*temp, '\n');
	}
	else
	{
		temp2 = ft_strjoin(*temp, buffer);
		ft_memset(buffer, 0, BUFFER_SIZE);
		free(*temp);
		*temp = ft_strjoin(temp2, buffer);
		*x = ft_strchr(*temp, '\n');
		free(temp2);
	}
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	static char	remainder[BUFFER_SIZE];
	int			bytes_read;
	char		*x;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	temp = NULL;
	x = NULL;
	bytes_read = 0;
	x = ft_strchr(remainder, '\n');
	if (x == NULL)
	{
		while (x == NULL)
		{
			bytes_read = read(fd, buffer, BUFFER_SIZE);
			if (bytes_read <= 0)
				return (read_check(bytes_read, remainder, buffer, temp));
			else if (bytes_read > 0)
				ft_strconcat(&temp, &x, remainder, buffer);
		}
	}
	return (ft_line(temp, x, remainder, buffer));
}
