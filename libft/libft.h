/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-l <bgomes-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 18:16:08 by bgomes-l          #+#    #+#             */
/*   Updated: 2023/08/10 13:30:22 by bgomes-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

# define DEC_BASE "0123456789"
# define HEXA_LOW "0123456789abcdef"
# define HEXA_UP "0123456789ABCDEF"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int		ft_isalpha(int letter);
int		ft_isdigit(int number);
int		ft_isalnum(int digit);
int		ft_isascii(int ascii);
int		ft_isprint(int print);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *s, int bytes, size_t memory);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t n);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *str1, const char *str2, size_t n);
void	*ft_memchr(const void *ptr, int value, size_t n);
int		ft_memcmp(const void *ptr1, const void *ptr2, size_t n);
char	*ft_strnstr(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t nitems, size_t size);
char	*ft_strdup(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	ft_strlcpy2(char *dst, const char *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_findchr(const char *s, char c);
char	*ft_line(char *temp, char *x, char *remainder, char *buffer);
char	*read_check(int bytes_read, char *remainder, char *buffer, char *temp);
void	ft_strconcat(char **temp, char **x, char *remainder, char *buffer);
int		ft_printf(int fd, const char *format, ...);
int		ft_print_format(int fd, char specifier, va_list ap);
int		ft_print_digit_upper(long n, int base);
int		ft_print_d(int fd, long n, int base, char *symbols);
int		ft_print_pointer(int fd, unsigned long num);
int		ft_print_str(int fd, char *str);
int		ft_print_char(int fd, int c);

#endif
