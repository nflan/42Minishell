/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:10:15 by nflan             #+#    #+#             */
/*   Updated: 2022/06/26 19:18:35 by omoudni          ###   ########.fr       */
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
	TOK_D_QUOTER,
	TOK_QUOTER,
	TOK_S_QUOTER,
	TOK_EXPANDER_OP,
	TOK_EXPANDER_CL,
	TOK_OPERATOR,
	TOK_PATH,
	TOK_REDIRECTOR_LEFT,
	TOK_REDIRECTOR_RIGHT,
	TOK_WORD,
	TOK_WORD_NULL_S,
	TOK_WORD_NULL_D,
	TOK_WORD_S_QUOTED,
	TOK_WORD_D_QUOTED,
	TOK_EXPANDER,
} 			t_tok_type;

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
} 			t_char_type;

typedef struct s_token
{
	//	int						sp_after;
	int		index;
	int 	start;
	int 	length;
	char	*value;
	t_tok_type token;
	int 	quoted; //1- dol quoted 2- d quoted 3- s quoted
	struct s_token *prev;
	struct s_token *next;
} 			t_token;

typedef enum s_big_tok_type
{
	TOK_LEFT_OR,
	TOK_LEFT_AND,
	TOK_LEFT_PIPE,
	TOK_CLEAN,
	TOK_CLEAN_PIPED,
	TOK_LAST,
	TOK_PIPE_LAST,
	// TOK_OR_LEFT,
	// TOK_AND_RIGHT,
	// TOK_PIPE_LEFT,
	// TOK_PIPE_RIGHT,
} 			t_big_tok_type;

typedef	struct	s_wildcards
{
	DIR					*fd;
	struct dirent		*dir;
	struct s_wildcards	*next;
}	t_wildcards;

typedef enum	s_par_left_right
{
	NOT_A_PAR,
	PAR_LEFT_OR,
	PAR_RIGHT_OR,
	PAR_LEFT_AND,
	PAR_RIGHT_AND,
	PAR_PIPE_LEFT,
	PAR_PIPE_RIGHT,
}				t_par_left_right;

/*typedef struct s_big_token
{
	t_big_tok_type	type;
	int				ind_tok_start;
	int				length;
	int				par;
	int				hd;
	int				red_in;
	int				fdin;
	int				err_in;
	int				red_out;
	int				fdout;
	int				err_out;
	char			*cmd;
	int				sc;
	int				done;
	struct s_big_token *parent;
	struct s_big_token *child;
	struct s_big_token *sibling;
} 			t_big_token;*/

//	int					*err_in; // potentiellement erreur d'ouverture
//	int					*err_out;
//	int					rd_inouthd[3]; //0 -> nb de fdin - 1 -> nb de fdout 2-> nb de heredoc
//	int					*red_in;
//	int					*red_out; // donne l'info sur le type d'ecriture sur le out aka > ou >>
//	char				**delimitator; // tableau de delimitateur de here doc (demension== rd_inouthd[2])
//	char				**infile; // tableau de fichier a ouvrir en in (dimension = rd_inouthd[0])
//	char				**outfile;  // tableau de fichier a ouvrir en out (dimension = rd_inouthd[1])
//	int					*fdin; // tableau malloc avec la dimension rd_inouthd[0] a remplir lors de l'extraction des FD
//	int					*fdout;

typedef struct s_fd
{
	int					fd; // fd in ou out (que je rempli a l'exec SI PAS HEREDOC)
	int					red; // if 1 >> if 0 >. Si fd_in && 1, heredoc
	char				*file; // nom du ficher infile ou outfile ou heredoc
	char				*delimitator; // delimitateur si heredoc sinon NULL
	struct s_fd			*next;
}	t_fd;

typedef struct s_big_token
{
	t_big_tok_type		type;
	int					ind_tok_start;
	int					length;
	int					par;
	int					fdin;
	int					fdout;
	int					nb_hd;
	int					cmd_args_num;
	char				**cmd_args;
	char				**envp;
	int					sc;
	t_fd				*fd_in;
	t_fd				*fd_out;
	struct s_big_token	*parent;
	struct s_big_token	*child;
	struct s_big_token	*sibling;
} 	t_big_token;

