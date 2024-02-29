

# include "../../minishell.h"

int	ft_pwd(t_minishell *shell)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (shell->exit_status = 1, 1);
	printf("%s\n", current_dir);
	free(current_dir);
	return (shell->exit_status = 0, 0);
}
