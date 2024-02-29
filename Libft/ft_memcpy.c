/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 15:02:29 by mawad             #+#    #+#             */
/*   Updated: 2023/07/12 21:13:00 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_buffer;
	const unsigned char	*src_buffer;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	dest_buffer = (unsigned char *)dest;
	src_buffer = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dest_buffer[i] = src_buffer[i];
		i++;
	}
	return (dest);
}