static const t_char_type get_char_class[255] =
	{
		['\0'] = CHR_NULL,
		['\t'] = CHR_SPACE,
		[' '] = CHR_SPACE,
		['!'] = CHR_EXCLAM,
		['\"'] = CHR_D_QUOTE,
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
		[CHR_D_QUOTE] = TOK_D_QUOTER,
		[CHR_COMMENT] = TOK_IDK,
		[CHR_DOL] = TOK_WORD,
		[CHR_PER] = TOK_IDK,
		[CHR_AND] = TOK_OPERATOR,
		[CHR_S_QUOTE] = TOK_S_QUOTER,
		[CHR_OP_PAREN] = TOK_EXPANDER_OP,
		[CHR_CL_PAREN] = TOK_EXPANDER_CL,
		[CHR_AST] = TOK_WORD,
		[CHR_PLUS] = TOK_WORD, // j'ai change c'etait operator
		[CHR_DASH_MINES] = TOK_WORD, // j'ai change c'etait operator
		[CHR_POINT] = TOK_WORD,
		[CHR_SLASH] = TOK_WORD,
		[CHR_NUM] = TOK_WORD,
		[CHR_COLON] = TOK_PATH,
		[CHR_SEMI_COLON] = TOK_WORD,
		[CHR_MINES] = TOK_REDIRECTOR_LEFT,
		[CHR_EQUAL] = TOK_WORD, // j'ai change c'etait operator
		[CHR_SUPERIOR] = TOK_REDIRECTOR_RIGHT,
		[CHR_INTEROG] = TOK_WORD,
		[CHR_AT] = TOK_WORD,
		[CHR_ALPHA] = TOK_WORD,
		[CHR_OP_BRACKET] = TOK_WORD, // j'ai change c'etait IDK
		[CHR_ANTI_SLASH] = TOK_WORD, // on doit pas gerer les '\' donc j'ai mis en WORD (ancien quoter)
		[CHR_CL_BRACKET] = TOK_WORD, // j'ai change c'etait IDK
		[CHR_CIRCUM] = TOK_IDK,
		[CHR_UNDERSCORE] = TOK_WORD,
		[CHR_OP_BRACE] = TOK_IDK,
		[CHR_PIPE] = TOK_OPERATOR,
		[CHR_CL_BRACE] = TOK_IDK,
		[CHR_TILDA] = TOK_WORD, // j'ai change c'etait PATH
		[CHR_BACKTICK] = TOK_IDK,
};

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_info
{
	char		*rdline;
	int			status;
	int			nb_cmd;
	int			tmp_start;
	t_env		*env;
	t_big_token	*parse;
	t_token		*old_tokens;
	t_token		*tokens;
	int			*pid;
	int			pdes[2];
	int			tmp[2];
}	t_info;

//-----------main.c------------------------------------------
void	ft_envadd_back(t_env **alst, t_env *new);
int		ft_fill_envnew(t_env *env, char *line);
t_env	*ft_envnew(char *line);
void	ft_cmd_signal(int sig);
void	ft_signal(int sig);

//-----------ft_launch_cmd----------------------------------------
int	ft_exit_cmd(t_info *info, char *str, int err);
int	ft_wash_btoken(t_info *info, t_big_token *b_tokens);
int	ft_check_builtins(t_big_token *b_tokens);
int	ft_builtins(t_info *info, t_big_token *b_tokens);
int	ft_launch_sibling(t_info *info, t_big_token *b_tokens);
void	ft_close_cmd(t_info *info, t_big_token *b_tokens, pid_t child);
int	ft_lead_fd(t_info *info, t_big_token *b_tokens);
int	ft_launch_cmd(t_info *info, t_big_token *b_tokens);
int	ft_launch_cmd_pipex(t_info *info, t_big_token *b_tokens, int pid);
int	ft_find_cmd(t_info *info);

