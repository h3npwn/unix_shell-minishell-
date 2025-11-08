/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:25:50 by abahja            #+#    #+#             */
/*   Updated: 2025/06/05 17:30:17 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	execve_fail(char *cmd, char **env)
{
	char	*arg[3];

	arg[0] = "/bin/bash";
	arg[1] = cmd;
	arg[2] = NULL;
	if (errno == ENOEXEC)
	{
		execve(arg[0], arg, env);
		final_label(exit_status(1));
	}
	if (errno == EACCES)
	{
		perror(cmd);
		final_label(exit_status(126));
	}
	if (errno == ENOENT)
	{
		perror(cmd);
		final_label(exit_status(127));
	}
	else
	{
		perror(cmd);
		final_label(exit_status(1));
	}
}

static int	check_valid(char *str)
{
	int		i;
	char	*err;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[0] || str[i])
	{
		if (!state()->is_child)
			ft_putstr_fd("exit\n", 2);
		err = ft_strjoin(": exit: ", str);
		err = ft_strjoin(err, ": numeric argument required\n");
		ft_putstr_fd(err, 2);
		return (0);
	}
	return (1);
}

void	exit_bash(char **arg)
{
	if (arg[1])
	{
		if (!check_valid(arg[1]))
			final_label(exit_status(2));
		else if (arg_count(arg) > 2)
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			exit_status(1);
		}
		else
		{
			if (!state()->is_child)
				ft_putstr_fd("exit\n", 2);
			final_label(ft_atoi(arg[1]));
		}
	}
	else
	{
		if (!state()->is_child)
			ft_putstr_fd("exit\n", 2);
		final_label(exit_status(-1));
	}
}
