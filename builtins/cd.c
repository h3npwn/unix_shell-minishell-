/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:39:07 by abahja            #+#    #+#             */
/*   Updated: 2025/06/02 10:28:28 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	update(t_minishell *bash, char	*curr, char *old)
{
	char	*oldpwd;
	char	*currpwd;
	char	*arg[3];
	char	arg1[7];

	currpwd = ft_strjoin("PWD=", curr);
	ft_memcpy(arg1, "export", 7);
	arg[0] = arg1;
	arg[1] = currpwd;
	arg[2] = NULL;
	if (curr)
		export(arg, bash->env, bash->env_arr, 0);
	oldpwd = ft_strjoin("OLDPWD=", old);
	arg[1] = oldpwd;
	if (old)
		export(arg, bash->env, bash->env_arr, 0);
	heap_manager(0, 'r', currpwd);
	heap_manager(0, 'r', oldpwd);
	exit_status(0);
}

char	*home(char **env, char *str, int *flag)
{
	int		i;
	char	*err;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
			return (ft_strchr(env[i], '=') + 1);
		i++;
	}
	err = ft_strjoin("cd: ", str);
	err = ft_strjoin(err, " not set\n");
	ft_putstr_fd(err, 2);
	*flag = 1;
	return (NULL);
}

void	where_are_we(char *new, t_minishell *bash)
{
	char	*err;

	err = ft_strjoin("cd: error retrieving current ", "directory: getcwd:");
	err = ft_strjoin(err, " cannot access parent directories:");
	err = ft_strjoin(err, " No such file or directory\n");
	ft_putstr_fd(err, 2);
	if (!bash->oldpwd)
		return ;
	new = ft_strdup(bash->oldpwd);
	if (ft_strrchr(new, '/') - new > 0)
		new[ft_strrchr(new, '/') - new] = 0;
	update(bash, new, bash->oldpwd);
	bash->oldpwd = ft_envstrdup(new);
	heap_manager(0, 'r', new);
}

void	cd_helper(t_minishell *bash, int *flag, char **args, char *new)
{
	if (args[1] && !*args[1])
		args[1] = ".";
	if (!(*flag) && chdir(args[1]))
	{
		perror(ft_strjoin("cd : ", args[1]));
		*flag = 1;
	}
	else if (!(*flag))
	{
		new = getcwd(0, 0);
		if (new)
		{
			update(bash, new, bash->oldpwd);
			heap_manager(0, 'n', bash->oldpwd);
			bash->oldpwd = ft_envstrdup(new);
			free(new);
		}
		else
			where_are_we(new, bash);
	}
}

void	cd(char **args, t_minishell *bash)
{
	char	*new;
	char	*tmp;
	int		flag;

	new = NULL;
	tmp = args[1];
	if (arg_count(args) > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		exit_status(1);
		return ;
	}
	flag = 0;
	if (!args[1])
		args[1] = home(bash->env_arr, "HOME=", &flag);
	else if (!ft_strcmp(args[1], "-"))
	{
		args[1] = home(bash->env_arr, "OLDPWD=", &flag);
		if (args[1])
			printf("%s\n", args[1]);
	}
	cd_helper(bash, &flag, args, new);
	args[1] = tmp;
	exit_status(flag);
}
