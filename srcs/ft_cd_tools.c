/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 20:06:31 by nflan             #+#    #+#             */
/*   Updated: 2022/07/24 22:01:31 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd_tool(t_info *info, char **home, char **new_dir)
{
	*home = ft_get_env_value(info, "HOME");
	*new_dir = NULL;
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
	while (tmp && ft_strncmp(tmp->name, "PWD", 4))
		tmp = tmp->next;
	if (!tmp)
		return (free(pwd), 0);
	ft_export_replace(tmp, pwd, -1);
	free(pwd);
	return (0);
}

int	ft_oldpwd(t_info *info)
{
	t_env	*tmp;

	tmp = info->env;
	if (!tmp)
		return (1);
	while (tmp && ft_strncmp(tmp->name, "OLDPWD", 7))
		tmp = tmp->next;
	if (tmp)
		return (ft_export_replace(tmp, ft_get_env_value(info, "PWD"), -1));
	else
		return (0);
}
