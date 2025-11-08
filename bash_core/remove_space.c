/* ************************************************************************** */
/*	                                                                    */
/*                                                        :::      ::::::::   */
/*   remove_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:49:58 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/06 16:37:31 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static char	remove_space(t_token **token, t_token *t)
{
	t_token	*tmp;

	tmp = t;
	if (tmp == *token)
		*token = tmp->next;
	if (tmp->prev)
		tmp->prev->next = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;
	heap_manager(0, 'r', tmp->brick);
	heap_manager(0, 'r', tmp);
	return (0);
}

static void	check_one(t_token *t, t_type type, char *s)
{
	char	*last_d;
	int		i;

	last_d = ft_strrchr(s, '$');
	i = last_d - s;
	if (last_d && !expandable(s, i, last_d) && type == WORD)
	{
		t->ambiguous = 0;
		t->expanded = 0;
	}
}

static void	ambiguous_scaner(t_token *t, t_token *n_t)
{
	if (t->ambiguous != 6)
	{
		if (t->ambiguous == 1 || t->ambiguous == 2)
			t->ambiguous = n_t->ambiguous;
		else if (n_t->ambiguous == 2 && !t->ambiguous)
			t->ambiguous = 3;
		else if (t->ambiguous == 3 && (n_t->ambiguous >= 3 || !n_t->ambiguous))
			t->ambiguous = 6;
		else if ((t->ambiguous == 4 && n_t->ambiguous >= 3)
			|| (!t->ambiguous && n_t->ambiguous == 4))
			t->ambiguous = 6;
		else if ((t->ambiguous == 5 && (n_t->ambiguous >= 3 || !n_t->ambiguous))
			|| (!t->ambiguous && n_t->ambiguous == 5))
			t->ambiguous = 6;
		else if (n_t->ambiguous == 6)
			t->ambiguous = 6;
		else if (t->ambiguous)
			t->ambiguous = n_t->ambiguous;
	}
}

static char	join_words(t_token *t, t_minishell *bash)
{
	char	*s;
	t_token	*next_one;

	next_one = t->next;
	s = t->brick;
	t->brick = ft_strjoin(t->brick, t->next->brick);
	t->bin = ft_strjoin(t->bin, t->next->bin);
	if (!t->expanded)
		t->expanded = next_one->expanded;
	if (!t->assign)
		t->assign = next_one->assign;
	ambiguous_scaner(t, next_one);
	if (!next_one->next
		|| !is_in_token_list(next_one->next->type, bash->t_word))
		check_one(t, next_one->type, next_one->old);
	t->next = t->next->next;
	if (t->next)
		t->next->prev = t;
	heap_manager(0, 'r', s);
	heap_manager(0, 'r', next_one->brick);
	heap_manager(0, 'r', next_one);
	return (1);
}

void	ft_spacer(t_minishell *bash)
{
	t_token	*t;
	t_token	*next;
	char	f;

	t = bash->token;
	f = 0;
	while (t)
	{
		if (t->type == W_SPACE)
		{
			next = t->next;
			f = remove_space(&bash->token, t);
			t = next;
			continue ;
		}
		if (t->next && (is_in_token_list(t->type, bash->t_word)
				&& is_in_token_list(t->next->type, bash->t_word)))
		{
			f = join_words(t, bash);
			continue ;
		}
		if (!f && is_in_token_list(t->type, bash->t_word))
			check_one(t, t->type, t->old);
		t = t->next;
	}
}
