
#include "../../minishell.h"

static int	bool_key_match(char *s1, char *s2)
{
	char	*temp;
	int		i;

	i = 0;
	temp = (char *)malloc(ft_strlen(s1) + 1);
	while (s1[i])
	{
		if (s1[i] == '=')
			break ;
		temp[i] = s1[i];
		i++;
	}
	temp[i] = '\0';
	if (!ft_strncmp(temp, s2, ft_strlen(s2) + 1))
		return (free(temp), 1);
	return (free(temp), 0);
}

char	*copy_new_environ(char *env_var, char *new_val)
{
	char	*buffer;
	int		i;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	buffer = (char *)malloc(i + 1 + ft_strlen(new_val) + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	while (env_var[i] && env_var[i] != '=')
	{
		buffer[i] = env_var[i];
		i++;
	}
	buffer[i++] = '=';
	ft_strlcpy(buffer + i, new_val, ft_strlen(new_val) + 1);
	free(env_var);
	return (buffer);
}

//Basically, the purpose of this function is sometimes
//when you start bash (I guess it depends on cache or
//something like that), OLDPWD is not there. It gets
//created whenever you use cd. So to mimic that, we
//have to use add_old_pwd whenever cd is correctly
//called in case it wasn't there when we first started
//our minishell. If we didn't have add_old_pwd, then
//we would never have an OLDPWD in our minishell.
//In buffer[j] = (char *)malloc(ft_strlen("OLDPWD") + 
//ft_strlen(new_val) + 2); The plus 2 is because I want
//to give space for the '=' sign and the null terminator
char	**add_old_pwd(char **env_list, char *new_val)
{
	char	**buffer;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env_list[i])
		i++;
	buffer = (char **)malloc(sizeof(char *) * (i + 1 + 1));
	i = 0;
	while (env_list[i])
	{
		buffer[j++] = ft_strdup(env_list[i]);
		free(env_list[i++]);
	}
	buffer[j] = (char *)malloc(ft_strlen("OLDPWD")
		+ ft_strlen(new_val) + 2);
	ft_strlcpy(buffer[j], "OLDPWD", 7);
	buffer[j][6] = '=';
	ft_strlcpy(buffer[j++] + 7, new_val, ft_strlen(new_val) + 1);
	buffer[j] = NULL;
	return (free(env_list), buffer);
}

char	**update_env_list(char **env_list, char *var, char *new_val)
{
	char	**buffer;
	int		i;

	buffer = env_list;
	if (!env_list || !var || !new_val)
		return (buffer);
	i = 0;
	while (env_list[i])
	{
		if (bool_key_match(env_list[i], var))
			break ;
		i++;
	}
	if (!env_list[i] && !ft_strncmp(var, "OLDPWD", ft_strlen(var)))
	{
		buffer = add_old_pwd(env_list, new_val);	
		return (buffer);
	}
	if (!env_list[i])
		return (buffer);
	env_list[i] = copy_new_environ(env_list[i], new_val);
	return (buffer);
}

int	count_cd_args(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd_args[i])
		i++;
	return (i - 1);
}

char	*get_value(char **env_list, char *str)
{
	char	*buffer;
	int		i;
	int		j;

	i = 0;
	if (!env_list || !str)
		return (NULL);
	while (env_list[i])
	{
		if (bool_key_match(env_list[i], str))
			break;
		i++;
	}
	if (!env_list[i])
		return (NULL);
	j = 0;
	while (env_list[i][j] && env_list[i][j] != '=')
		j++;
	if (!env_list[i][j])
		return (NULL);
	buffer = ft_strdup(env_list[i] + j + 1);
	if (!buffer)
		warn_message();
	return (buffer);
}

//This is to handle cases where the user says [cd ~]
//or even [cd ~/path/part1/part2/etc]. That is because
//you can also go to home like this and not just by
//saying [cd] on its own like that.
char	*go_to_home(t_minishell *shell, char *str)
{
	char	*new_dir;
	int		i;
	int		j;
	char	*home_path;
	int		len;

	home_path = get_value(shell->env, "HOME");
	if (!home_path)
		return (printf("-minishell: cd: HOME not set\n"), NULL);
	len = ft_strlen(home_path) + (ft_strlen(str) - 1); 
	new_dir = (char *)malloc(len + 1);
	ft_strlcpy(new_dir, home_path, ft_strlen(home_path) + 1);
	j = ft_strlen(home_path);
	i = 1;
	while (str[i])
		new_dir[j++] = str[i++];
	new_dir[j] = '\0';
	return (free(home_path), new_dir);
}

//This is to handle when the user says [cd -] which
//prints the OLDPWD path in the terminal and it actually
//takes you back to the OLDPWD
char	*go_to_oldpwd(t_minishell *shell)
{
	char	*new_dir;
	char	*oldpwd_path;

	oldpwd_path = get_value(shell->env, "OLDPWD");
	if (!oldpwd_path)
		return (printf("-minishell: cd: OLDPWD not set\n"), NULL);
	new_dir = ft_strdup(oldpwd_path);
	printf("%s\n", new_dir);
	return (free(oldpwd_path), new_dir);
}

//In the if (arg_count == 0 || (arg_count == 1 && !(cmd->cmd_args[1][0])))
//statement, the second half of the or statement is there to check if the
//user does [cd ""] or [cd $FOO] (where FOO is not a valid envar and thus,
//it expands to an empty string, rednering it the same as [cd ""]); in such
//scenarios, cd actually takes you to the home directory. The first half is
//pretty obvious, if you just do [cd], then it also takes you to the home
//directory.
char	*assign_new_dir(t_minishell *shell, t_command *cmd)
{
	char	*home_path;
	char	*new_dir;
	int		arg_count;

	new_dir = NULL;
	arg_count = count_cd_args(cmd);
	if (arg_count > 1)
		return (printf("-minishell: cd: too many arguments\n"), NULL);
	if (arg_count == 0 || (arg_count == 1 && !(cmd->cmd_args[1][0])))
	{
		home_path = get_value(shell->env, "HOME");
		if (!home_path)
			return (printf("-minishell: cd: HOME not set\n"), NULL);
		new_dir = home_path;
	}
	else if (arg_count == 1 && cmd->cmd_args[1][0])
	{
		if (!ft_strncmp(cmd->cmd_args[1], "-", 2))
			new_dir = go_to_oldpwd(shell);
		else if (!ft_strncmp(cmd->cmd_args[1], "~", 1))
			new_dir = go_to_home(shell, cmd->cmd_args[1]);
		else
			new_dir = ft_strdup(cmd->cmd_args[1]);
	}
	return (new_dir);
}

int	ft_cd(t_minishell *shell, t_command *cmd)
{
	char	*current_dir;
	char	*new_dir;

	shell->exit_status = 0;
	current_dir = getcwd(NULL, 0);
	if (!cmd->cmd_args)
		return (free(current_dir), 1);
	new_dir = assign_new_dir(shell, cmd);
	if (new_dir)
	{
		if (chdir(new_dir) == -1)
			return (shell->exit_status = 1,
				printf("-minishell: cd: %s: No such file or directory\n",
					new_dir), free(new_dir), free(current_dir), 1);
		shell->env = update_env_list(shell->env, "OLDPWD", current_dir);
		free(current_dir);
		current_dir = getcwd(NULL, 0);
		shell->env = update_env_list(shell->env, "PWD", current_dir);
	}
	else
		shell->exit_status = 1;
	return (free(current_dir), free(new_dir), 0);
}
