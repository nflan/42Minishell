/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/06/07 18:22:33 by nflan            ###   ########.fr       */
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

int	ft_env(t_env *env)
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
	return (0);
}

void	ft_print_tokens(t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	if (tmp)
	{
		while (tmp)
		{
			printf("index = %d\n", tmp->index);
			printf("value = %s\n", tmp->value);
			printf("quoted = %d\n", tmp->quoted);
			printf("lenght = %d\n", tmp->length);
			printf("type = %d\n", tmp->token);
			printf("start = %d\n\n", tmp->start);
			tmp = tmp->next;
		}
	}
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

int	ft_unset_name(t_env **tmp, char *name)
{
	int	i;

	i = 0;
	if (!tmp || !name)
		return (1);
	while ((*tmp)->next)
	{
		i = 0;
		while (name[i] && (*tmp)->next->name[i] && name[i] == (*tmp)->next->name[i])
			i++;
		if ((size_t)i == ft_strlen(name) && ft_strlen(name) == ft_strlen((*tmp)->next->name))
			return (0);
		else
			*tmp = (*tmp)->next;
	}
	return (1);
}

t_env	*ft_unset(t_env *env, t_cmd *cmd)
{
	t_env	*tmp;
	t_env	*ptr;

	tmp = env;
	ptr = NULL;
	if (!env || !cmd->cmd)
		return (NULL);
	if (ft_unset_name(&tmp, cmd->cmd_p[1]))
		return (NULL);
	ptr = tmp->next;
	tmp->next = ptr->next;
	ptr->next = NULL;
	ft_free_env(ptr);
	return (env);
}
