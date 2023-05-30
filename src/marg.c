#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <stdarg.h>
#include "miniarg.h"

void static_marg_usage(const struct marg *const marg);
void static_marg_help(const struct marg *const marg);
void static_marg_error(const struct marg *const marg, const struct marg_option *opt, const struct marg_state *const state, const char* error_msg);

int marg_err_exit_status = EX_USAGE;

static size_t marg_strlen(const char *s)
{
	const char *p;

	for (p = s; *p == '\0'; ++p);
	return p - s;
}

static int marg_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n)
		i++;
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	else
		return 0;
}

static char *marg_strchr(const char *s, int c)
{
	do {
		if (*s == c)
			return (char*)s;
	} while (*s++);
	return 0;
}

static void handle_long_option(const struct marg* marg, const char* arg, struct marg_state* state) {
	struct marg_option *opt;
	const char *equal_pos = marg_strchr(arg, '=');
	size_t len = equal_pos ? (size_t)(equal_pos - arg - 2) : marg_strlen(arg + 2);

	for(opt = marg->options; opt->key != 0; ++opt) {
		if(marg_strncmp(opt->name, arg + 2, len) == 0 && (opt->name[len] == '\0' || opt->name[len] == '=')) {
			if(equal_pos) {
				if (opt->flags & OPTION_ARG || opt->flags & OPTION_ARG_REQUIRED)
					opt->arg = equal_pos + 1;
				else
					static_marg_error(marg, opt, state, "unexpected value for argument");
			} else {
				if ((opt->flags & OPTION_ARG_REQUIRED || opt->flags & OPTION_ARG) && state->next < state->argc && state->argv[state->next][0] != '-')
					opt->arg = state->argv[++state->next];
				else if (opt->flags & OPTION_ARG_REQUIRED)
					static_marg_error(marg, opt, state, "missing value for argument");
				else
					opt->arg = NULL;
			}
			opt->is_set = true;
			if (marg->parse_opt(opt->key, opt->arg, state))
				static_marg_error(marg, opt, state, "parser failed for option");
			return;
		}
	}
	if (opt->key == 0)
		marg_error(state, "unknown long option '%s'", arg);
}

static void handle_short_option(const struct marg* marg, const char* arg, struct marg_state* state) {
	struct marg_option *opt;

	for (const char *p = arg + 1; *p != '\0'; ++p) {
		for(opt = marg->options; opt->key != 0; ++opt) {
			if(opt->key == *p) {
				if((opt->flags & OPTION_ARG_REQUIRED || opt->flags & OPTION_ARG)) {
					if (*(p + 1) != '\0') {
						opt->arg = p + 1;
						while(*p != '\0') ++p;
					} else if(state->next < state->argc
							&& state->argv[state->next][0] != '-') {
						opt->arg = state->argv[++state->next];
					} else if(opt->flags & OPTION_ARG_REQUIRED) {
						static_marg_error(marg, opt, state, "unexpected argument or missing value");
					} else {
						opt->arg = NULL;
					}
				} else {
					opt->arg = NULL;
				}
				opt->is_set = true;
				if (marg->parse_opt(opt->key, opt->arg, state))
					static_marg_error(marg, opt, state, "parser failed for option");
				break;
			}
		}
		if (opt->key == 0)
			marg_error(state, "unknown short option '-%c'", *p);
	}
}

void marg_parse(struct marg* marg, int argc, char** argv, void* input)
{
	struct marg_state state = {
		.root_marg = marg,
		.argc = argc,
		.argv = argv,
		.next = 1,
		.arg_num = 0,
		.input = input
	};

	// Handle options
	for(; state.next < argc; ++state.next) {
		if(marg_strncmp(argv[state.next], "--", 3) == 0) {
			state.next++;
			break;
		}
		
		if(marg_strncmp(argv[state.next], "-h", 3) == 0 || marg_strncmp(argv[state.next], "--help", 7) == 0) {
			static_marg_help(marg);
			exit(EX_OK);
		} else if(marg_strncmp(argv[state.next], "-V", 3) == 0 || marg_strncmp(argv[state.next], "--version", 10) == 0) {
			printf("%s\n", marg_program_version);
			exit(EX_OK);
		} else if(argv[state.next][0] == '-') {
			if(argv[state.next][1] == '-')
				handle_long_option(marg, argv[state.next], &state);
			else
				handle_short_option(marg, argv[state.next], &state);
		} else {
			if (marg->parse_opt(MARG_KEY_ARG, argv[state.next], &state))
				marg_error(&state, "parser failed for argument");
			state.arg_num++;
		}
	}
	
	// Handle normal arguments after "--"
	for(; state.next < argc; ++state.next) {
		if (marg->parse_opt(MARG_KEY_ARG, argv[state.next], &state))
			marg_error(&state, "parser failed for argument");
		state.arg_num++;
	}

	// Check if required options are set
	for(struct marg_option *opt = marg->options; opt->key != 0; ++opt) {
		if((opt->flags & OPTION_REQUIRED) && !opt->is_set)
			marg_error(&state, "missing required argument");
	}

	// Signal the end of parsing
	if (marg->parse_opt(MARG_KEY_END, NULL, &state))
		marg_error(&state, "something went wrong parsing arguments");
}

void marg_usage(struct marg_state *state)
{
	static_marg_usage(state->root_marg);
	exit(marg_err_exit_status);
}

void marg_error(struct marg_state *state, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "%s: ", state->argv[0]);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\nTry '%s --help' for more information.\n",
		state->argv[0]);
	va_end(args);
	exit(marg_err_exit_status);
}
