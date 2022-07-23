/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/07/23 17:11:54 by nflan            ###   ########.fr       */
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

char	*ft_cd_tilde(char *home, char *dir)
{
	char	*new_dir;
	char	*tmp;

	new_dir = NULL;
	tmp = NULL;
	if (!dir)
		return (NULL);
	if (dir[1] == '/') //de meme -> oui parce que si tu fais ~/Documents, le bash va chercher au home alors que si t'as un + ou un -, il part du repertoire courant ou precedent. D'ou le fait que je fasse un join du home puis du path qu'on envoi +1, pour enlever le ~
		new_dir = ft_strjoin(home, dir + 1);
	else if (dir[1] == '+' || dir[1] == '-')
	{
		new_dir = getcwd(new_dir, 0);
		if (dir[1] == '+')
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		else
		{
			tmp = ft_strrchr(new_dir, '/');
			new_dir = ft_substr_free(new_dir, 0,
					ft_strlen(new_dir) - ft_strlen(tmp));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
	}
	return (new_dir);
}

int	ft_newpwd(t_info *info)
{
	t_env	*tmp;
	char	*pwd;

	pwd = NULL;
	tmp = info->env;
	pwd = getcwd(pwd, 0);
	if (!pwd || !tmp)
		return (1);
	while (ft_strncmp(tmp->name, "PWD", 4))
		tmp = tmp->next;
	ft_export_replace(tmp, pwd, -1);
	free(pwd);
	if (!tmp)
		return (1);
	return (0);
}

int	ft_oldpwd(t_info *info)
{
	t_env	*tmp;

	tmp = info->env;
	if (!tmp)
		return (1);
	while (ft_strncmp(tmp->name, "OLDPWD", 7))
		tmp = tmp->next;
	return (ft_export_replace(tmp, ft_get_env_value(info, "PWD"), -1));
}

int	ft_do_tilde(t_info *info, char *arg, char *home, char *new_dir)
{
	if (!strncmp(arg, "~", 2) && !home) //pk tu compares tilde toute seule aved 2? -> pour eviter comparer "~\0" car potentiellement, je peux avoir "~+" ou "~/" etc
		new_dir = ft_strdup(info->home);
	else if (!strncmp(arg, "~", 2) && home)
		new_dir = ft_strdup(home);
	else
		new_dir = ft_cd_tilde(info->home, arg);
	if (!new_dir)
		return (1);
	if (chdir(new_dir))
		return (ft_perror_free("minishell: cd: ", new_dir, 2));
	ft_oldpwd(info);
	if (ft_newpwd(info))
		return (1);
	return (free(new_dir), 0);
}

int	ft_do_cd(t_info *info, t_big_token *b_tokens)
{
	char	*err;

	err = NULL;
	if (!ft_strncmp(b_tokens->cmd_args[1], "-", 2))
	{
		if (chdir(ft_get_env_value(info, "OLDPWD")))
			return (ft_perror("minishell: cd: ", ft_get_env_value(info, "OLDPWD")));
		ft_putstr_fd(ft_get_env_value(info, "OLDPWD"), b_tokens->fdout);
		ft_putstr_fd("\n", b_tokens->fdout);
	}
	else
	{
		if (b_tokens->cmd_args[1][0] != '\0')
		{
			if (access(b_tokens->cmd_args[1], F_OK))
			{
				err = ft_strjoiiin("minishell: cd: ", b_tokens->cmd_args[1],": No such file or directory\n");
				if (!err)
					return (ft_putstr_error("Malloc error\n"));
				return (ft_putstr_error(err), free(err), 1);
			}
			if (chdir(b_tokens->cmd_args[1]))
				return (ft_perror("minishell: cd: ", b_tokens->cmd_args[1]));
		}
	}
	ft_oldpwd(info);
	return (0);
}

int	ft_cd(t_info *info, t_big_token *b_tokens)
{
	char	*home;
	char	*new_dir;

	home = ft_get_env_value(info, "HOME");
	new_dir = NULL;
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
