/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:50:05 by mochajou          #+#    #+#             */
/*   Updated: 2025/06/05 21:50:42 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	ft_skip2(t_re **head, t_re *tmp)
{
	t_re	*move;
	t_re	*prev;

	move = (*head)->next;
	prev = *head;
	close(tmp->hd_fd);
	if (*head == tmp)
	{
		*head = (*head)->next;
		tmp->next = NULL;
		return ;
	}
	while (move)
	{
		if (move == tmp)
		{
			prev->next = move->next;
			tmp->next = NULL;
			return ;
		}
		prev = move;
		move = move->next;
	}
}

void	open_handler(int fd)
{
	ft_putstr_fd("cannot create temp file for here-document: \n", 2);
	isatty(fd);
	if (errno != EBADF)
		close(fd);
	ft_errno(1);
}

int	last_open_fd(int fd)
{
	static int	lastest;

	if (fd > 0)
		lastest = fd;
	return (lastest);
}
