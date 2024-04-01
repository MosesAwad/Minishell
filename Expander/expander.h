/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:58:53 by mawad             #+#    #+#             */
/*   Updated: 2024/03/04 15:58:53 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

//expand_str_or_toklist.c
char	*q_expand_str(char *str, char *env[], int exit_status);
char	*expand_str(char *str, char *env[], int exit_status);
void	expand_toklist(t_token *tok_list, char *env[], int exit_status);

//expander_cases1.c
char	*delpotenvar(char *str, int state);
char	*keep_standalone(char *str, int state);
char	*deldollar(char *str, int state);

//expander_cases2.c
char	*detector(char *str, int s_ind, char *env[]);

//expander_cases3.c
char	*copy_exit_status(char *str, int exit_status);
char	*delnumber(char *str, int state);

//expander_utils.c
int		trailor_validity(char c);
int		choose_state2(char c, int state);
int		in_env_string(char *str1, char *env_str);
void	copier(char *buffer, char *str, char *env);

//expander_utils2.c
int		not_stand_alone(char c1, char c2, int state);

//expander.c
char	*expansion(char *str, char *env[], int exit_status);

//strip_quotes.c
char	*strip_quotes(char *str);

#endif