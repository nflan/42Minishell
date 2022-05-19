/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:39:37 by nflan             #+#    #+#             */
/*   Updated: 2022/05/19 16:23:17 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_keep_history(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (*str && *str == '\n')
			str++;
	if (str)
		while (*str++)
			i++;
	return (i);
}

void	ft_signal(int sig)
{
	signal(sig, SIG_IGN);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, &ft_signal);
}

int	ft_exit(char *str)
{
	if (str)
		while (*str && *str == ' ')
			str++;
	if (!ft_strncmp(str, "exit", 5) || !ft_strncmp(str, "exit ", 5))
		return (0);
	return (1);
}

char	*ft_cmd_line(char *str)
{
	char	*cmd;
	int		i;
	int		y;

	i = -1;
	cmd = NULL;
	if (str)
		while (*str && *str == ' ')
			str++;
	if (str)
		while (str[++i])
			if ((str[i] == '|' && str[i + 1] == '|')
				|| (str[i] == '&' && str[i + 1] == '&'))
				break ;
	cmd = ft_calloc(sizeof(char *), i);
	if (!cmd)
		return (NULL);
	y = -1;
	while (++y < i)
		cmd[y] = str[y];
	return (cmd);
}

/*void	ft_cmdadd_back(t_cmd **acmd, t_cmd *new)
{
	t_cmd	*tmp;

	tmp = NULL;
	tmp = *acmd;
	if (acmd && new)
	{
		if (*acmd == NULL)
			*acmd = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			new->prev = tmp;
			tmp->next = new;
		}
	}
}*/

char	*ft_onecmd(t_info *info, int i)
{
	char	*cmd_line;
	char	*cmd;
	int		y;

	cmd = NULL;
	y = 0;
	cmd_line = ft_cmd_line(info->rdline);
	if (!cmd_line)
		return (NULL);
	if (i)
	{
		y = -1;
		while (i && cmd_line[++y])
			if (cmd_line[y - 1] == '|')
				i--;
	}
	while (cmd_line[y + i] && cmd_line[y + i] != '|')
		i++;
	printf("i = %d\n", i);
	cmd = ft_calloc(sizeof(char), i);
	if (!cmd)
		return (NULL);
	i = -1;
	y -= 1;
	while (cmd_line[++y] && cmd_line[++i] != '|')
		cmd[i] = cmd_line[y];
	return (cmd);
}

/*int	ft_count_pipe(t_info *info)
{
	int	count;

	count = 0;

	return (count);
}*/

/*t_cmd	*ft_cmdnew(t_info *info, char *cmd)
{
	t_cmd	*new;

	new = ft_calloc(sizeof(t_cmd), 1);
	if (!new)
		return (NULL);
	new->cmd = cmd;
	new->fdin = 0;
	new->fdout = 1;
	return (new);
}*/

void	ft_print_cmd(t_cmd	*cmd)
{
	printf("cmd->line = %s\n", cmd->cmd);
	printf("cmd->fdin = %d\n", cmd->fdin);
	printf("cmd->fdout = %d\n", cmd->fdout);
}

t_cmd	*ft_cmdnew(char *cmd, t_tree *ptr)
{
	t_cmd	*tmp;

	if (!cmd)
		return (NULL);
	tmp = NULL;
	tmp = ft_calloc(sizeof(t_cmd), 1);
	if (!tmp)
		return (NULL);
	tmp->cmd = ft_strdup(cmd);
	tmp->fdin = 0;
	tmp->fdout = 1;
	tmp->tree = NULL;
	if (ptr)
		tmp->tree = ptr;
	return (tmp);
}

void	ft_lstadd_tree(t_tree **alst, t_tree *new, int i)
{
	t_tree	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else if (i == 2)
		{
			while (tmp->right)
				tmp = tmp->right;
			tmp->right = new;
		}
		else if (i == 1)
		{
			while (tmp->right)
				tmp = tmp->right;
			tmp->left = new;
		}
	}
}

t_tree	*ft_treenew(char *str, t_tree *ptr)
{
	t_tree	*new;

	new = NULL;
	new = ft_calloc(sizeof(t_tree), 1);
	if (!new)
		return (NULL);
	new->cmd = ft_cmdnew(str, ptr);
	new->right = NULL;
	new->left = NULL;
	return (new);
}

