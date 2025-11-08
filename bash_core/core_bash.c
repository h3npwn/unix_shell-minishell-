/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_bash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:36:23 by abahja            #+#    #+#             */
/*   Updated: 2025/06/04 10:19:27 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	deli_type(t_token *token, char *slice, int size, int *flag)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (slice[i] == '\'' || slice[i] == '\"')
			break ;
		i++;
	}
	if (i == size)
		token->type = DEL;
	else
		token->type = Q_DEL;
	*flag = 0;
}

int	keep(char *brick, char *slice, int *i, int size)
{
	int	x;
	int	j;

	x = 1;
	j = 0;
	while (*i < size && slice[*i] != '\'' && slice[*i] != '\"')
	{
		if ((slice[*i] == '$' && x % 2) && (slice[*i + 1] == '\''
				|| slice[*i + 1] == '\"'))
		{
			(*i)++ ;
			break ;
		}
		if (slice[*i] == '$')
			x++;
		brick[j++] = slice[(*i)++];
		if (slice[(*i)] != '$')
			x = 1;
	}
	return (j);
}

void	heredoc_mode(t_token *new, char *slice, int size, int *flag)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	if (!new || !slice)
		return ;
	deli_type(new, slice, size, flag);
	while (i < size)
	{
		if (slice[i] == '\'' || slice[i] == '\"')
		{
			quote = slice[i];
			i++;
			while (slice[i] && slice[i] != quote)
				new->brick[j++] = slice[i++];
			i++;
			continue ;
		}
		j += keep(&new->brick[j], slice, &i, size);
	}
	new->brick[j] = 0;
}

static void	ft_strnxcpy(t_token *new, char *slice, int size, int *flag)
{
	if (*flag == 1337)
		heredoc_mode(new, slice, size, flag);
	else
		simple_mode(new, slice, size);
}

void	core_tokinizer(t_minishell *bash)
{
	t_token	*t_new;
	t_type	type;
	int		flag;
	int		i;
	int		j;

	j = 0;
	i = 0;
	flag = 0;
	t_new = NULL;
	while (bash->prompt[i])
	{
		j = i;
		type = ft_ismeta(bash->prompt, &i, &flag);
		t_new = create_new(i - j, type);
		ft_strnxcpy(t_new, bash->prompt + j, i - j, &flag);
		t_new->old = ft_strdup(t_new->brick);
		lst_add_back(&bash->token, t_new);
	}
	return ;
}
