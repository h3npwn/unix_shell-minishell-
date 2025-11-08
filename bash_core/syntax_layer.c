/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_layer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:36:17 by abahja            #+#    #+#             */
/*   Updated: 2025/05/24 11:32:50 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_eflow	*ft_last(t_eflow *last)
{
	while (last->next)
		last = last->next;
	return (last);
}

int	is_in_token_list(t_type type, t_type *list)
{
	int	i;

	i = 0;
	while (list[i] != NILL)
	{
		if (list[i] == type)
			return (1);
		i++;
	}
	return (0);
}

int	pipe_syntax(t_token *cur)
{
	char	*err;

	err = ft_strdup("\033[1;31mSyntax error near unexpected token `|'\033[0m\n");
	if (!cur->prev || !cur->next || cur->next->type == PIPE)
	{
		ft_putstr_fd(err, 2);
		return (0);
	}
	return (1);
}

int	redirs_syntax(t_token *token, t_minishell bash)
{
	char	*err;

	err = ft_strdup("\033[1;31mSyntax error near ");
	err = ft_strjoin(err, "unexpected redirection \033[0m\n");
	if (!token->next || (!is_in_token_list(token->next->type, bash.t_del)
			&& !is_in_token_list(token->next->type, bash.t_word)))
	{
		ft_putstr_fd(err, 2);
		return (0);
	}
	return (1);
}

int	syntax(t_minishell *bash)
{
	t_token	*token;
	t_type	type;

	type = NILL;
	token = bash->token;
	while (token)
	{
		type = token->type;
		if (type == PIPE)
			if (!pipe_syntax(token))
				return (0);
		if (is_in_token_list(token->type, bash->t_redirs))
			if (!redirs_syntax(token, *bash))
				return (0);
		if (is_in_token_list(token->type, bash->t_nohandle))
			return (ft_putstr_fd("invalid symbol entred\n", 2), 0);
		token = token->next;
	}
	return (1);
}
