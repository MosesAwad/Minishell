/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:18:32 by mawad             #+#    #+#             */
/*   Updated: 2023/12/30 17:18:18 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	spcsign_loop(const char *nptr, int *i, int *negative_flag)
{
	while ((nptr[*i] >= 9 && nptr[*i] <= 13) || nptr[*i] == 32)
		(*i)++;
	if (nptr[*i] == '+' || nptr[*i] == '-')
	{
		if (nptr[*i] == '-')
			*negative_flag *= -1;
		(*i)++;
	}
}

int	ft_atoi(const char *nptr)
{
	int				i;
	int				negative_flag;
	int				digit;
	long long int	store;

	i = 0;
	negative_flag = 1;
	store = 0;
	spcsign_loop(nptr, &i, &negative_flag);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - 48;
		if ((store > (9223372036854775807 - digit) / 10) && negative_flag == 1)
			return (-1);
		if ((store > (9223372036854775807 - digit) / 10) && negative_flag == -1)
			return (0);
		store = store * 10 + digit;
		i++;
	}
	return ((int)(store * negative_flag));
}
