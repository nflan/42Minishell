/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:10:15 by nflan             #+#    #+#             */
/*   Updated: 2022/05/31 12:25:32 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>

typedef enum s_tok_type
{
	TOK_IDK,
	TOK_SEP,
	TOK_QUOTER,
	TOK_EXPANDER_OP,
	TOK_EXPANDER_CL,
	TOK_OPERATOR,
	TOK_PATH,
	TOK_WORD,
	TOK_EXPANDER,
} t_tok_type;

typedef enum s_char_type
{
	CHR_NULL,
	CHR_SPACE,
	CHR_EXCLAM,
	CHR_D_QUOTE,
	CHR_COMMENT,
	CHR_DOL,
	CHR_PER,
	CHR_AND,
	CHR_S_QUOTE,
	CHR_OP_PAREN,
	CHR_CL_PAREN,
	CHR_AST,
	CHR_PLUS,
	CHR_DASH_MINES,
	CHR_POINT,
	CHR_SLASH,
	CHR_NUM,
	CHR_COLON,
	CHR_SEMI_COLON,
	CHR_MINES,
	CHR_EQUAL,
	CHR_SUPERIOR,
	CHR_INTEROG,
	CHR_AT,
	CHR_ALPHA,
	CHR_OP_BRACKET,
	CHR_ANTI_SLASH,
	CHR_CL_BRACKET,
	CHR_CIRCUM,
	CHR_UNDERSCORE,
	CHR_BACKTICK,
	CHR_OP_BRACE,
	CHR_PIPE,
	CHR_CL_BRACE,
	CHR_TILDA,
} t_char_type;

typedef	struct 				s_token
{
	char					*cmd;
	int						sp_before;
//	int						sp_after;
	int						length;
	t_tok_type				token;
	int						quoted;
	struct s_token			*prev;
	struct s_token			*next;
}							t_token;


static const t_char_type get_char_class[255] =
	{
		['\0'] = CHR_NULL,
		['\t'] = CHR_SPACE,
		[' '] = CHR_SPACE,
		['!'] = CHR_EXCLAM,
		['"'] = CHR_D_QUOTE,
		['#'] = CHR_COMMENT,
		['$'] = CHR_DOL,
		['%'] = CHR_PER,
		['&'] = CHR_AND,
		['\''] = CHR_S_QUOTE,
		['('] = CHR_OP_PAREN,
		[')'] = CHR_CL_PAREN,
		['*'] = CHR_AST,
		['+'] = CHR_PLUS,
		['-'] = CHR_DASH_MINES,
		['.'] = CHR_POINT,
		['/'] = CHR_SLASH,
		['0' ... '9'] = CHR_NUM,
		[':'] = CHR_COLON,
		[';'] = CHR_SEMI_COLON,
		['<'] = CHR_MINES,
		['='] = CHR_EQUAL,
		['>'] = CHR_SUPERIOR,
		['?'] = CHR_INTEROG,
		['@'] = CHR_AT,
		['A' ... 'Z'] = CHR_ALPHA,
		['['] = CHR_OP_BRACKET,
		['\\'] = CHR_ANTI_SLASH,
		[']'] = CHR_CL_BRACKET,
		['^'] = CHR_CIRCUM,
		['_'] = CHR_UNDERSCORE,
		['`'] = CHR_BACKTICK,
		['a' ... 'z'] = CHR_ALPHA,
		['{'] = CHR_OP_BRACE,
		['|'] = CHR_PIPE,
		['}'] = CHR_CL_BRACE,
		['~'] = CHR_TILDA,
};

static const t_tok_type get_tok_type[255] =
	{
		[CHR_NULL] = TOK_IDK,
		[CHR_SPACE] = TOK_SEP,
		[CHR_EXCLAM] = TOK_IDK,
		[CHR_D_QUOTE] = TOK_QUOTER,
		[CHR_COMMENT] = TOK_IDK,
		[CHR_DOL] = TOK_EXPANDER,
		[CHR_PER] = TOK_IDK,
		[CHR_AND] = TOK_QUOTER,
		[CHR_S_QUOTE] = TOK_QUOTER,
		[CHR_OP_PAREN] = TOK_EXPANDER_OP,
		[CHR_CL_PAREN] = TOK_EXPANDER_CL,
		[CHR_AST] = TOK_EXPANDER,
		[CHR_PLUS] = TOK_OPERATOR,
		[CHR_DASH_MINES] = TOK_OPERATOR,
		[CHR_POINT] = TOK_PATH,
		[CHR_SLASH] = TOK_PATH,
		[CHR_NUM] = TOK_WORD,
		[CHR_COLON] = TOK_PATH,
		[CHR_SEMI_COLON] = TOK_WORD,
		[CHR_MINES] = TOK_OPERATOR,
		[CHR_EQUAL] = TOK_OPERATOR,
		[CHR_SUPERIOR] = TOK_OPERATOR,
		[CHR_INTEROG] = TOK_OPERATOR,
		[CHR_AT] = TOK_WORD,
		[CHR_ALPHA] = TOK_WORD,
		[CHR_OP_BRACKET] = TOK_EXPANDER_OP,
		[CHR_ANTI_SLASH] = TOK_QUOTER,
		[CHR_CL_BRACKET] = TOK_EXPANDER_CL,
		[CHR_CIRCUM] = TOK_IDK,
		[CHR_UNDERSCORE] = TOK_WORD,
		[CHR_OP_BRACE] = TOK_EXPANDER_OP,
		[CHR_PIPE] = TOK_OPERATOR,
		[CHR_CL_BRACE] = TOK_EXPANDER_CL,
		[CHR_TILDA] = TOK_PATH,
		[CHR_BACKTICK] = TOK_IDK,
};

typedef struct s_cmd
{
	char			*cmd;
	char			**cmd_p;
	char			**envp;
	int				fdin;
	int				fdout;
	pid_t			child;
//	int				type;// && = 1 / || = 2 -> j'envoie dans X ou Y fonction au cas ou
//	unsigned int	prof;// profondeur
	struct s_tree	*tree;
}	t_cmd;

typedef struct s_tree
{
	t_cmd			*cmd;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_info
{
	char	*rdline;
	int		status;
	t_tree	*tree;
	t_env	*env;
	int		pdes[2];
}	t_info;

//----------tokenizer.c--------------------------------------

void	ft_fill_tab(int n, int **tab, t_tok_type *get_tok_type);
void	fill_them_tables(int **tab, int size, t_tok_type *get_tok_type);
t_token	*ft_create_token(t_tok_type tok_type);
void	add_tok_last(t_token **tok_list, t_tok_type tok_type, int length);
int		is_quoted(t_token **tok_list, int rank_in_list);
void	init_tok_struct(t_token **tok_list, int	rank_in_list, int length);
void	detect_tokens(t_token **tok_list, char *str);

//-----------main.c------------------------------------------
void	ft_free_env(t_env *env);
void	ft_free_all(t_info *info, t_env *env);
void	ft_envadd_back(t_env **alst, t_env *new);
int		ft_fill_envnew(t_env *env, char *line);
t_env	*ft_envnew(char *line);
void	ft_print_cmd(t_cmd *cmd);

//-----------builtins----------------------------------------
void	ft_echo(char *line, int fd, int ret);
int	ft_pwd(void);
void	ft_env(t_env *env);
//int	ft_exit(t_info *env, int ret);
int	ft_exit(t_info *env, char *value, char **tofree);
t_env	*ft_unset(t_env *env, char *line);
//-----------ft_cd.c-----------------------------------------
int	ft_cd(t_info *info, char *dir);
//-----------ft_export.c-------------------------------------
void	ft_export(t_env *env, char *line);

//-----------ft_tools.c--------------------------------
int	ft_putstr_error(char *error);
int	ft_putstr_frror(char *error, char *str, int i);
int	ft_perror(char *error, char *str);
int	ft_perror_free(char *error, char *str, int i);
char	*ft_get_env_value(t_info *info, char *name);

//----------ft_pipex.c---------------------------------
int				ft_pipex_end(t_info *info, t_cmd *cmd);
int				ft_do_pipe(t_info *info, t_cmd *cmd, int tmp);
int				ft_pipe_to_pipe(t_info *info, t_cmd *cmd);
char			**ft_env_to_tab(t_env *env);
int				ft_do_pipex(t_info *info, t_cmd *cmd);
int				ft_pipex(t_info *info, t_cmd *cmd);

//---------ft_pipex_tools.c----------------------------
int				ft_fdout_me(t_info *info);
int				ft_here(t_info *info, int i);
int				close_pipex_heredoc(t_info *info);
int				ft_do_heredoc(t_info *info);
int				ft_pipex_heredoc(t_info *g);

//---------ft_pipex_utils.c----------------------------
int			ft_cmd_path(t_info *info, t_cmd *cmd);
int			ft_command(t_info *info, t_cmd *cmd);

//---------ft_pipex_tools2.c---------------------------
void	ft_error_2(t_info *info, t_cmd *cmd);
int		ft_error(int i, t_info *info, t_cmd *cmd);

#endif
