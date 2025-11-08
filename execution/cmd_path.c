/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:45:50 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/04 10:15:50 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	print_error(int error_nb, int error, char *cmd)
{
	if (error)
	{
		if (error_nb == 1)
			ft_putstr_fd(ft_strjoin(cmd, ": No such file or directory\n"), 2);
		else if (error_nb == 2)
			ft_putstr_fd("Command '' not found\n", 2);
		else if (error_nb == 3)
			ft_putstr_fd(ft_strjoin(cmd, ": Is a directory\n"), 2);
		else if (error_nb == 4)
		{
			ft_putstr_fd(ft_strjoin(cmd, ": command not found\n"), 2);
			exit_status(127);
		}
	}
}

char	*the_real_finder(t_eflow *flow, char **path, char *cmd_path, int error)
{
	char		no_acess;
	int			i;
	struct stat	x;

	i = -1;
	no_acess = 0;
	while (path[++i])
	{
		cmd_path = ft_pathjoin(path[i], flow->cmd);
		stat(cmd_path, &x);
		if (access(cmd_path, F_OK | X_OK) == 0 && !S_ISDIR(x.st_mode))
			return (cmd_path);
		if (errno == EACCES)
			no_acess = 1;
	}
	if (no_acess && error)
	{
		errno = EACCES;
		perror(flow->cmd);
		exit_status(126);
	}
	else
		print_error(4, error, flow->cmd);
	return (NULL);
}

static char	*check_everything(char *cmd, int error, char *cmd_path)
{
	if (access(cmd_path, F_OK | X_OK) == 0)
		return (cmd_path);
	if (error)
	{
		if (errno == ENOENT)
			exit_status(127);
		else
			exit_status(126);
		perror(cmd);
	}
	return (NULL);
}

static char	*is_dir(char *cmd, char *tmp, char *cmd_path, int error)
{
	struct stat	x;

	ft_memset(&x, 0, sizeof(x));
	stat(cmd, &x);
	if (S_ISDIR(x.st_mode))
	{
		print_error(3, error, cmd);
		if (error)
			exit_status(126);
		return (NULL);
	}
	if (cmd[0] == '.' && cmd[1] == '/')
	{
		tmp = getcwd(NULL, 0);
		cmd_path = ft_strjoin(tmp, cmd + 1);
		free(tmp);
		return (check_everything(cmd, error, cmd_path));
	}
	return (check_everything(cmd, error, cmd));
}

char	*path_finder(t_eflow *flow, char **env, int error)
{
	char	**path;
	char	*cmd_path;
	char	*tmp;

	cmd_path = NULL;
	tmp = NULL;
	if (ft_strchr(flow->cmd, '/'))
		return (is_dir(flow->cmd, tmp, cmd_path, error));
	path = ft_split(env_path(env), ':');
	if (!path || !flow->cmd[0])
	{
		if (!path)
			print_error(1, error, flow->cmd);
		else if (!flow->cmd[0])
			print_error(2, error, 0);
		if (error)
			exit_status(127);
		return (NULL);
	}
	cmd_path = the_real_finder(flow, path, cmd_path, error);
	return (cmd_path);
}
