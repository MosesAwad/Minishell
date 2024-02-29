/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:41:01 by mawad             #+#    #+#             */
/*   Updated: 2023/07/06 18:01:29 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		*buffer;
	unsigned char		character;
	size_t				i;

	buffer = (unsigned char *) s;
	character = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (buffer[i] == character)
		{
			return (buffer + i);
		}
		i++;
	}
	return (NULL);
}
