/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 13:20:34 by mawad             #+#    #+#             */
/*   Updated: 2024/07/04 17:58:08 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	event(void)
{
	return (0);
}

// This function is inherited by the child process BUT
// SIGQUIT and SIGINT keep their original behaviors in
// the child. So what happens is that let's say from
// minishell I call [cat]. Then without any arguments
// (aka input) it would hang. So when we hit SIGQUIT
// by clicking Ctrl + D in minishell, what happens is
// that the signal goes to both the parent and the child.
// In the child, it actually SIGQUITS the cat process and
// in our main, it does NOT quit minishell but instead, it
// just prints Quit and a \n. That loop then ends because
// the child has finished, its exit status is actually
// picked up by WIFSIGNALED (so the child finished by
// a signal, not WIFEXTED normally) and so, in the waiters
// function that we have in Execution/execution_utils.c, we
// set that to the shell->exit_status and another round of
// the main while (1) loop in main.c begins.
void	child_handler(int sig)
{
	if (sig == SIGQUIT)
		ft_dprintf(STDERR_FILENO, "Quit\n");
	else if (sig == SIGINT)
		ft_dprintf(STDERR_FILENO, "\n");
}

void	init_child_signals(void)
{
	signal(SIGINT, child_handler);
	signal(SIGQUIT, child_handler);
}

// rl_replace_line("", 0); does not do anything!
void	ft_sighandler(int sig)
{
	(void) sig;
	if (g_sigstat == IN_SHELL || g_sigstat == IN_CHILD)
	{
		g_sigstat = 130;
		ft_dprintf(2, "^C\n");
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_done = 1;
	}
	else if (g_sigstat == IN_HDOC)
	{
		g_sigstat = OUT_HDOC;
		ft_dprintf(2, "\n");
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_done = 1;
	}
}

void	init_signals(void)
{
	rl_catch_signals = 0;
	rl_event_hook = event;
	signal(SIGINT, ft_sighandler);
	signal(SIGQUIT, SIG_IGN);
}
