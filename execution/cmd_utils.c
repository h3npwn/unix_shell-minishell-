/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:36:28 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/04 10:35:03 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_pathjoin(char const *s1, char const *s2)
{
	char	*new;
	size_t	i;
	size_t	j;
	size_t	k;

	if (!s1 || !s2)
		return (NULL);
	j = ft_strlen(s1) + ft_strlen(s2) + 2;
	new = (char *)heap_manager(sizeof(char) * j, 'x', 0);
	if (!new)
		return (NULL);
	j = 0;
	k = 0;
	i = 0;
	while (s1[k])
		new[i++] = s1[k++];
	new[i++] = '/';
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = '\0';
	return (new);
}

char	*env_path(char **env)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			path = ft_strnstr(env[i], "PATH=", 5);
			break ;
		}
		i++;
	}
	if (!path)
		return (NULL);
	return (path + 5);
}

void	set_env(t_eflow *flow, t_minishell *bash, int f, int i)
{
	char	*cmd_path;
	char	*arr[3];

	arr[0] = "export";
	arr[2] = NULL;
	if (f)
	{
		while (flow->arg[i] && flow->arg[i + 1])
			i++;
		if (!i)
			arr[1] = ft_strjoin("_=", flow->cmd);
		else
			arr[1] = ft_strjoin("_=", flow->arg[i]);
		export(arr, bash->env, bash->env_arr, 0);
	}
	else if (flow->cmd)
	{
		cmd_path = path_finder(flow, bash->env_arr, 0);
		if (cmd_path)
			arr[1] = ft_strjoin("_=", cmd_path);
		else
			arr[1] = ft_strjoin("_=", flow->cmd);
		export(arr, bash->env, bash->env_arr, 0);
	}
}
