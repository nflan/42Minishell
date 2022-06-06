/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_launch_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 10:29:00 by nflan             #+#    #+#             */
/*   Updated: 2022/06/06 19:25:38 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_deeper_bt(t_big_token *b_tokens, t_big_token **tmp_b)
{
	*tmp_b = b_tokens;
	if (!tmp_b)
		return (1);
	while ((*tmp_b)->child)
		*tmp_b = (*tmp_b)->child;
	return (0);
}

int	ft_wash_btoken(t_info *info, t_big_token *b_tokens)
{
	t_token	*tokens;

	tokens = info->tokens;
	while (tokens && tokens->start != b_tokens->ind_tok_start)
		tokens = tokens->next;
	if (!tokens)
		return (1);
	if (tokens->token == TOK_SEP)
	{
		tokens = tokens->next;
		b_tokens->ind_tok_start++;
		if (b_tokens->ind_tok_start == b_tokens->length)
		{
			printf("Que des separateurs dans le bat7 (' ')\n");
			return (1);
		}
	}
	return (0);
}

t_cmd	*ft_convert_bt_cmd(t_info *info, t_big_token *b_tokens)
{
	t_cmd	*tmp;
	t_token	*tokens;
	int	i;

	i = b_tokens->ind_tok_start;
	tokens = info->tokens;
	while (tokens && i--)
		tokens = tokens->next;
	i = b_tokens->length;
	if (!tokens)
		return (NULL);
	tmp = ft_calloc(sizeof(t_cmd), 1);
	if (!tmp)
		return (NULL);
	while (tokens && i--)
	{
		tmp->cmd = ft_strjoin_free(tmp->cmd, tokens->value, 1);
		tokens = tokens->next;
	}
	if (!tmp->cmd)
		return (free(tmp), NULL);
	tmp->cmd_p = ft_split(tmp->cmd, ' ');
	if (!tmp->cmd_p)
		return (free(tmp->cmd), free(tmp), NULL);
	tmp->envp = ft_env_to_tab(info->env);
	tmp->fdin = 0;
	tmp->fdout = 1;
	return (tmp);
}

int	ft_builtins(t_info *info, t_cmd *cmd)
{
	int	len;

	len = ft_strlen(cmd->cmd_p[0]) + 1;
	if (!ft_strncmp(cmd->cmd_p[0], "pwd", len))
		return (ft_pwd());
	else if (!ft_strncmp(cmd->cmd_p[0], "env", len))
		return (ft_env(info->env));
	else if (!ft_strncmp(cmd->cmd_p[0], "echo", len))
		return (ft_echo(info, cmd));
	else if (!ft_strncmp(cmd->cmd_p[0], "unset", len))
	{
		if (!ft_unset(info->env, cmd->cmd + 6))
			return (1);
		else
			return (0);
	}
	else if (!ft_strncmp(cmd->cmd_p[0], "export", len))
		return (ft_export(info->env, cmd->cmd + 7));
	else if (!ft_strncmp(cmd->cmd_p[0], "cd", len))
		return (ft_cd(info, cmd));
	else if (!ft_strncmp(cmd->cmd_p[0], "exit", len) || !ft_strncmp(cmd->cmd, "exit ", 5))
		ft_exit(info, cmd->cmd_p[1], cmd->cmd_p);
	return (2);
}

int	ft_launch_cmd(t_info *info, t_big_token *b_tokens)
{
	t_cmd	*cmd;

	cmd = ft_convert_bt_cmd(info, b_tokens);
	if (!cmd)
		return (1);
//	printf("cmd->cmd = %s\n", cmd->cmd);
	info->status = ft_builtins(info, cmd);
	if (info->status == 2)
	{
		if (ft_pipex(info, cmd))
			return (ft_free_cmd(cmd), 1);
	}
	else if (info->status == 1)
		return (ft_free_cmd(cmd), 1);
	return (ft_free_cmd(cmd), 0);
}

int	ft_launch_sibling(t_info *info, t_big_token *b_tokens)
{
	t_big_token	*tmp_b;

	tmp_b = b_tokens;
	while (tmp_b)
	{
		if (ft_wash_btoken(info, tmp_b))
			return (1);
		if (ft_launch_cmd(info, tmp_b))
			return (1);
		if (tmp_b->sibling)
			tmp_b = tmp_b->sibling;
		else
			break;
	}
	return (0);
}

int	ft_find_cmd(t_info *info)
{
	t_big_token	*b_tokens;
	t_big_token	*tmp_b;

	b_tokens = info->parse;
	tmp_b = NULL;
	if (!b_tokens)
	{
		printf("pas de b_tokens dans ft_launch_cmd\n");
		return (1);
	}
//	while (b_tokens)
//	{
		if (ft_deeper_bt(b_tokens, &tmp_b))
		{
			printf("pb de deeper_bt\n");
			return (1);
		}
	//	while (tmp_b->parent)
	//	{
	//	if (tmp_b->sibling)
			if (ft_launch_sibling(info, tmp_b))
				return (1);
	//		tmp_b = tmp_b->parent;
	//	}
//		b_tokens = b_tokens->sibling;
//	}
	return (0);
}
