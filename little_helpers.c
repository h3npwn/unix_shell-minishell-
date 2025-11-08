/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:32:18 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/06 16:11:08 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

int	exit_status(int status)
{
	static unsigned char	exit_status;

	if (status >= 0)
		exit_status = status;
	return (exit_status);
}

void	clear_quotes(t_token *token)
{
	char	*tmp;

	tmp = token->brick;
	token->brick = ft_substr(tmp, 1, strlen(tmp) - 2);
	heap_manager(0, 'r', tmp);
}

static int	count_word(char const *s)
{
	static char	sp[3] = "\t\n ";
	int			count;
	int			flag;

	count = 0;
	flag = 0;
	while (*s)
	{
		if (ft_strchr(sp, *s))
			flag = 0;
		else if (!ft_strchr(sp, *s) && flag == 0)
		{
			count++;
			flag = 1;
		}
		s++;
	}
	return (count);
}

void	check_ambiguous(t_token *t, char *s, int expanded)
{
	size_t	i;

	i = 0;
	if (s && count_word(s) > 1 && t->type == WORD && expanded)
		t->ambiguous = 6;
	else if (s && !*s && t->type == WORD && expanded)
		t->ambiguous = 1;
	else if (s && *s && t->type == WORD && expanded)
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (!s[i])
			t->ambiguous = 2;
		else if (ft_isspace(*s) && ft_isspace(s[ft_strlen(s) - 1]))
			t->ambiguous = 5;
		else if (ft_isspace(s[ft_strlen(s) - 1]))
			t->ambiguous = 3;
		else if (ft_isspace(*s))
			t->ambiguous = 4;
	}
	if (expanded)
		t->expanded = 1;
}

void	name_file(t_type type, t_token *token, t_re *re)
{
	char	**splited;

	splited = bash_split(token->brick, token->bin);
	if (type == RED_IN || type == RED_OUT || type == APPEND)
	{
		if (token->ambiguous)
		{
			if (splited && *splited)
				re->filename = ft_strdup(*splited);
			else
				re->filename = ft_strdup("");
		}
		else
			re->filename = ft_strdup(token->brick);
	}
	else if (type == HEREDOC)
	{
		re->del = ft_strdup(token->brick);
		if (token->type == Q_DEL)
			re->q_del = 1;
	}
}
