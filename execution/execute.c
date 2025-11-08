/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:16:11 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/06 13:27:21 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	wait_child(int pid, int *status)
{
	waitpid(pid, status, 2);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(*status))
		exit_status(WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
	{
		if (exit_status(128 + WTERMSIG(*status)) == 131)
			ft_putstr_fd("Quit (core dumped)\n", 2);
	}
}

void	set_pipe(int *fds, int prev_fd)
{
	if (prev_fd != -1)
	{
		check_fail(dup2(prev_fd, STDIN_FILENO));
		close(prev_fd);
	}
	isatty(fds[0]);
	if (errno != EBADF)
		close(fds[0]);
	isatty(fds[1]);
	if (errno != EBADF)
	{
		check_fail(dup2(fds[1], STDOUT_FILENO));
		close(fds[1]);
	}
}

pid_t	execute_cmd(t_eflow *flow, t_minishell *bash, int prev_fd, int *fds)
{
	pid_t	pid;

	pid = forking();
	if (pid == 0)
	{
		set_env(flow, bash, 0, 0);
		bash->env_arr = env_setup(bash);
		set_pipe(fds, prev_fd);
		signal(SIGQUIT, SIG_DFL);
		state()->is_child = 1;
		if (!set_redirections(flow))
			final_label(exit_status(1));
		clear_cache(bash->flow);
		if (!flow->cmd)
			final_label(exit_status(0));
		if (flow->is_built_in)
			execution_trigger(bash, flow, 1);
		flow->cmd = path_finder(flow, bash->env_arr, 1);
		if (!flow->cmd)
			final_label(exit_status(-1));
		no_need(flow);
		execve(flow->cmd, &flow->arg[0], bash->env_arr);
		execve_fail(flow->arg[0], bash->env_arr);
	}
	return (pid);
}

void	pipe_cmd(t_minishell *bash, int status, int prev_fd)
{
	t_eflow	*flow;
	pid_t	pid;
	int		fd[2];

	flow = bash->flow;
	while (flow->next)
	{
		if (pipe(fd) == -1)
		{
			ft_putstr_fd("fatal error : Pipe\n", 2);
			ft_errno(1);
			return ;
		}
		execute_cmd(flow, bash, prev_fd, fd);
		close(fd[1]);
		if (prev_fd != -1)
			close(prev_fd);
		prev_fd = fd[0];
		flow = flow->next;
	}
	pid = execute_cmd(flow, bash, prev_fd, fd);
	close(fd[0]);
	if (ft_errno(-1) == 0)
		wait_child(pid, &status);
}

void	one_cmd(t_minishell *bash)
{
	pid_t	pid;
	int		status;

	set_env(bash->flow, bash, 0, 0);
	bash->env_arr = env_setup(bash);
	pid = forking();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		state()->is_child = 1;
		if (!set_redirections(bash->flow))
			final_label(exit_status(1));
		if (!bash->flow->cmd)
			final_label(exit_status(0));
		bash->flow->cmd = path_finder(bash->flow, bash->env_arr, 1);
		if (!bash->flow->cmd)
			final_label(exit_status(-1));
		no_need(bash->flow);
		execve(bash->flow->cmd, bash->flow->arg, bash->env_arr);
		execve_fail(bash->flow->arg[0], bash->env_arr);
	}
	if (ft_errno(-1) == 0)
		wait_child(pid, &status);
	set_env(bash->flow, bash, 1, 0);
}
