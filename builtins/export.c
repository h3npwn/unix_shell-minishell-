/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:37:56 by abahja            #+#    #+#             */
/*   Updated: 2025/06/03 15:29:11 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	add_exist(t_env *env, char *str)
{
	t_env	*t;
	char	*key;

	key = ft_strdup(str);
	if (ft_strchr(key, '='))
		*ft_strchr(key, '=') = 0;
	t = env;
	while (t)
	{
		if (ft_strcmp(t->key, key) == 0)
		{
			if (!ft_strchr(str, '='))
				return (1);
			heap_manager(0, 'n', t->value);
			t->value = ft_envstrdup(ft_strchr(str, '='));
			return (1);
		}
		t = t->next;
	}
	heap_manager(0, 'r', key);
	return (0);
}

void	sort(char *arr[])
{
	char	*key;
	int		i;
	int		j;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				key = arr[i];
				arr[i] = arr[j];
				arr[j] = key;
			}
			j++;
		}
		i++;
	}
}

void	print_it(char **arg)
{
	int		i;

	i = 0;
	sort(arg);
	i = 0;
	while (arg[i])
	{
		if (ft_strchr(arg[i], '='))
		{
			printf("declare -x %.*s\"%s\"\n",
				(int)(ft_strchr(arg[i], '=') - arg[i] + 1),
				arg[i], ft_strchr(arg[i], '=') + 1);
		}
		else
			printf("declare -x %s\n", arg[i]);
		i++;
	}
}

int	valid_key(char *key, int f, int i)
{
	char	*stop;
	char	*err;

	err = ft_strjoin("`", key);
	err = ft_strjoin(err, "': not a valid identifier\n");
	stop = ft_strchr(key, '=');
	if (!stop)
		stop = "";
	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		if (!f)
			return (0);
		return (ft_putstr_fd(err, 2), 0);
	}
	while (key[i] && key[i] != *stop)
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			if (!f)
				return (0);
			return (ft_putstr_fd(err, 2), 0);
		}
		i++;
	}
	return (1);
}

void	export(char **arg, t_env *env, char **env_arr, int update)
{
	int	i;
	int	f;

	i = 0;
	f = 0;
	if (!arg[1])
		print_it(env_arr);
	while (arg[++i])
	{
		if (valid_key(arg[i], update, 1))
		{
			if (add_exist(env, arg[i]) != 1)
				add_back(&env, create(arg[i]));
		}
		else
			f = 1;
	}
	if (update)
		exit_status(f);
}
