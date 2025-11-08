/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flow_bash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:51:05 by abahja            #+#    #+#             */
/*   Updated: 2025/06/06 13:19:25 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	is_redir(t_token *token)
{
	return (token->type == RED_IN || token->type == RED_OUT
		|| token->type == HEREDOC || token->type == APPEND);
}

int	quotes(char *s)
{
	if (!s || !*s)
		return (1);
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (!ft_strchr(s + 1, *s))
			{
				ft_putstr_fd("syntax error: unclosed quotes\n", 2);
				return (0);
			}
			else
				s = ft_strchr(s + 1, *s);
		}
		s++;
	}
	return (1);
}

void	clear_cache(t_eflow *flows)
{
	t_re	*re;
	t_eflow	*flow;

	flow = flows;
	while (flow)
	{
		re = flow->redirs;
		while (re)
		{
			if (re->type == HEREDOC && re->hd_fd != -1)
			{
				isatty(re->hd_fd);
				if (errno != EBADF)
					close(re->hd_fd);
			}
			re = re->next;
		}
		flow = flow->next;
	}
}

void	set_bin(t_minishell *bash)
{
	t_token		*t;
	int			i;
	static char	*ifs = " \t\n";

	t = bash->token;
	while (t)
	{
		i = 0;
		if (is_in_token_list(t->type, bash->t_word))
		{
			t->bin = ft_strdup(t->brick);
			while (t->brick[i] && t->bin[i])
			{
				if (t->type == WORD && ft_strchr(ifs, t->bin[i]))
					t->bin[i] = '0';
				else
					t->bin[i] = '1';
				i++;
			}
		}
		t = t->next;
	}
}

void	the_core(t_minishell *bash)
{
	core_tokinizer(bash);
	expandator(bash, 0);
	set_bin(bash);
	ft_spacer(bash);
	if (!bash->token)
		return ;
	if (!syntax(bash))
	{
		exit_status(2);
		return ;
	}
	make_flow(bash, 0, 0, 0);
	if (!here_doc(bash))
		minishell(bash);
	clear_cache(bash->flow);
	heap_manager(0, 'f', 0);
	state()->is_excution = 0;
	return ;
}
