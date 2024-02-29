
# include "../../minishell.h"

//If you have a variable like HI=2 in your env list
//but then you say unset HI=23. HI=2 remains in your
//env list. So basically the way it works is it just
//loops through the keys of the environment list (so
//it loops until it means an equal sign or the null
//terminator) and then it checks if that key's a full match
//with the argument that you gave to unset (the whole
//argument, it doesn't give a shit about key value
//pairs or anything for the argument). If it's a match
//between the whole argument AND the KEY of any member
//in the environ list, it deletes that member of the
//env list. To further prove that, if you have an env
//variable HI=42 and then you say unset HI=42, it ain't
//gonna work. You have to say unset HI only! Also unset
//can also takes multiple arguments at once, so you can
//unset multiple environment variables at a time.
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

static int	is_key_match(char **env_list, char *str)
{
	int	i;

	i = 0;
	while (env_list[i])
	{
		if (bool_key_match(env_list[i], str))
			return (1);
		i++;
	}
	return (0);
}

static char	**rmv_from_list(char **env_list, char *str)
{
	char	**buffer;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	buffer = NULL;
	while (env_list[i])
		i++;
	buffer = (char **)malloc(sizeof(char *) * (i - 1 + 1));
	i = 0;
	while (env_list[i])
	{
		if (!bool_key_match(env_list[i], str))
		{
			len = ft_strlen(env_list[i]);
			buffer[j] = (char *)malloc(len + 1);
			ft_strlcpy(buffer[j++], env_list[i], len + 1);
		}
		free(env_list[i]);
		i++;
	}
	buffer[j] = NULL;
	return (free(env_list), buffer);
}

//Unset using the bash on the macs at 42 does not work
//if you give it an argument other than a valid key. It
//wont accept a key-value pair though. So it won't accept
//an equal sign followed by any value of your choice.
//So unset 7NAME or unset NAME=hey (even if it is available
//in the environ list) won't work. Only something like unset _NAME
//or unset NAME would work. I believe that different bash versions
//handle this differently. So for example, the WSL bash simply
//ignores something like 7NAME or unset NAME=hey (even if it is 
//available in the environ list). It just wouldn't do anything and
//no error message is displayed. The only thing it will accept is a
//valid key (JUST KEY) similar to the bash version at 42 macs. I 
//chose to replicate the behavior of the macts at 42, which is
//bash 3.2
static int	valid_syntax(char *str)
{
	int	i;

	i = 0;
	if (!(str[i] == '_' || ft_isalpha(str[i])))
			return (0);
	i++;
	while (str[i])
	{
		if (!(str[i] == '_' || ft_isalnum(str[i])))
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(t_minishell *shell, t_command *cmd)
{
	int		i;

	i = 1;
	shell->exit_status = 0;
	while (cmd->cmd_args[i])
	{
		if (valid_syntax(cmd->cmd_args[i]))
		{
			if (is_key_match(shell->env, cmd->cmd_args[i]))
				shell->env = rmv_from_list(shell->env, cmd->cmd_args[i]);
		}
		else
		{
			printf("-minishell: unset: '%s': not a valid identifier\n", cmd->cmd_args[i]);
			shell->exit_status = 1;
		}
		i++;
	}
	return (shell->exit_status);
}
