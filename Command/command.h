/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:35:51 by mawad             #+#    #+#             */
/*   Updated: 2024/02/23 17:37:38 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

//Export/export_w_flags_utils.c
int		check_export_syntax(char *str);
int		bool_same_key(char *s1, char *s2);
char	*update_value(char *s1, char *s2);
int		check_same_key(char **env_list, char *str);

//Export/export_w_flags.c
int		bool_no_update(char *s1, char *s2);
int		check_no_update(char **env_list, char *str);
char	**add_to_loc_list(char **env_list, char *str);


#endif