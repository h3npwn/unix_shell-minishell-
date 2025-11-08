/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:35:02 by abahja            #+#    #+#             */
/*   Updated: 2025/05/25 22:35:57 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	pwd(t_minishell *bash)
{
	char	*pwd;

	pwd = getcwd(0, 0);
	if ((!bash->oldpwd || !*bash->oldpwd) && !pwd)
	{
		perror("");
		exit_status(1);
	}
	else
	{
		if (!bash->oldpwd)
			printf("%s\n", pwd);
		else
			printf("%s\n", bash->oldpwd);
		exit_status(0);
	}
	free(pwd);
}
