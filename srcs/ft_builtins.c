/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/05/31 12:25:55 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit(t_info *info, char *value, char **tofree)
{
	int	ret;

	if (value)
		ret = ft_atoi(value);
	else
		ret = 0;
	if (tofree)
		ft_free_split(tofree);
	printf("exit\n");
	if (info)
		ft_free_all(info, info->env);
	exit(ret);
}

void	ft_env(t_env *env)
{
	t_env	*print;

	print = env;
	if (print)
	{
		while (print)
		{
			printf("%s=%s\n", print->name, print->value);
			print = print->next;
		}
	}
}

void	ft_echo(char *line, int fd, int ret)
{
	char	*tmp;
	int		i;

	i = 2;
	tmp = NULL;
	if (!line)
		return ;
	if (!ft_strncmp(line, "$?", 3))
		tmp = ft_itoa(ret);
	if ((line[0] == '-' || line[1] == 'n'))
	{
		while (line[i] && line[i] == 'n')
			i++;
		if (line[i] == ' ')
		{
			tmp = ft_substr(line, i + 1, ft_strlen(line) - i - 1);
			if (!tmp)
				return ;
		}
	}
	if (!tmp)
		tmp = ft_strjoin(line, "\n");
	if (!tmp)
		return ;
	ft_putstr_fd(tmp, fd);
	free(tmp);
}

int	ft_pwd(void)
{
	char	*buf;
	char	*tofree;

	buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		return (1);
	tofree = buf;
	buf = ft_strjoin(buf, "\n");
	ft_putstr_fd(buf, 1);
	free(buf);
	free(tofree);
	return (0);
}

t_env	*ft_unset(t_env *env, char *line)
{
	t_env	*tmp;
	t_env	*ptr;

	tmp = env;
	ptr = NULL;
	if (!env || !line)
		return (NULL);
	while (tmp->next && ft_strncmp(tmp->next->name, line, ft_strlen(line + 1)))
		tmp = tmp->next;
	if (!tmp->next && ft_strncmp(tmp->name, line, ft_strlen(line + 1)))
		return (NULL);
	ptr = tmp->next;
	tmp->next = ptr->next;
	ptr->next = NULL;
	ft_free_env(ptr);
	return (env);
}