/*t_tree	*ft_fill_tree(t_info *info, t_token *token)
{
	t_tree	*ptr;
	t_tree	*new;
	t_token	*tok;
	int		len;

	ptr = NULL;
	new = NULL;
	tok = token;
	while (tok)
		tok = tok->next;
	while (tok)
	{
		ptr = ft_treenew(tok->cmd, ptr);
		if (tok->token == TOK_WORD)
			ft_lstadd_tree(&new, ptr, 1);
		else if (tok->token == TOK_OPERATOR)
			ft_lstadd_tree(&new, ptr, 2);
		tok = tok->prev;
	}
	return (new);
}*/

t_tree	*ft_fill_tree(t_info *info)
{
	t_tree	*ptr;
	t_tree	*new;
	t_tree	*tmpcmd;
	char	**tab;
	int		i;

	i = 0;
	tmpcmd = NULL;
	ptr = NULL;
	new = NULL;
	tab = ft_split(info->rdline, ';');
	if (!tab)
		return (NULL);
//	for (int y = 0; tab[y]; y++)
//		printf("tab[%d] = %s\n", y, tab[y]);
	while (tab[i])
	{
		ptr = ft_treenew(tab[i], ptr);
		if (!ptr)
			return (NULL);
		if (i % 2)
		{
			if (i != 1)
			{
				ft_lstadd_tree(&new, ptr, 2);
				ptr = new->right;
			}
			else
				new = ptr;
			ft_lstadd_tree(&new, tmpcmd, 1);
		}
		else
			tmpcmd = ptr;
		i++;
		if (!tab[i])
			ft_lstadd_tree(&new, tmpcmd, 2);
	}
	ft_free_split(tab, i);
	return (new);
}

void	ft_print_test(t_tree *tree, int i)
{
	while (--i > 0)
		printf("\t");
	if (tree->cmd)
		printf(">%s<\n",  tree->cmd->cmd);
}

int	ft_init_tree(t_info *info, t_token *token)
{
//	info->tree = ft_fill_tree(info, token);
	(void) token;
	info->tree = ft_fill_tree(info);
	if (!info->tree)
			return (1);
	return (0);
}

void	ft_print_tree(t_tree *tree, int i)
{
	i++;
	if (tree->left)
	{
		ft_print_tree(tree->left, i);
		ft_print_test(tree, i);
	}
	if (tree->right)
		ft_print_tree(tree->right, i);
	if (!tree->left)
		ft_print_test(tree, i);
}

int	ft_nb_andor(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = -1;
	if (str)
		while (str[++i + count])
			if ((str[i + count] == '|' && str[i + count + 1] == '|')
				|| (str[i + count] == '&' && str[i + count + 1] == '&'))
					count++;
	return (count);
}

int	ft_fill_envnew(t_env *env, char *line)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	if (!line)
		return (1);
	while (line[i] && line[i] != '=')
		i++;
	env->name = ft_calloc(sizeof(char), i + 1);
	if (!env->name)
		return (1);
	while (++j < i)
		env->name[j] = line[j];
	if (line[j] != '=')
		return (0);
	i = 0;
	j++;
	while (line[j + i])
		i++;
	env->value = ft_calloc(sizeof(char), i + 1);
	if (!env->value)
	{
		free(env->name);
		return (1);
	}
	i = 0;
	while (line[j])
		env->value[i++] = line[j++];
	return (0);
}

t_env	*ft_envnew(char *line)
{
	t_env	*new;

	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->next = NULL;
	if (ft_fill_envnew(new, line))
		return (NULL);
	return (new);
}

/*t_tree	*ft_fill_tree(t_info *info, t_token *token)
{
	t_tree	*ptr;
	t_tree	*new;
	t_token	*tok;
	int		len;

	ptr = NULL;
	new = NULL;
	tok = token;
	while (tok)
		tok = tok->next;
	while (tok)
	{
		ptr = ft_treenew(tok->cmd, ptr);
		if (tok->token == TOK_WORD)
			ft_lstadd_tree(&new, ptr, 1);
		else if (tok->token == TOK_OPERATOR)
			ft_lstadd_tree(&new, ptr, 2);
		tok = tok->prev;
	}
	return (new);
}*/

