/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:37:10 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/05 17:37:01 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	ft_errno(char err)
{
	static char	ft_errnox;

	if (err == 1)
		exit_status(1);
	if (err != -1)
		ft_errnox = err;
	return (ft_errnox);
}

t_sig	*state(void)
{
	static t_sig	state;

	return (&state);
}

void	bash_signal(int sig)
{
	t_sig	*stat;

	stat = state();
	(void)sig;
	exit_status(128 + SIGINT);
	if (!stat->is_excution && !stat->is_heredoc)
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (!stat->is_heredoc)
		ft_putstr_fd("\n", 1);
}
