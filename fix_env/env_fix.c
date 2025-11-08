/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_fix.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochajou <mochajou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:53:35 by abahja            #+#    #+#             */
/*   Updated: 2025/06/04 20:16:01 by mochajou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_envstrdup(const char *s)
{
	size_t	s_os;
	char	*dup;

	if (!s)
		return (NULL);
	s_os = ft_strlen(s) + 1;
	dup = (char *)heap_manager(s_os * sizeof(char), 'e', 0);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s, s_os);
	return (dup);
}

char	*ft_envsubstr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	s_len;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (len == 0)
		return (ft_envstrdup(""));
	if (start >= s_len)
		return (ft_envstrdup(""));
	if (s_len <= start + len)
		substring = (char *)heap_manager((s_len - start + 1)
				* sizeof(char), 'e', 0);
	else
		substring = (char *)heap_manager((len + 1) * sizeof(char), 'e', 0);
	if (!substring)
		return (NULL);
	while (s[start] && i < len)
		substring[i++] = s[start++];
	substring[i] = '\0';
	return (substring);
}

static char	*ft_helper(char const *s1, char const *s2, char *dest)
{
	size_t	index;

	index = 0;
	while (*s1)
		dest[index++] = *s1++;
	while (*s2)
		dest[index++] = *s2++;
	dest[index] = '\0';
	return (dest);
}

char	*ft_envstrjoin(char const *s1, char const *s2)
{
	size_t	sizen;
	char	*res;

	if (!s1 || !s2)
	{
		if (s1)
			return (ft_strdup(s1));
		if (s2)
			return (ft_strdup(s2));
		else
			return (ft_strdup(""));
	}
	sizen = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = (char *)heap_manager(sizeof(char) * (sizen + 1), 'e', 0);
	res = ft_helper(s1, s2, res);
	return (res);
}
