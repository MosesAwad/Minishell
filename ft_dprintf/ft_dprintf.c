/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 15:53:18 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 23:05:23 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

static int	ft_print_str(int fd, char *s)
{
	if (s == NULL)
	{
		ft_putstr(fd, "(null)");
		return (ft_strlen("(null)"));
	}
	ft_putstr(fd, s);
	return ((int) ft_strlen(s));
}

static int	ft_print_char(int fd, int c)
{
	ft_putchar(fd, c);
	return (1);
}

static int	case_choose(int fd, va_list args, const char *str, unsigned int *i)
{
	unsigned int	len;

	if (str[*i + 1] == 'x' || str[*i + 1] == 'X')
		len = ft_print_hex(fd, va_arg(args, unsigned int), str[*i + 1]);
	if (str[*i + 1] == 'd' || str[*i + 1] == 'i')
		len = ft_print_nbr(fd, va_arg(args, int));
	if (str[*i + 1] == 's')
		len = ft_print_str(fd, va_arg(args, char *));
	if (str[*i + 1] == 'p')
		len = ft_print_address(fd, va_arg(args, unsigned long long int));
	if (str[*i + 1] == 'c')
		len = ft_print_char(fd, va_arg(args, int));
	if (str[*i + 1] == 'u')
		len = ft_print_unsigned(fd, va_arg(args, unsigned int));
	if (str[*i + 1] == '%')
	{
		ft_putchar(fd, str[*i + 1]);
		len = 1;
	}
	return (len);
}

int	ft_dprintf(int fd, const char *str, ...)
{
	va_list			args;
	unsigned int	i;
	unsigned int	len;
	const char		*set;

	va_start(args, str);
	i = 0;
	len = 0;
	set = "cdisxXpu%%";
	while (str[i])
	{
		if (str[i] == '%' && ft_strchr(set, str[i + 1]))
		{
			len += case_choose(fd, args, str, &i);
			i++;
		}
		else
		{
			ft_putchar(fd, str[i]);
			len += 1;
		}
		i++;
	}
	va_end(args);
	return (len);
}
