/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:38:48 by abahja            #+#    #+#             */
/*   Updated: 2025/06/03 14:17:44 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

extern char	**environ;

t_env	*create(char *s)
{
	t_env	*new;

	new = heap_manager(sizeof(t_env), 'e', NULL);
	new->key = ft_envsubstr(s, 0, ft_strchr(s, '=') - s);
	if (ft_strchr(s, '='))
		new->value = ft_envstrdup(ft_strchr(s, '='));
	else
		new->value = ft_envstrdup(" ");
	new->next = NULL;
	return (new);
}

void	add_back(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!head || !(*head))
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	increment_shlvl(char **str)
{
	int		shlvl;
	char	*err;

	shlvl = ft_atoi(*str + 1);
	if (shlvl < 999)
		*str = ft_envstrjoin("=", ft_itoa(shlvl + 1));
	else
	{
		err = ft_strjoin("warning: shell level (1000) too high,",
				"resetting to 1\n");
		ft_putstr_fd(err, 2);
		*str = ft_envstrjoin("=", ft_itoa(1));
	}
}

t_env	*copy_env(void)
{
	int		i;
	t_env	*env;
	t_env	*new;
	char	x;

	x = 0;
	i = 0;
	env = create("");
	new = NULL;
	while (environ[i])
	{
		new = create(environ[i]);
		if (ft_strncmp("SHLVL", new->key, 5) == 0)
		{
			increment_shlvl(&new->value);
			x = 1;
		}
		add_back(&env, new);
		i++;
	}
	if (!x)
		add_back(&env, create("SHLVL=1"));
	return (env);
}
