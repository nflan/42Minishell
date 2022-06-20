/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:37:41 by nflan             #+#    #+#             */
/*   Updated: 2022/06/20 11:30:47 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_is_tilde_or_home(char *home, char *dir)
{
	if (!dir || !ft_strncmp(dir, "", 1))
		return (1);
	if (!home && dir[1] == '/')
		return (1);
	if (dir[1] == '/' || dir[1] == '+' || dir[1] == '-')
		return (0);
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
	if (dir[1] == '/')
		new_dir = ft_strjoin(home, dir + 1);
	else if (dir[1] == '+' || dir[1] == '-')
	{
		new_dir = getcwd(new_dir, 0);
		if (dir[1] == '+')
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		else
		{
			tmp = ft_strrchr(new_dir, '/');
			new_dir = ft_substr_free(new_dir, 0, ft_strlen(new_dir) - ft_strlen(tmp));
			new_dir = ft_strjoin_free(new_dir, dir + 2, 1);
		}
	}
	return (new_dir);
}

int	ft_cd(t_info *info, t_big_token *b_tokens)
{
	char	*home;
	char	*new_dir;

	home = ft_get_env_value(info, "HOME");
	new_dir = NULL;
	if (b_tokens->cmd_args[2])
		return (ft_putstr_error("minishell: cd: too many arguments\n"));
	if (ft_is_tilde_or_home(home, b_tokens->cmd_args[1]) == 1)
	{
		if (!home)
			return (ft_putstr_error("minishell: cd: HOME not set\n"));
		else if (chdir(home))
			return (ft_perror("minishell: cd: ", b_tokens->cmd_args[1]));
	}
	else if (!ft_is_tilde_or_home(home, b_tokens->cmd_args[1]))
	{
		new_dir = ft_cd_tilde(home, b_tokens->cmd_args[1]);
		if (!new_dir)
			return (1);
		if (chdir(new_dir))
			return (ft_perror_free("minishell: cd: ", new_dir, 2));
		free(new_dir);
	}
	else
		if (chdir(b_tokens->cmd_args[1]))
			return (ft_perror("minishell: cd: ", b_tokens->cmd_args[1]));
	return (0);
}
