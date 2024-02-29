/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 15:42:40 by mawad             #+#    #+#             */
/*   Updated: 2023/07/10 21:55:39 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_buffer;
	const unsigned char	*src_buffer;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	dest_buffer = (unsigned char *)dest;
	src_buffer = (const unsigned char *)src;
	i = 0;
	if (dest > src)
	{
		while (n > 0)
		{
			dest_buffer[n - 1] = src_buffer[n - 1];
			n--;
		}
		return (dest);
	}
	while (i < n)
	{
		dest_buffer[i] = src_buffer[i];
		i++;
	}
	return (dest);
}
