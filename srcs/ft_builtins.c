/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:22:55 by nflan             #+#    #+#             */
/*   Updated: 2022/06/03 17:34:14 by nflan            ###   ########.fr       */
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

void	ft_env(t_env *env)
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
}

void	ft_echo(char *line, int fd, int ret)
{
	char	*tmp;
	int		i;

	i = 2;
	tmp = NULL;
	if (!line)
		return ;
	if (!ft_strncmp(line, "$?", 3))
		tmp = ft_itoa(ret);
	if ((line[0] == '-' || line[1] == 'n'))
	{
		while (line[i] && line[i] == 'n')
			i++;
		if (line[i] == ' ')
		{
			tmp = ft_substr(line, i + 1, ft_strlen(line) - i - 1);
			if (!tmp)
				return ;
		}
	}
	if (!tmp)
		tmp = ft_strjoin(line, "\n");
	if (!tmp)
		return ;
	ft_putstr_fd(tmp, fd);
	free(tmp);
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

int	ft_convert_cmd(t_big_token *tok, t_token *tokens)
{
	t_token	*tmp;
	char	*line;

	(void)tok;
	line = NULL;
	tmp = tokens;
	ft_print_tokens(tokens);
	while (tmp)
	{
		if (tmp->start >= 6 && tmp->start <= 17)
			line = ft_strjoin_free(line, tmp->value, 1);
		tmp = tmp->next;
	}
	printf("LINE = %s\n", line);
	ft_echo(line, 1, 0);
	return (0);
}

/*int	main(int ac, char **av)
{
	t_big_token	*tok;
	t_token *tokens;
	(void) ac;

	tokens = NULL;
	detect_tokens(&tokens, av[1]);
	fill_tok_value(&tokens, av[1]);
	tok = ft_calloc(sizeof(t_big_token), 1);
	tok->type = TOK_CLEAN;
	tok->ind_tok_start = 0;
	tok->length = 3;
	tok->par = 0;
	tok->parent = NULL;
	tok->child = NULL;
	tok->sibling = NULL;
	divide_by_pipe(&tok, &tokens);
	ft_convert_cmd(tok, tokens);
	return (0);
}*/

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

t_env	*ft_unset(t_env *env, char *line)
{
	t_env	*tmp;
	t_env	*ptr;

	tmp = env;
	ptr = NULL;
	if (!env || !line)
		return (NULL);
	while (tmp->next && ft_strncmp(tmp->next->name, line, ft_strlen(line + 1)))
		tmp = tmp->next;
	if (!tmp->next && ft_strncmp(tmp->name, line, ft_strlen(line + 1)))
		return (NULL);
	ptr = tmp->next;
	tmp->next = ptr->next;
	ptr->next = NULL;
	ft_free_env(ptr);
	return (env);
}
