/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_flow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:57:51 by abahja            #+#    #+#             */
/*   Updated: 2025/06/05 15:37:49 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	**alloc_arg(t_token	*t, char **split, char **args)
{
	int		i;
	int		j;

	i = 0;
	while (t && t->type != PIPE)
	{
		if (!t->visited)
		{
			if (t->ambiguous || t->expanded)
			{
				split = bash_split(t->brick, t->bin);
				if (split && *split)
				{
					j = i;
					while (split && split[i - j])
						i++;
				}
			}
			else
				i++;
		}
		t = t->next;
	}
	args = heap_manager(sizeof(char *) * (i + 1), 'x', NULL);
	return (ft_memset(args, 0, sizeof(char *) * (i + 1)), args);
}

void	cmd_setup(t_eflow *flow, t_token *t, int *i, char **args)
{
	int	j;

	if (!*i)
	{
		if (args && ((t->ambiguous || t->expanded) && !t->assign))
			flow->cmd = ft_strdup(args[0]);
		else
			flow->cmd = ft_strdup(t->brick);
		is_builtin(flow);
	}
	if (((t->ambiguous || t->expanded) && !t->assign) && args && *args)
	{
		j = -1;
		while (args[++j])
			flow->arg[(*i)++] = ft_strdup(args[j]);
	}
	else if (t->brick)
		flow->arg[(*i)++] = ft_strdup(t->brick);
}

void	set_cur(t_eflow *flow, t_token **t)
{
	while (*t && (*t)->visited)
		*t = (*t)->next;
	flow->arg = alloc_arg(*t, NULL, NULL);
}

void	check_cmd(t_eflow *flow, t_token *t, char **args, int i[2])
{
	set_cur(flow, &t);
	while (t && t->type != PIPE)
	{
		if (!t->visited)
		{
			t->visited = 1;
			if (!flow->cmd || ft_strcmp(flow->cmd, "export"))
				t->assign = 0;
			if (!t->assign && (t->ambiguous || t->expanded))
			{
				args = bash_split(t->brick, t->bin);
				if (!args || (t->ambiguous && !**args))
				{
					t = t->next;
					continue ;
				}
			}
			cmd_setup(flow, t, &i[1], args);
		}
		t = t->next;
	}
	if (t)
		t->visited = 2;
}

void	make_flow(t_minishell *bash, t_token *t, t_re *redir, t_type type)
{
	int	index[2];

	t = bash->token;
	while (t)
	{
		index[0] = 0;
		index[1] = 0;
		redir = NULL;
		while (t && t->type != PIPE)
		{
			if (is_redir(t))
			{
				type = t->type;
				t = t->next;
				if (t)
					re_add_new(&redir, create_re(type, t));
			}
			t = t->next;
		}
		flow_add_new(&bash->flow, create_flow(redir));
		check_cmd(ft_last(bash->flow), bash->token, NULL, index);
		if (t && t->type == PIPE)
			t = t->next;
	}
}
