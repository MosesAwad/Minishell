/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 10:38:29 by mawad             #+#    #+#             */
/*   Updated: 2024/03/03 10:38:29 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	trailor_validity(char c)
{
	if (ft_isalpha(c))
		return (1);
	if (c == '_')
		return (1);
	if (ft_isdigit(c))
		return (1);
	return (0);
}

int	choose_state2(char c, int state)
{
	if (c == '\'' && state == STATE_GENERAL)
		return (IN_SQUOTE);
	else if (c == '\'' && state == IN_SQUOTE)
		return (STATE_GENERAL);
	if (c == '"' && state == STATE_GENERAL)
		return (IN_DQUOTE);
	else if (c == '"' && state == IN_DQUOTE)
		return (STATE_GENERAL);
	return (state);
}

//THIS FUNCTION IS NOW USELESS BECAUSE BY THE TIME I START EXPANDING
//ALL THE DOLLARS I DON'T WANT ARE NOT MARKED ANYWAYS SO THEY WILL BE
//IGNORED AND I AM CHANGING EACH DOLLAR AT A TIME. SO BY THE TIME I REACH
//THE DOLLAR I WANT, THERE WON'T BE ANY PRECEDING DOLLARS BECAUSE EVERYTHING
//BEFORE IT HAS ALREADY BEEN EXPANDED AND THE UNEXPANDABLE DOLLARS WHICH WERE
//IN SINGLE QUOTES ARE UNMARKED. THIS IS A REMNANT OF MY PREVIOUS CODE WHICH
//HAD AN ABSOLUTE WHACK DESIGN. BUT I CAN STILL USE IT EVEN THOUGH IT'S USELSS.
//IF YOU WANT REMOVE IT LATER ON BUT THIS FUNCTION WAS THERE TO ENSURE THAT THE
//DOLLAR SIGN I AM EXPANDING NOW IS THE SAME ONE I FOUND IN THE LIST BECAUSE I
//DONT WANT IT TO EXPAND THE UNEXPANDABLE ONES, BUT THE THING IS MY UNEXPANDABLE
//ONES ARE NOT MARKED ANYWAYS, SO I DO NOT EVEN TOUCH THEM.
int	in_env_string(char *str1, char *env_str)
{
	int	j;

	j = 0;
	while ((env_str[j] != '=') && (str1[j] == env_str[j])
		&& str1[j] && env_str[j])
		j++;
	if (env_str[j] == '=')
		return (j);
	return (0);
}

//THIS FUNCTION COPIES THE EXPANSION OF THE TRAILOR AND THEN WHATEVER
//COMES AFTER THE TRAILOR IN THE STRING. EXTRA INFO: THE i += 2 LINE
//IN THIS FUNCTION IS THERE TO SKIP THE ${ IN THE ACTUAL STRING SO
//THAT THEY DO NOT GET COPIED
static void	copy_expansion(char *buffer, char *str, char *env)
{
	int	k;
	int	i;
	int	j;

	k = 0;
	i = 0;
	j = 0;
	while (env[k] != '=')
		k++;
	k++;
	while (env[k])
		buffer[j++] = env[k++];
	i += 2;
	while (trailor_validity(str[i]))
		i++;
	while (str[i])
		buffer[j++] = str[i++];
	buffer[j] = '\0';
}

//THIS FUNCTION COPIES UP THE STRING UP UNTIL THE ${, IT THEN SKIPS
//THE ${ AND GOES OVER TO copy_expansion() FUNCTION
void	copier(char *buffer, char *str, char *env)
{
	int	i;
	int	j;
	int	state;

	i = 0;
	j = 0;
	state = STATE_GENERAL;
	while (str[i])
	{
		state = choose_state2(str[i], state);
		if (not_stand_alone(str[i], str[i + 1], state))
		{
			if (in_env_string(str + i + 1, env))
				break ;
		}
		buffer[j++] = str[i++];
	}
	copy_expansion(buffer + j, str + i, env);
}