void	ft_envadd_back(t_env **alst, t_env *new)
{
	t_env	*tmp;

	tmp = NULL;
	tmp = *alst;
	if (alst && new)
	{
		if (*alst == NULL)
			*alst = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}

int	ft_init_env(t_info *info, char **envp)
{
	t_env	*ptr;
	t_env	*new;
	int		i;

	ptr = NULL;
	new = NULL;
	i = 0;
	while (envp[i])
	{
		ptr = ft_envnew(envp[i]);
		if (!ptr)
			return (1);
		ft_envadd_back(&new, ptr);
		i++;
	}
	info->env = new;
	return (0);
}

void	ft_print_env(t_env *env)
{
	t_env	*print;

	print = env;
	printf("%s=%s\n", print->name, print->value);
	if (print->next)
	{
		print = print->next;
		while (print)
		{
			printf("%s=%s\n", print->name, print->value);
			print = print->next;
		}
	}
}

int	ft_init_info(t_info *info, t_token *token)
{
	info->tree = NULL;
	if (ft_init_tree(info, token))
		return (ft_putstr_error("Error create tree\n"));
	info->status = 0;
//	ft_print_env(info->env);
	return (0);
}

void	ft_free_branch(t_tree *branch)
{
	free(branch->cmd->cmd);
	free(branch->cmd);
	branch->cmd = NULL;
	free(branch);
	branch = NULL;
}

void	ft_free_tree(t_tree *tree)
{
	if (tree->left)
		ft_free_tree(tree->left);
	if (tree->right)
		ft_free_tree(tree->right);
	ft_free_branch(tree);
}

void	ft_free_env(t_env *env)
{
	t_env *tmp;

	if (env)
	{
		while (env)
		{
			tmp = env;
			env = tmp->next;
			free(tmp->name);
			tmp->name = NULL;
			free(tmp->value);
			tmp->value = NULL;
			free(tmp);
			tmp = NULL;
		}
	}
	tmp = NULL;
}

void	ft_free_all(t_info *info)
{
	ft_free_tree(info->tree);
	free(info->rdline);
	info->rdline = NULL;
}

void	ft_do_it(t_info *info)
{
	static int	i = 0;
	char	**tab;
	t_tree	*tree;

	tree = info->tree;
	if (i == 2)
	{
		i = 0;
		return ;
	}
	if (!ft_strncmp(info->tree->cmd->cmd, "|", 2))
	{
		if (!i)
			tree = info->tree->left;
		else if (i == 1)
			tree = info->tree->right;
		i++;
	}
	if (!ft_strncmp(tree->cmd->cmd, "pwd", 3))
		if (ft_pwd())
			printf("oscour pwd\n");
	tab = ft_split(tree->cmd->cmd, ' ');
	if (!ft_strncmp(tab[0], "cd", 3))
		ft_cd(info, tab[1]);
	ft_free_split(tab, 2);
	if (i)
		ft_do_it(info);
}

char	*ft_get_env_value(t_info *info, char *name)
{
	t_env	*env;
	
	env = NULL;
	if (!info->env || !name)
		return (NULL);
	env = info->env;
	while (env && ft_strncmp(env->name, name, ft_strlen(name)))
		env = env->next;
	if (!env)
		return (NULL);
	return (env->value);
}

char	*ft_rdline_word(t_info *info)
{
	char		*word;
	char		*tmp;

	word = NULL;
	tmp = ft_get_env_value(info, "HOME");
	word = getcwd(word, 0);
	if (!word)
		return (NULL);
	if (tmp && !strncmp(tmp, word, ft_strlen(tmp)))
	{
		word = ft_substr_free(word, ft_strlen(tmp), ft_strlen(word) - ft_strlen(tmp));
		word = ft_strjoin_free("minishell:~", word, 2);
	}
	else
		word = ft_strjoin_free("minishell:", word, 2);
	word = ft_strjoin_free(word, "$ ", 1);
	return (word);
}

int	main(int ac, char **av, char **envp)
{
	t_token		*tokens;
	t_info		info;
	char		*word;

	(void) av;
	info.rdline = NULL;
	tokens = NULL;
	info.status = 0;
	if (ac > 1)
		return (ft_putstr_fd("Too much arguments\n", 2), 1);
	if (envp)
		if (ft_init_env(&info, envp))
			return (ft_putstr_error("Error create env\n"));
	signal(SIGINT, &ft_signal);
	while (1)
	{
		word = ft_rdline_word(&info);
		if (!word)
			return (1);
		info.rdline = readline(word);
		free(word);
		if (!info.rdline || !ft_exit(info.rdline))
			break ;
		if (ft_keep_history(info.rdline))
			add_history(info.rdline);
		if (ft_init_info(&info, tokens))
			return (1);
		ft_do_it(&info);
		ft_free_all(&info);
	}
	rl_clear_history();
	ft_free_env(info.env);
	return (info.status);
}
