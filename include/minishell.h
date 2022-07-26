/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 15:10:15 by nflan             #+#    #+#             */
/*   Updated: 2022/07/26 16:53:20 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define LLMAX "9223372036854775807"
# define LLMIN "-9223372036854775808"

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
}			t_tok_type;

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
}			t_char_type;

typedef enum s_big_tok_type
{
	TOK_LEFT_OR,
	TOK_LEFT_AND,
	TOK_LEFT_PIPE,
	TOK_CLEAN,
	TOK_CLEAN_PIPED,
	TOK_LAST,
	TOK_PIPE_LAST,
}			t_big_tok_type;

typedef enum s_par_left_right
{
	NOT_A_PAR,
	PAR_LEFT_OR,
	PAR_RIGHT_OR,
	PAR_LEFT_AND,
	PAR_RIGHT_AND,
	PAR_PIPE_LEFT,
	PAR_PIPE_RIGHT,
}				t_par_left_right;

// quoted -> 1- dol quoted - 2- d quoted - 3- s quoted

typedef struct s_token
{
	int				index;
	int				start;
	int				length;
	char			*value;
	t_tok_type		token;
	int				quoted;
	struct s_token	*prev;
	struct s_token	*next;
}			t_token;

typedef struct s_wildcards
{
	DIR					*fd;
	struct dirent		*dir;
	struct s_wildcards	*next;
}	t_wildcards;

// fd -> fd in ou out (que je rempli a l'exec SI PAS HEREDOC)
// red -> if 1 >> if 0 >. Si fd_in && 1, heredoc
// file -> nom du ficher infile ou outfile ou heredoc
// delimitator -> delimitateur si heredoc sinon NULL

typedef struct s_fd
{
	int					fd;
	int					red;
	int					inout;
	char				*file;
	char				*delimitator;
	struct s_fd			*next;
	struct s_info		*info;
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
	t_fd				*fd;
	struct s_big_token	*parent;
	struct s_big_token	*child;
	struct s_big_token	*sibling;
	struct s_info		*info;
}	t_big_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_info
{
	char		*rdline;
	t_tok_type	tok_type_tab[128];
	int			status;
	int			nb_cmd;
	t_env		*env;
	t_big_token	*parse;
	t_token		*tokens;
	int			*pid;
	char		*home;
	int			pdes[2];
	int			tmp[2];
}	t_info;

typedef struct s_tmp
{
	t_token		*s;
	t_big_token	*b;
}				t_tmp;

typedef struct s_tab
{
	int			params[2];
	int			stend_par[2];
}				t_tab;

//-----------main.c-------------------------------------------------------------
void			ft_end(t_info *info);
int				ft_launch_minishell(t_info info, char *word);
//-----------ft_start.c---------------------------------------------------------
void			ft_keep_history(t_info *info, char *str);
int				ft_init_info(t_info *info);
char			*ft_rdline_word(t_info *info);
int				ft_init_first(t_info *info, char **envp);
//-----------ft_env_tools.c-----------------------------------------------------
void			ft_envadd_back(t_env **alst, t_env *new);
t_env			*ft_envnew(char *line);
t_env			*ft_without_env(int i);
int				ft_fill_envnew(t_env *env, char *line, int i, int j);
//-----------ft_env_tools2.c----------------------------------------------------
int				ft_init_env(t_info *info, char **envp);
//-----------ft_global.c--------------------------------------------------------
int				ft_change_cmd(t_big_token *b_tokens, char *tofree);
int				ft_path(t_info *info, t_big_token *b_tokens, int err);
int				ft_cmd_nopath(t_big_token *b_tokens);
int				ft_is_cmd(t_big_token *b_tokens, t_info *info);
int				ft_commanding(t_info *info, t_big_token *b_tokens);
//-----------ft_global_bis.c----------------------------------------------------
int				ft_command(t_info *info, t_big_token *b_tokens);
char			*strjoin_4(char *str1, char *str2);

// FD
//-----------ft_fd_tools.c------------------------------------------------------
int				ft_fill_fdnew(t_fd *fd, t_token **tmp, int itscl[5], int *hd);
int				ft_fdnew(t_big_token *b_toks, t_fd **fd, t_token **t, int r[7]);
int				ft_create_tmp(t_fd *fd, int hd);
char			*ft_create_del(t_token **tmp, int *red);
void			ft_fdadd_back(t_fd **alst, t_fd *new);
//-----------ft_fd_tools_bis.c--------------------------------------------------
int				create_del_cond(t_tok_type tok);
//-----------ft_fd_open.c-------------------------------------------------------
int				ft_open_all_fdout(t_big_token *b_tokens, t_fd *fd, t_info *inf);
int				ft_open_all_fdin(t_big_token *b_tokens, t_fd *fd, t_info *inf);
int				ft_open_fd(t_big_token *b_tokens, t_info *info);
//-----------ft_fd_close.c------------------------------------------------------
void			ft_close_all_fd(t_fd *fd);
void			ft_close_fd(t_big_token *b_tokens);
//-----------ft_exec.c----------------------------------------------------------
int				ft_expanding(t_info *info, t_big_token *b_tokens);
int				ft_exec_simple(t_info *info, t_big_token *b_tokens);
int				exec_the_bulk(t_info *info, int sib_child, t_big_token *b);
//-----------ft_launch_cmd------------------------------------------------------
int				ft_exit_cmd(t_info *info, char *str, int err);
int				ft_fork_par(t_info *info, t_big_token *b_tokens);
int				ft_change__(t_env *env, t_big_token *b_tokens);
int				ft_do_solo(t_info *info, t_big_token *b_tokens, int ret);
int				ft_launch_cmd(t_info *info, t_big_token *b_tokens);
//-----------ft_launch_tools.c--------------------------------------------------
void			ft_close_pdes(int fd, int pdes);
void			ft_close_cmd(t_info *info, t_big_token *b_tokens);
int				ft_lead_fd(t_info *info, t_big_token *b_tokens);
int				ft_wash_btoken(t_info *info, t_big_token *b_tokens);

// BUILTINS
//-----------ft_launch_built.c--------------------------------------------------
int				ft_check_builtins(t_big_token *b_tokens);
int				ft_builtins(t_info *info, t_big_token *b_tokens);
//-----------ft_builtins--------------------------------------------------------
int				ft_pwd(t_big_token *b_tok);
int				ft_unset_name(t_env **tmp, char *name);
void			unset_tool(t_env **ptr, t_env **tmp, t_info **info, int type);
int				ft_unset(t_info *info, t_big_token *b_tokens, int err);
//-----------ft_exit------------------------------------------------------------
int				ft_ex_val_1(int *i, char *cmd_args_1);
int				ft_ex_val_2(int *i, char *cmd_args_1);
int				ft_ex_val(t_big_token *b);
int				ft_mystic_exit(t_big_token *b, unsigned long long *ret);
int				ft_exit(t_info *info, t_big_token *b_tokens);
//-----------ft_env-------------------------------------------------------------
int				ft_env_err(t_big_token *b, int arg, int i);
int				ft_env_error(t_big_token *b, int arg);
int				ft_env_args(t_big_token *b_tok, int *i);
int				ft_env(t_info *info, t_big_token *b_tok, int i);
//-----------ft_cd.c------------------------------------------------------------
int				ft_do_tilde(t_info *info, char *arg, char *home, char *new_dir);
int				ft_unset_solo(t_info *info, char *name);
int				ft_do_cd(t_info *info, t_big_token *b, char *op);
int				ft_cd_home(t_info *info, char *home, char *op);
int				ft_cd(t_info *info, t_big_token *b_tokens, int err);
//-----------ft_cd_2.c----------------------------------------------------------
int				ft_is_tilde_or_home(char *home, char *dir);
char			*ft_cd_tilde(t_info *info, char *home, char *dir);
int				ft_back(t_env *env, char *op, char *home);
//-----------ft_cd_tools.c------------------------------------------------------
void			ft_cd_tool(t_info *info, char **home, char **new_dir);
int				ft_newpwd(t_info *info);
int				ft_oldpwd(t_info *info);
//-----------ft_export.c--------------------------------------------------------
int				ft_exp_err(char *line, int i);
int				ft_not_valid_id(char *line);
int				ft_check_export(char *line);
int				ft_export_new(t_env *env, t_env *tmp, char *line);
int				ft_export_replace(t_env *env, char *line, int i);
//-----------ft_export_bis.c----------------------------------------------------
int				ft_if_eg(t_info *info, t_env *tmp, char *line, int i);
int				ft_ifnot_eg(t_info *info, t_env *tmp, char *line, int i);
int				ft_do_export(t_info *info, t_big_token *b_tok, t_env *tmp,
					int i);
int				ft_export(t_info *info, t_big_token *b_tokens);
//-----------ft_export_tools.c--------------------------------------------------
int				ft_export_concat(t_env *env, char *line, int i);
void			ft_export_solo(t_env *env, t_big_token *b_tokens);
//-----------ft_echo.c----------------------------------------------------------
int				ft_echo(t_big_token *b_tokens);
void			ft_create_echo(t_big_token *b_tokens, int i);
int				ft_echo_none(t_big_token *b_tokens, int i);
int				ft_handle_ret(t_big_token *b_tokens, char *ret, int i);
int				ft_option_echo(t_big_token *b_tokens);

//-----------ft_tools.c---------------------------------------------------------
int				ft_putstr_error(char *error);
int				ft_putstr_frror(char *error, char *str, int i);
int				ft_perror(char *error, char *str);
int				ft_perror_free(char *error, char *str, int i);
char			*ft_get_env_value(t_info *info, char *name);
//----------ft_pipex.c----------------------------------------------------------
int				ft_dupping(t_info *info, int *fd_r, int fd_o);
int				ft_pipex(t_info *info, t_big_token *b_tokens);
int				ft_launch_cmd_pipex(t_info *info, t_big_token *b, pid_t *pid);
int				ft_exec_pipex(t_info *info, t_big_token *b_tokens, int *pid);
int				ft_init_pipex(t_info *info, t_big_token *b_tokens);
//----------ft_pipex_2.c--------------------------------------------------------
int				ft_do_pipex(t_info *info, t_big_token *b_tokens, int ret);
//---------ft_here_doc.c--------------------------------------------------------
int				ft_write_here(t_fd *fd, char **str, int i, int red);
int				ft_here(t_fd *fd, int red);
char			**ft_env_to_tab(t_env *env, char **tab);
//---------ft_pipex_utils.c-----------------------------------------------------
int				ft_cmd_path(t_info *info, t_big_token *b_tokens);
int				ft_command(t_info *info, t_big_token *b_tokens);
//---------ft_tools2.c----------------------------------------------------
void			ft_error_2(int i, t_info *info, t_big_token *b_tokens);
int				ft_error(int i, t_info *info, t_big_token *b_tokens);
void			ft_write(char *str);
int				ft_first_error(char *av1);

//-----------------ft_free.c----------------------------------------------------
void			ft_signal(int sig);
void			ft_cmd_signal(int sig);
void			ft_manage_sig(t_info *info, int a, int b);
//-----------------ft_free.c----------------------------------------------------
void			ft_free_all2(t_info *info, t_env *env);
void			ft_free_all(t_info *info, t_env *env);
void			ft_free_b_tokens(t_big_token *b_tokens);
void			ft_free_fd(t_fd *fd);
void			ft_free_cmd(t_big_token *b_tokens);
//-----------------ft_free2.c---------------------------------------------------
void			ft_free_wildcards(t_wildcards *wd);
void			ft_free_tokens(t_token *tokens);
void			ft_free_env(t_env *env);
int				ft_mal_err(t_info *info, t_env *env, char *err);

// WILDCARDS
//-----------------ft_wildcards_check.c-----------------------------------------
void			init_wc(int *i, char **pwd);
int				ft_check_wildcards(t_info *info, t_big_token *b_tokens, int i);
int				ft_add_wildcards(t_big_token *b_tokens);
int				ft_keep(char *str, char *dir, int *i, int j);
int				ft_dk_util(char **tmp, char *dir, int *i, int j);
//-----------------ft_wildcards_check_bis.c-------------------------------------
int				ft_do_keep(char *str, t_wildcards *wd, int type, int i);
int				ft_wd_nb_args(t_wildcards *wd, t_big_token *b, int i, int t);
//-----------------ft_wildcards_tools.c-----------------------------------------
void			ft_wdadd_back(t_wildcards **alst, t_wildcards *new);
int				ft_wildcardsnew(t_wildcards **wd, struct dirent *dir, DIR *fd);
int				ft_strlen_before_ast(char *str);
int				ft_manage_type(char *str, char *dir, int d_type, int type);
int				ft_get_wildcards(t_wildcards **wd, char *pwd);
//-----------------ft_wildcards_do.c--------------------------------------------
char			**ft_fill_old_args(t_big_token *b, char **tmp, int j, int list);
int				ft_do_wildcards(t_big_token *b_tokens, int i, char *pwd);
//-----------------ft_wildcards_do2.c-------------------------------------------
int				ft_create_begin(t_big_token *b, int count, char ***tmp, int i);
int				ft_end_realloc(t_big_token *b_toks, char **tmp);
int				ft_check_dir_wd(t_wildcards *wd, t_big_token *b, int *it);
int				ft_realloc_args(t_wildcards *w, t_big_token *b, int *it, int j);

// AGENT O
//----------main_O.c------------------------------------------------------------
int				main_agent_o(t_info *info);
char			*concat_argvs(int argc, char **argv);
//----------parser.c------------------------------------------------------------
void			give_parent(t_big_token **b_child, t_big_token **parent);
void			sub_parse_1(t_big_token **b, t_info *i, int b_start, int b_l);
void			sub_parse_2(t_big_token **b_child, t_big_token **tb, t_info *i);
int				parse(t_big_token **b_tokens, t_info *info, int btok_info[2]);
void			extract_fds(t_big_token **tmp_b, t_token **tokens);
//----------ft_executer.c-------------------------------------------------------
void			lead_ao(t_info *i, t_big_token **b, t_big_token *t, int f[3]);
void			ft_init_exec(t_big_token **b, t_big_token **t, int f[3], int a);
int				ft_first_part(t_info *info, t_big_token *tmp_b, int faoi[3]);
void			ft_second_part(t_big_token *tmp_b, int faoi[3]);
int				rec_exec(t_info *info, t_big_token **b_tokens, int and_or);
//----------ft_executer_tools.c-------------------------------------------------
int				no_sib_has_child(t_big_token *b_tokens);
void			give_parent_sc(t_big_token **child, t_big_token **parent);

// CREATE TOKENS
//----------tokenizer_1.c-------------------------------------------------------
unsigned int	get_real_tok_type(char c, t_token **tok_l, t_tok_type *t_t_t);
int				is_quoted(t_token **tok_list, char c);
void			init_tok_struct(t_token **tok_list, int rank_in_list);
//----------tokenizer_1_tool.c--------------------------------------------------
int				len_ll_list(t_token *tok_list);
t_token			*ft_create_token(t_tok_type tok_type, int length, int i);
t_token			*create_tok_bis(t_tok_type tok_type, int quoted, char *value);
//----------tokenizer_2.c-------------------------------------------------------
int				check_tok_type(t_tok_type tok_t, char c, t_info *i, int check);
int				checko(t_tok_type tok_type);
int				detect_tokens(t_info *info);
void			index_toks(t_token **tokens);
//----------tokenizer_3.c-------------------------------------------------------
int				add_tok_last(t_token **tok_l, t_tok_type t_t, int l, int i);
int				add_tok_last_bis(t_token **t_l, t_tok_type t_t, int q, char *v);
int				add_tok(t_token **tokens, int st, int len, t_tok_type type);
int				fill_tok_value(t_token **tok, char *str);
char			*ft_strncpy(char *str, int n);

// SYNTAX ERROR
//-----------syntax_errorinizer_1.c---------------------------------------------
int				count_op_tok(t_token **tokens);
int				count_cl_tok(t_token **tokens);
int				check_count_errors(t_token **tokens);
int				r_quotes_impair(t_token **tokens);
int				is_last_op(t_token **tokens);
//-----------syntax_errorinizer_2.c---------------------------------------------
int				ft_test_pipe(t_token *tmp, int i);
int				is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens);
int				is_there_par(t_big_token *b_tokens, t_token *tokens);
int				is_red_st_par(t_big_token *b_tokens, t_token *tokens);
//-----------syntax_errorinizer_3.c---------------------------------------------
int				r_2_op_succeding(t_token **tokens, int op_next, int op_prev);
int				ft_test_isdir(t_token *t, int i);
int				r_dir_op_succeding(t_token **tokens);
int				op_cl_par_succeeding(t_token **tokens);
int				syntax_err_handler(t_token **tokens);
//-----------syntax_errorinizer_3.c---------------------------------------------
int				par_not_afore_op(t_token **tokens);
//-----------big_tokenizer_1.c--------------------------------------------------
t_big_token		*ft_create_btoken(t_big_tok_type t, int its, int l, t_info *i);
int				add_b_tok_sib_last(t_big_token **b, t_big_tok_type t,
					int bi[2], t_info *i);
int				check_divider_type(char *tmp_value);
void			move_tok_2_ind(t_token **tokens, int ind);
//-----------big_tokenizer_2.c--------------------------------------------------
int				divide_by_or_and(t_big_token **b, t_info *i, int btok_info[2]);
//-----------big_tokenizer_5.c--------------------------------------------------
int				cl_par_ind(t_token **tokens, int ind_tok);
int				piped(t_info *info, int start, int length);
int				sophisticated_piped(t_token **tokens, int start, int length);
//-----------big_tokenizer_4-1.c------------------------------------------------
int				handle_par(t_big_token **b_tokens, t_info *info);
//-----------big_tokenizer_4_tool.c---------------------------------------------
int				check_if_piped(int ind, t_info *i, int l);
void			rd_inout_type(char *str, int *type_red);
int				divide_by_pipe(t_big_token **b_tokens, t_info *info);
void			init_divide(t_big_token *b, t_info *i, t_big_token **o,
					t_token **tt);
//-----------big_tokenizer_6.c--------------------------------------------------
int				is_pipe(t_token *tmp_s);
//-----------handle_par_dir.c---------------------------------------------------
int				handle_par_dir_0(t_token **t, t_big_token **b, t_info *info,
					int (*itctlt)[7]);
int				handle_par_dir_1(t_token **t, t_big_token **b, int (*itctlt)[7],
					int step);
int				handle_par_dir_2(t_token *tmp, int (*itctlt)[7], int step);
int				handle_par_dir(t_big_token **tmp_b, t_info *info);
//-----------handle_dir.c-------------------------------------------------------
void			handle_dir_0(t_token **t, t_big_token **b, t_info *in,
					int (*itscl)[6]);
int				handle_dir_1(t_token **t, int (*it)[6], int s, t_big_token **b);
int				handle_dir_2(t_token **t, int (*it)[6], int s, t_big_token **b);
int				handle_dir(t_big_token **tmp_b, t_info *info);
//-----------handle_par.c-------------------------------------------------------
void			handle_par_1(t_token **s, t_big_token *b, int *a_s, int *t_r);
void			handle_par_2(t_token **s, t_big_token *b, int *t_r, t_token *t);
int				handle_par_3(t_big_token **b, int t_r, int a_s, t_info *i);
void			handle_par_4(int (*p)[2], t_token **s, t_big_token *b,
					t_info *i);
int				handle_par_5(t_token **s, t_big_token **b, t_tab *t, t_info *i);
//-----------big_tokenizer_8.c--------------------------------------------------
void			init_params(int *adv_steps, int *to_reduce);
void			count_cmd_args(t_big_token **tmp_b, int i, t_token **t, int l);
//-----------big_tokenizer_9.c--------------------------------------------------
int				handle_piped(t_big_token **tmp_b, t_info *info);
//------------printer.c--------------------------------------------------------
void			ft_print_wildcards(t_wildcards *wd);
void			print_b_tokens(t_big_token **b, t_token **toks, int i, int j);
void			print_s_tokens(t_token **tokens, int start, int length);
int				depth_b_token(t_big_token **b_token);
void			print_all_everything(t_big_token **b_tokens, t_token **tokens);
void			print_all_child(t_big_token **b, t_token **toks, int i, int j);

// EXPAND
//-----------ft_expand_check.c--------------------------------------------------
int				ft_check_dol(char *str);
int				ft_check_expand(t_token *token, int start, int length);
int				ft_check_exp_line(char *str);
//-----------ft_expand_args_1.c----------------------------------------------
int				ra_sub(char **tmpj, char *cmd_arg);
int				ft_reduce_args(t_big_token *b, size_t i);
int				nqa_tool(t_big_token *b_tokens);
int				ft_noquote_args(t_big_token *b_tokens);
char			*ft_expanded_value(t_info *info, char *tmp);
//-----------ft_expand_args_2.c------------------------------------------------
int				ft_splitlen(char *str);
char			*ms_tool(int *a, char **str, int b, int type);
char			*ft_minisplit(char *str);
int				resplit_tool(int (*jl)[2], char **tmp, char *to_copy, int type);
void			resplit_tool_2(int (*jl)[2], int i, t_big_token *b, int step);
//-----------ft_expand_args_3.c-------------------------------------------------
int				resplit_tool_3(char ***tmp, int len1, int len2);
int				ft_resplit(t_big_token *b, int *i, char *str);
int				ft_expand_args(t_big_token *b, t_info *info);
//-----------ft_expand_line.c---------------------------------------------------
char			*ft_noquote_line(char *line);
int				ft_get_length(char *str, int length, int t);
char			*ft_expand_line(char *str, int *i, t_info *info, int t);
char			*ft_expand_l(char *str, t_info *info, int hd);
//-----------ft_expand_tools.c--------------------------------------------------
void			ft_count_q(char *str, char c, size_t *i, size_t *q);
size_t			ft_strlen_nq(char *str);
void			ft_type(char c, int *t);
int				ft_postype(char *str, int pos);
char			*ft_strndup(char *str, int len);
char			*strjoin_4(char *str1, char *str2);
//-----------ft_expand_1.c------------------------------------------------------
int				dol_expand(t_token **old_tokens, t_info *info, t_big_token *b);
int				expand(char **str, t_info *info);
char			*str_join_exp(t_token **tokens, int ind, int type);
//---------------dollar_expander.c----------------------------------------------
int				expand_1(char **str, int *i, t_info *info);
int				expanded_toks_check(t_token **tokens);
int				expanded_toks(t_token **old_tokens, int start, int length);
//---------------init_tok_type_tab.c--------------------------------------------
void			init_tok_type_tab_bis(t_tok_type tok_type_tab[128], int i);
void			init_tok_type_tab(t_tok_type (*tok_type_tab)[128]);

#endif
