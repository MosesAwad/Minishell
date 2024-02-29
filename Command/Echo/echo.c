

# include "../../minishell.h"

int	ft_echo(t_minishell *shell, t_command *cmd)
{
	int		i;
	int		flag;

	i = 1;
	flag = 0;
	if (cmd->cmd_args[i] && !ft_strncmp("-n", cmd->cmd_args[i],
		ft_strlen(cmd->cmd_args[i])))
	{
		flag = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		ft_putstr_fd(cmd->cmd_args[i], 1);
		if (cmd->cmd_args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', 1);
	return (shell->exit_status = 0, 0);
}
