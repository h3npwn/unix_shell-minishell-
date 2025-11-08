/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 06:30:09 by abahja            #+#    #+#             */
/*   Updated: 2025/05/26 15:46:41 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	list_removeif(t_env **env, char *key)
{
	t_env	*t;
	t_env	*handler;
	t_env	*to_remove;

	t = *env;
	while (t && t->next)
	{
		if (ft_strcmp(t->next->key, key) == 0)
		{
			to_remove = t->next;
			handler = t->next->next;
			t->next = handler;
			heap_manager(0, 'n', to_remove);
			return ;
		}
		t = t->next;
	}
}

void	unset(t_minishell *bash, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		list_removeif(&bash->env, args[i]);
		i++;
	}
	exit_status(0);
}
