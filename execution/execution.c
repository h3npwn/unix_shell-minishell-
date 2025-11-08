/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 09:36:57 by abahja            #+#    #+#             */
/*   Updated: 2025/06/06 16:06:43 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

pid_t	forking(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		fd_backup(0, 0, 1);
	if (pid == -1)
	{
		ft_putstr_fd("fatal error : fork\n", 2);
		ft_errno(1);
	}
	return (pid);
}

int	redir_in(char *filename, int here_fd)
{
	int	fd;
	int	c;

	if (here_fd == -1)
		fd = open(filename, O_RDONLY);
	else
		fd = here_fd;
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	c = dup2(fd, STDIN_FILENO);
	close(fd);
	if (c == -1)
		return (0);
	return (1);
}

int	redir_out(char *filename, char is_append)
{
	int	fd;
	int	c;

	if (is_append)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	c = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (c == -1)
		return (0);
	return (1);
}

int	set_redirections(t_eflow *flow)
{
	t_re	*block;

	block = flow->redirs;
	while (block)
	{
		if (block->f_ambiguous && block->f_ambiguous != 3)
		{
			ft_putstr_fd("\033[1;31mambiguous redirect\033[0m\n", 2);
			return (0);
		}
		if (block->type == RED_IN || block->type == HEREDOC)
		{
			if (!redir_in(block->filename, block->hd_fd))
				return (0);
		}
		else if (block->type == RED_OUT || block->type == APPEND)
		{
			if (!redir_out(block->filename, (block->type == APPEND)))
				return (0);
		}
		block = block->next;
	}
	return (1);
}

void	minishell(t_minishell *bash)
{
	state()->is_excution = 1;
	bash->env_arr = env_setup(bash);
	if (!bash->flow->next && bash->flow->is_built_in)
	{
		set_env(bash->flow, bash, 0, 0);
		bash->env_arr = env_setup(bash);
		if (!set_redirections(bash->flow))
			exit_status(1);
		else
		{
			execution_trigger(bash, bash->flow, 0);
			set_env(bash->flow, bash, 1, 0);
		}
		return ;
	}
	else if (bash->flow->next)
		pipe_cmd(bash, 0, -1);
	else
		one_cmd(bash);
}
