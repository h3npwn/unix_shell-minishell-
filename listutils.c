/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listutils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:59:17 by abahja            #+#    #+#             */
/*   Updated: 2025/06/05 21:52:20 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"

t_token	*create_new(int size, t_type type)
{
	t_token	*new;

	new = (t_token *)heap_manager(sizeof(t_token), 'x', NULL);
	new->brick = (char *)heap_manager(sizeof(char) * (size + 1), 'x', NULL);
	new->bin = NULL;
	new->old = NULL;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	new->visited = 0;
	new->expanded = 0;
	new->ambiguous = 0;
	new->assign = 0;
	return (new);
}

void	flow_add_new(t_eflow **flow, t_eflow *nflow)
{
	t_eflow	*tmp;

	if (!flow || !nflow)
		return ;
	if (!*flow)
	{
		*flow = nflow;
		return ;
	}
	tmp = *flow;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = nflow;
	nflow->prev = tmp;
}

t_eflow	*create_flow(t_re *redirs)
{
	t_eflow	*flow;

	flow = heap_manager(sizeof(t_eflow), 'x', 0);
	flow->cmd = NULL;
	flow->arg = NULL;
	flow->is_built_in = 0;
	flow->redirs = redirs;
	flow->next = NULL;
	flow->prev = NULL;
	return (flow);
}

void	re_add_new(t_re **redirs, t_re *nredir)
{
	t_re	*tmp;

	if (!redirs || !nredir)
		return ;
	if (!*redirs)
	{
		*redirs = nredir;
		return ;
	}
	tmp = *redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = nredir;
}

t_re	*create_re(t_type type, t_token *token)
{
	t_re	*redir;

	redir = heap_manager(sizeof(t_re), 'x', 0);
	token->visited = 1;
	token->prev->visited = 1;
	redir->filename = NULL;
	redir->del = NULL;
	redir->f_ambiguous = token->ambiguous;
	if (token->ambiguous >= 3 && token->ambiguous < 6)
		redir->f_ambiguous = 3;
	redir->q_del = 0;
	redir->hd_fd = -1;
	name_file(type, token, redir);
	redir->type = type;
	redir->next = NULL;
	return (redir);
}
