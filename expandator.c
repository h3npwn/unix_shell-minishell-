/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandator.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:07:11 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/05 16:44:49 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

static char	*get_value2(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (ft_strdup(env->value + 1));
		env = env->next;
	}
	return (ft_strdup(""));
}

static void	expande(char **s, int *i, int j, t_env *env)
{
	char	*value;
	char	*tmp;
	char	*slice;
	char	*old;

	old = *s;
	slice = ft_substr(old + 1 + *i, 0, j - *i - 1);
	value = get_value2(env, slice);
	heap_manager(0, 'r', slice);
	slice = ft_substr(old, 0, *i);
	tmp = slice;
	slice = ft_strjoin(slice, value);
	heap_manager(0, 'r', tmp);
	*s = ft_strjoin(slice, &old[j]);
	*i += ft_strlen(value) - 1;
	heap_manager(0, 'r', value);
	heap_manager(0, 'r', slice);
}

static void	expande2(int *i, char **s)
{
	char	*value;
	char	*tmp;
	char	*slice;
	char	*old;

	old = *s;
	value = ft_itoa(exit_status(-1));
	slice = ft_substr(old, 0, *i);
	tmp = ft_strjoin(slice, value);
	*s = ft_strjoin(tmp, &old[*i + 2]);
	*i += ft_strlen(value) - 1;
	heap_manager(0, 'r', value);
	heap_manager(0, 'r', tmp);
	heap_manager(0, 'r', slice);
}

char	*get_value(t_env *env, char *s, char check, t_token *t)
{
	char		*tmp;
	int			in[2];

	tmp = s;
	in[0] = -1;
	while (s[++in[0]])
	{
		if (s[in[0]] == '$' && (ft_isalpha(s[in[0] + 1])
				|| s[in[0] + 1] == '_' || !s[in[0] + 1]))
		{
			if (!s[in[0] + 1] && (!check || check == -1))
				continue ;
			in[1] = in[0] + 1;
			while (s[in[1]] && (ft_isalnum(s[in[1]]) || s[in[1]] == '_'))
				in[1]++;
			expande(&s, &in[0], in[1], env);
		}
		else if (!ft_strncmp(&s[in[0]], "$?", 2))
			expande2(&in[0], &s);
	}
	if (check != -1)
		check_ambiguous(t, s, (tmp != s));
	if (tmp != s)
		heap_manager(0, 'r', tmp);
	return (s);
}

void	expandator(t_minishell *bash, int f)
{
	t_token	*t;
	char	check;

	t = bash->token;
	while (t)
	{
		if (t->type == D_QWORD || t->type == S_QWORD)
			clear_quotes(t);
		if (t->type != W_SPACE && t->brick && ft_strchr(t->brick, '=')
			&& ft_strchr(t->brick, '=') != t->brick)
			t->assign = 1;
		if (f && t->type != W_SPACE)
			f = 0;
		else if (t->type == HEREDOC)
			f = 1;
		else if (t->type != S_QWORD && ft_strchr(t->brick, '$'))
		{
			check = (t->next && t->next->type != W_SPACE && t->type != D_QWORD);
			t->brick = get_value(bash->env, t->brick, check, t);
		}
		t = t->next;
	}
}
