/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/07/21 22:56:59 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_exit(t_info *info, t_big_token *b_tokens)
{
	int	ret;

	ret = info->status;
	if (b_tokens && b_tokens->cmd_args[1])
		ret = ft_atoi(b_tokens->cmd_args[1]);
	ft_putstr_fd("exit\n", 1);
	if (b_tokens)
		ft_free_all(info, info->env);
	rl_clear_history();
	exit(ret);
}

int	ft_env(t_info *info, t_big_token *b_tok)
{
	t_env	*print;
	char	*line;

	print = info->env;
	line = NULL;
	if (print)
	{
		while (print)
		{
			line = ft_strjoiiin(print->name, "=", print->value);
			line = ft_strjoin_free(line, "\n", 1);
			ft_putstr_fd(line, b_tok->fdout);
			free(line);
			print = print->next;
		}
	}
	return (0);
}

int	ft_pwd(t_big_token *b_tok)
{
	char	*buf;
	char	*tofree;

	buf = NULL;
	buf = getcwd(buf, 0);
	if (!buf)
		return (1);
	tofree = buf;
	buf = ft_strjoin(buf, "\n");
	ft_putstr_fd(buf, b_tok->fdout);
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
		while (name[i] && (*tmp)->next->name[i]
			&& name[i] == (*tmp)->next->name[i])
			i++;
		if ((size_t)i == ft_strlen(name)
			&& ft_strlen(name) == ft_strlen((*tmp)->next->name))
			return (0);
		else
			*tmp = (*tmp)->next;
	}
	return (1);
}

int	ft_unset(t_info *info, t_big_token *b_tokens)
{
	t_env	*tmp;
	t_env	*ptr;

	tmp = info->env;
	ptr = NULL;
	if (!tmp || !b_tokens->cmd_args[0])
		return (1);
	if (ft_unset_name(&tmp, b_tokens->cmd_args[1]))
		return (1);
	ptr = tmp->next;
	tmp->next = ptr->next;
	ptr->next = NULL;
	ft_free_env(ptr);
	return (0);
}

//j'ai l'impression qu'on fait les choses mal ici. On unset pas si jamais la variable est la premiere de l'env
