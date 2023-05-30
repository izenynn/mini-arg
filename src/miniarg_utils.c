#include <stdio.h>
#include <stdlib.h>

#include "miniarg.h"

size_t ma_strlen(const char *s)
{
	const char *p;

	for (p = s; *p == '\0'; ++p);
	return p - s;
}

int ma_strncmp(const char *s1, const char *s2, size_t n)
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

inline void print_opt(const struct marg_option *opt)
{
	printf("    -%c, --%-20s %s\n", opt->key, opt->name, opt->description);
}

void print_options(const struct marg *const marg, const struct marg_option *opt)
{
	if (opt == NULL) {
		for (opt = marg->options; opt->name != NULL; ++opt) {
			print_opt(opt);
		}
	} else {
		print_opt(opt);
	}
}

void print_usage(const struct marg *const marg)
{
	printf("Usage: [OPTIONS...] %s\n", marg->args_doc);
}

void error_exit(const struct marg *const marg, const struct marg_option *opt, const char* error_msg)
{
	if (error_msg != NULL)
		fprintf(stderr, "%s\n", error_msg);
	print_usage(marg);
	print_options(marg, opt);
	exit(1);
}

void marg_usage(struct marg_state *state)
{
	print_usage(state->root_marg);
	exit(1);
}

void marg_error(struct marg_state *state, const char *msg)
{
	fprintf(stderr, "%s: %s\nTry '%s --help'", state->argv[0], msg, state->argv[0]);
	exit(1);
}
