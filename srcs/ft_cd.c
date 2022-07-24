/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 20:07:24 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_is_tilde_or_home(char *home, char *dir)
{
	if (dir && !ft_strncmp(dir, "~", 2))
		return (0);
	if (!dir)
		return (1);
	if (dir[0] == '~' && (dir[1] == '/' || dir[1] == '+' || dir[1] == '-'))
		return (0);
	if (!home && !dir)
		return (1);
	return (2);
}

char	*ft_cd_tilde(t_info *info, char *home, char *dir)
{
	char	*new_dir;
	char	*tmp;

	new_dir = NULL;
	tmp = NULL;
	if (!dir)
		return (NULL);
	if (dir[1] == '/')
		new_dir = ft_strjoin(home, dir + 1);
	else if (dir[1] == '+' || dir[1] == '-')
	{
		if (dir[1] == '+')
		{
			new_dir = ft_strdup(ft_get_env_value(info, "PWD"));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
		else
		{
			new_dir = ft_strdup(ft_get_env_value(info, "OLDPWD"));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
	}
	return (new_dir);
}

int	ft_do_tilde(t_info *info, char *arg, char *home, char *new_dir)
{
	if (!strncmp(arg, "~", 2) && !home)
		new_dir = ft_strdup(info->home);
	else if (!strncmp(arg, "~", 2) && home)
		new_dir = ft_strdup(home);
	else
		new_dir = ft_cd_tilde(info, info->home, arg);
	if (!new_dir)
		return (1);
	if (chdir(new_dir))
		return (ft_perror_free("minishell: cd: ", new_dir, 2));
	ft_oldpwd(info);
	if (ft_newpwd(info))
		return (1);
	return (free(new_dir), 0);
}

int	ft_do_cd(t_info *info, t_big_token *b)
{
	if (!ft_strncmp(b->cmd_args[1], "-", 2))
	{
		if (chdir(ft_get_env_value(info, "OLDPWD")))
			return (ft_perror("minishell: cd: ",
					ft_get_env_value(info, "OLDPWD")));
		ft_putstr_fd(ft_get_env_value(info, "OLDPWD"), b->fdout);
		ft_putstr_fd("\n", b->fdout);
	}
	else
	{
		if (b->cmd_args[1][0] != '\0')
		{
			if (access(b->cmd_args[1], F_OK))
				return (ft_putstr_fd_3("minishell: cd: ",
						b->cmd_args[1], ": No such file or directory\n", 2), 1);
			if (chdir(b->cmd_args[1]))
				return (ft_perror("minishell: cd: ", b->cmd_args[1]));
		}
	}
	ft_oldpwd(info);
	return (0);
}

int	ft_cd(t_info *info, t_big_token *b_tokens)
{
	char	*home;
	char	*new_dir;

	ft_cd_tool(info, &home, &new_dir);
	if (b_tokens->cmd_args_num > 2)
		return (ft_putstr_error("minishell: cd: too many arguments\n"));
	if (ft_is_tilde_or_home(home, b_tokens->cmd_args[1]) == 1)
	{
		if (!home)
			return (ft_putstr_error("minishell: cd: HOME not set\n"));
		else
		{	
			if (chdir(home))
				return (ft_perror("minishell: cd: ", home));
			ft_oldpwd(info);
		}
	}
	else if (!ft_is_tilde_or_home(home, b_tokens->cmd_args[1]))
		return (ft_do_tilde(info, b_tokens->cmd_args[1], home, new_dir));
	else
		if (ft_do_cd(info, b_tokens))
			return (1);
	if (ft_newpwd(info))
		return (1);
	return (0);
}
