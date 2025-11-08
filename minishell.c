/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 08:59:53 by abahja            #+#    #+#             */
/*   Updated: 2025/06/05 17:35:25 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

void	final_label(long long exit_code)
{
	fd_backup(0, 0, 1);
	rl_clear_history();
	heap_manager(0, 'f', 0);
	heap_manager(0, 'z', 0);
	exit(exit_code);
}

void	handle_doc(int sig)
{
	int	to_close;

	(void)sig;
	to_close = last_open_fd(-1);
	isatty(to_close);
	if (errno != EBADF)
		close(to_close);
	ft_putstr_fd("\n", 1);
	final_label(exit_status(128 + SIGINT));
}

int	main(void)
{
	t_minishell	bash;
	int			main_fd[2];

	fd_backup(&main_fd[0], &main_fd[1], 0);
	signal(SIGINT, bash_signal);
	signal(SIGQUIT, SIG_IGN);
	bash.env = copy_env();
	init_pwd(&bash);
	while (1)
	{
		init_bash(&bash, main_fd);
		bash.prompt = stdin_prompt();
		if (!*bash.prompt)
			continue ;
		if (quotes(bash.prompt))
			the_core(&bash);
		else
			exit_status(1);
	}
}
