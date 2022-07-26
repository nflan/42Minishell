/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 20:07:44 by omoudni           #+#    #+#             */
/*   Updated: 2022/07/24 22:14:11 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_command(t_info *info, t_big_token *b_tokens)
{
	if (b_tokens->cmd_args[0][0] == '\0')
		return (127);
	if (!b_tokens->cmd_args || !ft_strlen(b_tokens->cmd_args[0]))
		return (1);
	else if (ft_is_cmd(b_tokens, info) == 2)
		return (-4);
	else if (!ft_is_cmd(b_tokens, info))
	{
		if (!access(b_tokens->cmd_args[0], F_OK))
		{
			if (ft_cmd_nopath(b_tokens)
				|| access(b_tokens->cmd_args[0], X_OK | R_OK))
				return (126);
		}
		return (0);
	}
	else if (ft_get_env_value(info, "PATH"))
		return (ft_path(info, b_tokens, 127));
	else
		return (127);
	return (0);
}

char	*strjoin_4(char *str1, char *str2)
{
	char	*ret;

	ret = ft_strjoin(str1, str2);
	free(str1);
	return (ret);
}
