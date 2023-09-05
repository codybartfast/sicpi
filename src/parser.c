#include "obarray.h"
#include "sicp-error.h"
#include "parser.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static object parse_token(parser parser, token tkn);

static char *const dot_err_msg =
	"Dot '.' can only be used for dotted-tail definitions";

void parser_init(parser parser, token_source token_source)
{
	parser->token_source = token_source;
	parser->is_errored = false;
	parser->error_message = NULL;
}

inline bool parser_is_errored(parser parser)
{
	return parser->is_errored;
}

inline char *parser_error_message(parser parser)
{
	return parser->error_message;
}

static object lexer_error(parser parser)
{
	parser->is_errored = true;
	parser->error_message = "Lexer error.";
	return from_error_kind(ERROR_LEXER, NO_META_DATA);
}

static object parser_error(parser parser, char *const error_message)
{
	parser->is_errored = true;
	parser->error_message = error_message;
	return from_error_kind(ERROR_PARSER, NO_META_DATA);
}

static object number_integer(parser parser, token tkn)
{
	long long conv_int;
	char *end = NULL;
	char *s = token_text(tkn);
	errno = 0;
	conv_int = strtoll(s, &end, 10);
	if (end && !*end && errno != ERANGE && integer_min <= conv_int &&
	    conv_int <= integer_max) {
		return from_integer(conv_int, NO_META_DATA);
	}
	return parser_error(
		parser,
		"Failed to convert string to integer (digit overflow?).");
}

static object number_decimal(parser parser, token tkn)
{
	long double conv_flt;
	char *end = NULL;
	errno = 0;
	char *s = token_text(tkn);
	conv_flt = strtold(s, &end);
	if (end && !*end && errno != ERANGE && floating_min <= conv_flt &&
	    conv_flt <= floating_max) {
		return from_floating(conv_flt, NO_META_DATA);
	}
	return parser_error(parser,
			    "Failed to convert string to decimal (overflow?).");
}

static object reverse(object list)
{
	object rlist = Empty_List;
	while (list != Empty_List) {
		rlist = cons(car(list), rlist, object_meta_data(list));
		object pair = list;
		list = cdr(list);
		object_free(pair);
	}
	return rlist;
}

inline static object dot(parser parser, object lst)
{
	if (is_null(lst)) {
		return parser_error(parser, dot_err_msg);
	}
	object tail = parse(parser);
	if (!is_symbol(tail)) {
		return parser_error(parser, dot_err_msg);
	}
	if (token_type(token_read(parser->token_source)) != TOKEN_LIST_CLOSE) {
		return parser_error(parser, dot_err_msg);
	}
	lst = cons(tail, cons(Dot, lst, NO_META_DATA), NO_META_DATA);
	return reverse(lst);
}

static object list(parser parser)
{
	object lst = Empty_List;
	token tkn;
	object obj;
	enum token_type tkn_typ;
	while ((tkn_typ = token_type(
			(tkn = token_read(parser->token_source)))) > 0) {
		if (tkn_typ == TOKEN_LIST_CLOSE) {
			return reverse(lst);
		}
		if (tkn_typ == TOKEN_DOT) {
			return dot(parser, lst);
		}
		obj = parse_token(parser, tkn);
		if (is_error(obj)) {
			return obj;
		}
		lst = cons(obj, lst, NO_META_DATA);
	}
	if (token_type(tkn) == TOKEN_EOS) {
		return parser_error(parser,
				    "List was not closed before end of file.");
	}
	return lexer_error(parser);
}

static object abreviation(parser parser, char *name)
{
	object obj = parse(parser);
	if (is_error(obj) || obj == Eos) {
		return obj;
	}
	return cons(from_name(name, NO_META_DATA), obj, NO_META_DATA);
}

static object parse_token(parser parser, token tkn)
{
	switch (token_type(tkn)) {
	case TOKEN_IDENTIFIER:
		return from_name(token_text(tkn), NO_META_DATA);
	case TOKEN_NUMBER_INTEGER:
		return number_integer(parser, tkn);
	case TOKEN_NUMBER_DECIMAL:
		return number_decimal(parser, tkn);
	case TOKEN_STRING:
		return from_string(strdupx(token_text(tkn),
					   "parser:TOKEN_STRING"),
				   NO_META_DATA);
	case TOKEN_LIST_OPEN:
		return list(parser);
	case TOKEN_LIST_CLOSE:
		return parser_error(parser, "List close ')' was not expected.");
	case TOKEN_QUASIQUOTE:
		return abreviation(parser, "quasiquote");
	case TOKEN_QUOTE:
		return abreviation(parser, "quote");
	case TOKEN_UNQUOTE:
		return abreviation(parser, "unquote");
	case TOKEN_DOT:
		return parser_error(parser, dot_err_msg);
	case TOKEN_EOS:
		return Eos;
	case TOKEN_ERROR:
		return lexer_error(parser);
	case TOKEN_UNSPECIFIED:
	default:
		inyim("Parser given unexpected token type: %d",
		      token_type(tkn));
		exit(1); // keep compiler quiet
	}
}

object parse(parser parser)
{
	if (parser_is_errored(parser)) {
		return parser_error(
			parser, "Attempted to parse after an earlier error.");
	}

	token tkn = token_read(parser->token_source);
	return parse_token(parser, tkn);
}

object parse_all(parser parser)
{
	object rslt = Empty_List;
	object obj;
	while ((obj = parse(parser)) != Eos) {
		if (is_error(obj)) {
			return obj;
		}
		rslt = cons(obj, rslt, NO_META_DATA);
	}
	return reverse(rslt);
}