//-----------builtins----------------------------------------
int		ft_pwd(void);
int		ft_env(t_info *info);
//int	ft_exit(t_info *env, int ret);
int		ft_exit(t_info *info, t_big_token *b_tokens);
t_env	*ft_unset(t_info *info, t_big_token *b_tokens);
//-----------ft_cd.c-----------------------------------------
int		ft_cd(t_info *info, t_big_token *b_tokens);
//-----------ft_export.c-------------------------------------
int		ft_export(t_info *info, t_big_token *b_tokens);
//-----------ft_echo.c-------------------------------------
int		ft_echo(t_info *info, t_big_token *b_tokens);
char	*ft_create_echo(t_info *info, t_big_token *b_tokens, char *tmp, int i);
int		ft_echo_none(t_big_token *b_tokens, int i);
int		ft_handle_ret(t_big_token *b_tokens, char *ret, int i);
int		ft_option_echo(t_big_token *b_tokens, int i);

//-----------ft_tools.c--------------------------------
int	ft_putstr_error(char *error);
int	ft_putstr_frror(char *error, char *str, int i);
int	ft_perror(char *error, char *str);
int	ft_perror_free(char *error, char *str, int i);
char	*ft_get_env_value(t_info *info, char *name);

//----------ft_pipex.c---------------------------------
int				ft_do_pipex(t_info *info, t_big_token *b_tokens);
int				ft_pipex(t_info *info, t_big_token *b_tokens);

//---------ft_here_doc.c----------------------------
void			ft_write_here(t_fd *fd, char *str, int i);
int				ft_here(t_fd *fd);
char			**ft_env_to_tab(t_env *env);

//---------ft_pipex_utils.c----------------------------
int			ft_cmd_path(t_info *info, t_big_token *b_tokens);
int			ft_command(t_info *info, t_big_token *b_tokens);

//---------ft_pipex_tools2.c---------------------------
void	ft_error_2(int i, t_info *info, t_big_token *b_tokens);
int		ft_error(int i, t_info *info, t_big_token *b_tokens);

//-----------------ft_free.c---------------------------
void	ft_signal(int sig);
void	ft_cmd_signal(int sig);
void	ft_manage_sig(int sig);

//-----------------ft_free.c---------------------------
void	ft_free_wildcards(t_wildcards *wd);
void	ft_free_all(t_info *info, t_env *env);
void	ft_free_b_tokens(t_big_token *b_tokens);
void	ft_free_fd(t_fd *fd);
void	ft_free_cmd(t_big_token *b_tokens);
//-----------------ft_free2.c--------------------------
void	ft_free_tokens(t_token *tokens);
void	ft_free_env(t_env *env);

// WILDCARDS
//-----------------ft_wildcards_check.c---------------------------
int		ft_check_wildcards(t_info *info, t_big_token *b_tokens, int i);
int		ft_add_wildcards(t_info *info, t_big_token *b_tokens);
int		ft_keep(char *str, char *dir, int *i, int j);
int		ft_do_keep(char *str, t_wildcards *wd, int type);
int		ft_wd_nb_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type);

//-----------------ft_wildcards_tools.c---------------------------
void	ft_wdadd_back(t_wildcards **alst, t_wildcards *new);
int		ft_wildcardsnew(t_wildcards **wd, struct dirent *dir, DIR *fd);
int		ft_strlen_before_ast(char *str);
int		ft_manage_type(char *str, char *dir, int d_type, int type);
int		ft_get_wildcards(t_wildcards **wd);

//-----------------ft_wildcards_do.c---------------------------
char	**ft_fill_old_args(t_big_token *b_tokens, char **tmp, int j, int list);
int		ft_realloc_args(t_wildcards *wd, t_big_token *b_tokens, int i, int type);
int		ft_do_wildcards(t_big_token *b_tokens, int i);

// AGENT O
//----------main_O.c-------------------------------------------------------------------

int				main_agent_O(t_info *info);
char			*concat_argvs(int argc, char **argv);

//----------parser.c------------------------------------------------------------------

void			give_parent(t_big_token **b_child, t_big_token **parent);
void			sub_parse_1(t_big_token **tmp_b, t_token **tokens, int b_start, int b_length);
void			sub_parse_2(t_big_token **b_child, t_big_token **tmp_b, t_token **tokens);
int				parse(t_big_token **b_tokens, t_token **tokens, int start, int length);
void			extract_fds(t_big_token **tmp_b, t_token **tokens);

//----------executer.c-----------------------------------------------------------------

