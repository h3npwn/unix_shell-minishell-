/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 22:12:10 by abahja            #+#    #+#             */
/*   Updated: 2025/06/06 15:03:14 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	shift(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ((str[i] - 97) % 26) + 97;
		if (str[i] < 97)
			str[i] = (str[i] % 26) + 97;
		i++;
	}
}

char	*file_name(void)
{
	char			path[10];
	char			*file;
	int				fd;
	int				j;

	fd = open("/dev/random", O_RDWR);
	j = read(fd, path, 9);
	path[j] = 0;
	shift(path);
	file = ft_strjoin("/tmp/", path);
	while (!access(file, F_OK))
	{
		j = read(fd, path, 9);
		path[j] = 0;
		shift(path);
		file = ft_strjoin("/tmp/", path);
	}
	close(fd);
	return (file);
}

static void	create_and_fill(t_re *re, int fd, t_minishell *bash, char *content)
{
	char	*tmp;

	while (content && ft_strcmp(content, re->del))
	{
		tmp = readline("> ");
		content = ft_strdup(tmp);
		free(tmp);
		if (!content)
		{
			content = ft_strjoin("warning: here_doc delimited by EOF (wanted `",
					ft_strjoin(re->del, "')\n"));
			ft_putstr_fd(content, 2);
			close(fd);
			final_label(exit_status(0));
		}
		if (ft_strcmp(content, re->del))
		{
			if (!re->q_del && ft_strchr(content, '$'))
				content = get_value(bash->env, content, -1, 0);
			content = ft_strjoin(content, "\n");
			ft_putstr_fd(content, fd);
		}
	}
	close(fd);
	final_label(exit_status(0));
}

static char	*doc_content(t_re *re, t_minishell *bash, int status)
{
	int					id;
	int					fd;
	char				*file;

	file = file_name();
	fd = open(file, O_CREAT | O_WRONLY, 0644);
	re->hd_fd = open(file, O_RDONLY);
	unlink(file);
	if (fd == -1 || re->hd_fd == -1)
		return (open_handler(fd), NULL);
	last_open_fd(fd);
	id = forking();
	if (ft_errno (-1) != 0)
		return (close(fd), NULL);
	if (id == 0)
	{
		clear_cache(bash->flow);
		signal(SIGINT, handle_doc);
		create_and_fill(re, fd, bash, file);
	}
	(state())->is_heredoc = 1;
	waitpid(id, &status, 2);
	exit_status(WEXITSTATUS(status));
	(state())->is_heredoc = 0;
	return (close(fd), file);
}

int	here_doc(t_minishell *bash)
{
	t_eflow	*flow;
	t_re	*re;
	t_re	*tmp;

	flow = bash->flow;
	while (flow)
	{
		tmp = NULL;
		re = flow->redirs;
		while (re)
		{
			if (re->type == HEREDOC)
			{
				if (tmp)
					ft_skip2(&flow->redirs, tmp);
				re->filename = doc_content(re, bash, 0);
				if (exit_status(-1) != 0 || ft_errno(-1) != 0)
					return (1);
				tmp = re;
			}
			re = re->next;
		}
		flow = flow->next;
	}
	return (0);
}
