/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excut_label.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahja <abahja@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 08:50:11 by abahja            #+#    #+#             */
/*   Updated: 2025/06/03 11:18:53 by abahja           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	is_builtin(t_eflow *target)
{
	int		i;
	char	*b_ins[8];

	b_ins[0] = "pwd";
	b_ins[1] = "cd";
	b_ins[2] = "echo";
	b_ins[3] = "export";
	b_ins[4] = "unset";
	b_ins[5] = "env";
	b_ins[6] = "exit";
	b_ins[7] = NULL;
	i = 0;
	while (b_ins[i])
	{
		if (ft_strcmp(b_ins[i], target->cmd) == 0)
		{
			target->is_built_in = 1;
			return ;
		}
		i++;
	}
}

int	count_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**env_setup(t_minishell *bash)
{
	char	**new_env;
	t_env	*env;
	int		size;
	int		i;

	env = NULL;
	if (bash->env)
		env = bash->env->next;
	if (!env)
	{
		ft_putstr_fd("", 1);
		return (NULL);
	}
	size = count_size(env);
	new_env = (char **)heap_manager(sizeof(char *) * (size + 1), 'x', NULL);
	i = 0;
	while (env)
	{
		new_env[i] = ft_strjoin(env->key, env->value);
		env = env->next;
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	execution_trigger(t_minishell *bash, t_eflow *flows, int f)
{
	if (ft_strcmp(flows->cmd, "export") == 0)
		export(flows->arg, bash->env, bash->env_arr, 1);
	else if (ft_strcmp(flows->cmd, "env") == 0)
		env(bash->env_arr, flows->arg);
	else if (ft_strcmp(flows->cmd, "cd") == 0)
		cd(flows->arg, bash);
	else if (ft_strcmp(flows->cmd, "pwd") == 0)
		pwd(bash);
	else if (ft_strcmp(flows->cmd, "echo") == 0)
		echo(flows->arg);
	else if (ft_strcmp(flows->cmd, "unset") == 0)
		unset(bash, flows->arg);
	else if (ft_strcmp(flows->cmd, "exit") == 0)
		exit_bash(flows->arg);
	if (f)
		final_label(exit_status(-1));
}

void	no_need(t_eflow *flow)
{
	int		i;
	char	**new_arg;

	if (ft_strcmp(flow->cmd, "/usr/bin/ls") && ft_strcmp(flow->cmd, "/bin/ls"))
		return ;
	i = 0;
	while (flow->arg[i])
		i++;
	new_arg = heap_manager(sizeof(char *) * (i + 2), 'x', 0);
	i = 0;
	while (flow->arg[i])
	{
		new_arg[i] = flow->arg[i];
		i++;
	}
	new_arg[i++] = ft_strdup("--color=tty");
	new_arg[i] = NULL;
	flow->arg = new_arg;
}
