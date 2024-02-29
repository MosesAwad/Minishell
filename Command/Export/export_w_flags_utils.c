/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_w_flags_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:18:28 by mawad             #+#    #+#             */
/*   Updated: 2024/02/29 18:08:43 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

static int	bool_syntax_validity(char c)
{
	if (ft_isalpha(c))
		return (1);
	if (c == '_')
		return (1);
	if (ft_isdigit(c))
		return (1);
	return (0);
}

//applies the bool_syntax_validity function
//to the export arguments
int	check_export_syntax(char *str)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(str[i]) || str[i] == '_'))
	{
		printf("-minishell: export: '%s': not a valid identifier\n", str);
		return (0);
	}
	i++;
	while (str[i] != '=' && str[i])
	{
		if (!bool_syntax_validity(str[i]))
		{
			printf("-minishell: export: '%s': not a valid identifier\n", str);
			return (0);
		}
		i++;
	}
	return (1);
}

static char	*extract_key(char *str)
{
	char	*buffer;
	int		i;

	i = 0;
	buffer = (char *)malloc(ft_strlen(str) + 1);
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		buffer[i] = str[i];
		i++;
	}
	buffer[i] = '\0';
	return (buffer);
}

//For an update to happen, there has to be an equal
//sign in both s1 and s2. Simple as that. Otherwise,
//no update happens. So if I have HI=5 then I do
//export HI, nothing happens. For detecting that
//scenario, go to function bool_no_update()
int	bool_same_key(char *s1, char *s2)
{
	char	*key1;
	char	*key2;

	key1 = extract_key(s1);
	key2 = extract_key(s2);
	if (!ft_strncmp(key1, key2, ft_strlen(key1) + 1))
		return (free(key1), free(key2), 1);
	return (free(key1), free(key2), 0);
}

char	*update_value(char *s1, char *s2)
{
	char	*buffer;
	int		i;

	i = 0;
	while (s2[i])
		i++;
	buffer = (char *)malloc(i + 1);
	ft_strlcpy(buffer, s2, i + 1);
	free(s1);
	return (buffer);
}

//applies the bool_same_key to the export arguments
//to see whether the key of the argument is already
//present in the list. If yes, then no need to add
//a new element to the list, just update that element
//in the list using the update_value function above
int	check_same_key(char **env_list, char *str)
{
	int	i;

	i = 0;
	while (env_list[i])
	{
		if (bool_same_key(env_list[i], str))
		{
			env_list[i] = update_value(env_list[i], str);
			return (1);
		}
		i++;
	}
	return (0);
}
