/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:31:25 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/30 20:58:32 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>

typedef enum s_tok_type
{
	TOK_IDK,
	TOK_SEP,
	TOK_QUOTER,
	TOK_EXPANDER_OP,
	TOK_EXPANDER_CL,
	TOK_OPERATOR,
	TOK_PATH,
	TOK_REDIRECTOR_LEFT,
	TOK_REDIRECTOR_RIGHT,
	TOK_WORD,
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
	int 	quoted;
	struct s_token *prev;
	struct s_token *next;
} 			t_token;

typedef enum s_big_tok_type
{
	TOK_LEFT_OR,
	TOK_LEFT_AND,
	TOK_LEFT_PIPE,
	TOK_CLEAN,
	TOK_LAST,
	TOK_PIPE_LAST,
	// TOK_OR_LEFT,
	// TOK_AND_RIGHT,
	// TOK_PIPE_LEFT,
	// TOK_PIPE_RIGHT,
} 			t_big_tok_type;

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

typedef struct s_big_token
{
	t_big_tok_type	type;
	int				ind_tok_start;
	int				length;
	int				par;
	//	int		sp_after;
	// int		index;
	// int		priority;
	// t_tok_type tok_start;
	// t_tok_type tok_end;
	// t_par_left_right more_info;
	struct s_big_token *parent;
	struct s_big_token *child;
	struct s_big_token *sibling;
} 			t_big_token;

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
		[CHR_SPACE] = TOK_SEP,
		[CHR_EXCLAM] = TOK_IDK,
		[CHR_D_QUOTE] = TOK_QUOTER,
		[CHR_COMMENT] = TOK_IDK,
		[CHR_DOL] = TOK_EXPANDER,
		[CHR_PER] = TOK_IDK,
		[CHR_AND] = TOK_OPERATOR,
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
		[CHR_MINES] = TOK_REDIRECTOR_LEFT,
		[CHR_EQUAL] = TOK_OPERATOR,
		[CHR_SUPERIOR] = TOK_REDIRECTOR_RIGHT,
		[CHR_INTEROG] = TOK_OPERATOR,
		[CHR_AT] = TOK_WORD,
		[CHR_ALPHA] = TOK_WORD,
		[CHR_OP_BRACKET] = TOK_IDK,
		[CHR_ANTI_SLASH] = TOK_QUOTER,
		[CHR_CL_BRACKET] = TOK_IDK,
		[CHR_CIRCUM] = TOK_IDK,
		[CHR_UNDERSCORE] = TOK_WORD,
		[CHR_ALPHA] = TOK_WORD,
		[CHR_OP_BRACE] = TOK_IDK,
		[CHR_PIPE] = TOK_OPERATOR,
		[CHR_CL_BRACE] = TOK_IDK,
		[CHR_TILDA] = TOK_PATH,
		[CHR_BACKTICK] = TOK_IDK,
};

static int rules[9][255];

//----------tokenizer_1.c-------------------------------------------------------------------

int 			len_ll_list(t_token *tok_list);
int 			is_quoted(t_token **tok_list, int rank_in_list);
unsigned int	get_real_tok_type(char c, t_token **tok_list);
t_token 		*ft_create_token(t_tok_type tok_type, int length, int i);
void 			init_tok_struct(t_token **tok_list, int rank_in_list);

//----------tokenizer_2.c-------------------------------------------------------------------

void 			add_tok_last(t_token **tok_list, t_tok_type tok_type, int length, int i);
void 			detect_tokens(t_token **tok_list, char *str);
void 			fill_tok_value(t_token **tok, char *str);
char 			*ft_strncpy(char *str, int n);
void 			index_toks(t_token **tokens);

//-----------syntax_errorinizer_1.c----------------------------------------------------------------------

int 			count_op_tok(t_token **tokens);
int 			count_cl_tok(t_token **tokens);
int 			check_count_errors(t_token **tokens);
int 			r_quotes_impair(t_token **tokens);
int 			is_last_op(t_token **tokens);

//-----------syntax_errorinizer_2.c----------------------------------------------------------------------

int				r_2_op_succeding(t_token **tokens);
int				op_cl_par_succeeding(t_token **tokens);
int				syntax_err_handler(t_token **tokens);
int				is_pipe_in_st_end(t_big_token *b_tokens, t_token *tokens);

//-----------big_tokenizer_1.c---------------------------------------------------------------------------

t_big_token		*ft_create_btoken(t_big_tok_type type, int ind_tok_start, int length);
void			add_b_tok_sib_last(t_big_token **b_tok_list, t_big_tok_type type, int start, int length);
int				check_divider_type(char *tmp_value);
int				is_cl_2_op(char *value_tok_op, char *value_tok_cl);
void			move_tok_2_ind(t_token **tokens, int ind);

//-----------big_tokenizer_2.c---------------------------------------------------------------------------

int				cl_par_ind(t_token **tokens, t_tok_type tok, int ind_tok, char *value);
void			divide_by_or_and(t_big_token **b_tokens, t_token **tokens, int start, int length);
void parse(t_big_token **b_tokens, t_token **tokens, int start, int length);

//-----------big_tokenizer_4.c---------------------------------------------------------------------------

void			handle_par(t_big_token **b_tokens, t_token **tokens);

//-----------big_tokenizer_3.c---------------------------------------------------------------------------

// void		divide_by_or_and(t_big_token **b_tokens, t_token **tokens);
void		divide_by_pipe(t_big_token **b_tokens, t_token **tokens);


#endif
