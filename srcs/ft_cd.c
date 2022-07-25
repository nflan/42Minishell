/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/07/25 18:43:06 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_do_tilde(t_info *info, char *arg, char *home, char *new_dir)
{
	char	*op;

	op = ft_strdup(ft_get_env_value(info, "OLDPWD"));
	if (!strncmp(arg, "~", 2) && !home)
		new_dir = ft_strdup(info->home);
	else if (!strncmp(arg, "~", 2) && home)
		new_dir = ft_strdup(home);
	else
		new_dir = ft_cd_tilde(info, info->home, arg);
	if (!new_dir)
		return (free(op), 1);
	ft_oldpwd(info);
	if (chdir(new_dir))
	{
		ft_back(info->env, op, new_dir);
		free(new_dir);
		return (free(op), 1);
	}
	if (ft_newpwd(info))
		return (free(new_dir), free(op), 1);
	return (free(new_dir), free(op), 0);
}

int	ft_unset_solo(t_info *info, char *name)
{
	int		check;
	int		err;
	t_env	*tmp;
	t_env	*ptr;

	tmp = info->env;
	err = 0;
	ptr = NULL;
	if (!name || !info || !tmp)
		return (1);
	check = ft_unset_name(&tmp, name);
	if (!ft_strncmp(tmp->name, name, ft_strlen(tmp->name) + 1))
		unset_tool(&ptr, &tmp, &info, 1);
	else if (!check)
		unset_tool(&ptr, &tmp, &info, 2);
	else if (check == 2)
		err = 1;
	if (ptr)
		unset_tool(&ptr, &tmp, &info, 3);
	return (err);
}

int	ft_do_cd(t_info *info, t_big_token *b, char *op)
{
	ft_oldpwd(info);
	if (!ft_strncmp(b->cmd_args[1], "-", 2))
	{
		if (chdir(op))
		{
			ft_unset_solo(info, "OLDPWD");
			return (ft_putstr_error("minishell: cd: OLDPWD not set\n"));
		}
		ft_putstr_fd(ft_get_env_value(info, "OLDPWD"), b->fdout);
		ft_putstr_fd("\n", b->fdout);
	}
	else
	{
		ft_oldpwd(info);
		if (b->cmd_args[1][0] != '\0')
		{
			if (access(b->cmd_args[1], F_OK))
				return (ft_putstr_fd_3("minishell: cd: ",
						b->cmd_args[1], ": No such file or directory\n", 2), 1);
			if (chdir(b->cmd_args[1]))
				return (ft_back(info->env, op, b->cmd_args[1]));
		}
	}
	return (0);
}

int	ft_cd_home(t_info *info, char *home, char *op)
{
	if (!home)
		return (free(op), ft_putstr_error("minishell: cd: HOME not set\n"));
	else
	{	
		ft_oldpwd(info);
		if (chdir(home))
			return (ft_back(info->env, op, home), free(op), 1);
	}
	return (0);
}

int	ft_cd(t_info *info, t_big_token *b_tokens)
{
	char	*home;
	char	*new_dir;
	char	*op;

	ft_cd_tool(info, &home, &new_dir);
	if (b_tokens->cmd_args_num > 2)
		return (ft_putstr_error("minishell: cd: too many arguments\n"));
	op = ft_strdup(ft_get_env_value(info, "OLDPWD"));
	if (!op)
		return (1);
	if (ft_is_tilde_or_home(home, b_tokens->cmd_args[1]) == 1)
	{
		if (ft_cd_home(info, home, op))
			return (1);
	}
	else if (!ft_is_tilde_or_home(home, b_tokens->cmd_args[1]))
		return (free(op), ft_do_tilde(info, b_tokens->cmd_args[1], home,
				new_dir));
	else
		if (ft_do_cd(info, b_tokens, op))
			return (free(op), 1);
	if (ft_newpwd(info))
		return (free(op), 1);
	return (free(op), 0);
}