int				rec_exec(t_info *info, t_big_token **b_tokens, int and_or);
void			ft_close_fd(t_big_token *b_tokens);

//----------printer.c-------------------------------------------------------------------
void			print_tab(char **tab);
void			print_all_everything(t_big_token **b_tokens, t_token **tokens);
void			print_all_child(t_big_token **b_tokens, t_token **tokens, int i, int j);
void			print_b_tokens(t_big_token **b_tokens, t_token **tokens, int i, int j);
void			print_s_tokens(t_token **tokens, int start, int length);

//----------tokenizer_1.c-------------------------------------------------------------

int 			len_ll_list(t_token *tok_list);
int 			is_quoted(t_token **tok_list, char c);
unsigned int	get_real_tok_type(char c, t_token **tok_list);
t_token 		*ft_create_token(t_tok_type tok_type, int length, int i);
t_token			*create_tok_bis(t_tok_type tok_type, int quoted, char *value);
void 			init_tok_struct(t_token **tok_list, int rank_in_list);

//----------tokenizer_2.c----------------------------------------------------

int				add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i);
void			add_tok_last_bis(t_token **tok_list, t_tok_type tok_type, int quoted, char *value);
int				detect_tokens(t_token **tok_list, char *str);
int				fill_tok_value(t_token **tok, char *str);
char 			*ft_strncpy(char *str, int n);
void			index_toks(t_token **tokens);

//-----------syntax_errorinizer_1.c--------------------------------------------------

int 			count_op_tok(t_token **tokens);
int 			count_cl_tok(t_token **tokens);
int 			check_count_errors(t_token **tokens);
int 			r_quotes_impair(t_token **tokens);
int 			is_last_op(t_token **tokens);

//-----------syntax_errorinizer_2.c----------------------------------------

int				r_2_op_succeding(t_token **tokens);
int				op_cl_par_succeeding(t_token **tokens);
int				syntax_err_handler(t_token **tokens);
int				is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens);
int				is_there_par(t_big_token *b_tokens, t_token *tokens);
int				is_red_st_par(t_big_token *b_tokens, t_token *tokens);

//-----------big_tokenizer_1.c-------------------------------------------------

t_big_token		*ft_create_btoken(t_big_tok_type type, int ind_tok_start, int length);
int				add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length);
int				check_divider_type(char *tmp_value);
// int				is_cl_2_op(char *value_tok_op, char *value_tok_cl);
void			move_tok_2_ind(t_token **tokens, int ind);

//-----------big_tokenizer_2.c-----------------------------------------------------

int				cl_par_ind(t_token **tokens, int ind_tok);
int				divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start, int length);
int	piped(t_token **tokens, int start, int length);
int				sophisticated_piped(t_token **tokens, int start, int length);

//-----------big_tokenizer_4.c---------------------------------------------------------

int			handle_par(t_big_token **b_tokens, t_token **tokens);

//-----------big_tokenizer_3.c--------------------------------------------------

// void		divide_by_or_and(t_big_token **b_tokens, t_token **tokens);
int			divide_by_pipe(t_big_token **b_tokens, t_token **tokens);

//------------printer.c--------------------------------------------------------
void	ft_print_wildcards(t_wildcards *wd);
void print_b_tokens(t_big_token **b_token, t_token **tokens, int i, int j);
void print_s_tokens(t_token **tokens, int start, int length);
int	depth_b_token(t_big_token **b_token);
void	print_all_everything(t_big_token **b_tokens, t_token **tokens);
void	print_all_child(t_big_token **b_tokens, t_token **tokens, int i, int j);

//---------------dollar_expander.c------------------------------------------
char			*expand_join(char *s1, char *s2, char *s3);
char			*expand_join_nf(char *s1, char *s2, char *s3);
char			*strjoin_4(char *str1, char *str2);
char			*str_join_exp(t_token **tokens, int ind, int type);
char			*ft_strndup(char *str, int len);
void			expand_1(char **str, int *i, t_info *info);
void			expand(char **str, t_info *info);
void			dol_expand(t_token **old_tokens, t_info *info, int start, int length);
int				expanded_toks_check(t_token **tokens);
void			expanded_toks(t_token **old_tokens, int start, int length);

#endif
