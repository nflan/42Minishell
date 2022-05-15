/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoudni <omoudni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 14:31:25 by omoudni           #+#    #+#             */
/*   Updated: 2022/05/15 21:29:31 by omoudni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

typedef enum s_tok
{
	TOK_IDK,
	TOK_SEP,
	TOK_QUOTER,
	TOK_EXPANDER,
	TOK_EXPANDER_OP,
	TOK_EXPANDER_CL,
	TOK_OPERATOR,
	TOK_PATH,
	TOK_WORD,
} t_tok;

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

static t_char_type get_char_class[255] =
	{
		['\0'] = CHR_NULL,
		['\t'] = CHR_SPACE,
		[' '] = CHR_SPACE,
		['!'] = CHR_EXCLAM,
		['"'] = CHR_D_QUOTE,
		['#'] = CHR_COMMENT,
		['\$'] = CHR_DOL,
		['%'] = CHR_PER,
		['&'] = CHR_AND,
		['\''] = CHR_S_QUOTE,
		['\('] = CHR_OP_PAREN,
		['\)'] = CHR_CL_PAREN,
		['\*'] = CHR_AST,
		['\+'] = CHR_PLUS,
		['-'] = CHR_DASH_MINES,
		['\.'] = CHR_POINT,
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
		['\['] = CHR_OP_BRACKET,
		['\\'] = CHR_ANTI_SLASH,
		['\]'] = CHR_CL_BRACKET,
		['\^'] = CHR_CIRCUM,
		['_'] = CHR_UNDERSCORE,
		['`'] = CHR_BACKTICK,
		['a' ... 'z'] = CHR_ALPHA,
		['\{'] = CHR_OP_BRACE,
		['\|'] = CHR_PIPE,
		['\}'] = CHR_CL_BRACE,
		['~'] = CHR_TILDA,
};

static t_tok get_tok_type[255] =
	{
		[CHR_NULL] = TOK_IDK,
		[CHR_SPACE] = TOK_SEP,
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
		[CHR_BACKTICK] = TOK_EXPANDER,
		[CHR_ALPHA] = TOK_WORD,
		[CHR_OP_BRACE] = TOK_EXPANDER_OP,
		[CHR_PIPE] = TOK_OPERATOR,
		[CHR_CL_BRACE] = TOK_EXPANDER_CL,
		[CHR_TILDA] = TOK_PATH,
};

#endif
