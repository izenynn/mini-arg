#include <stdio.h>
#include <stdlib.h>
#include "miniarg.h"

static inline void print_opt(const struct marg_option *opt)
{
	printf("    -%c, --%-20s %s\n", opt->key, opt->name, opt->description);
}

static void print_options(const struct marg *const marg, const struct marg_option *opt)
{
	if (opt == NULL) {
		for (opt = marg->options; opt->key != 0; ++opt) {
			print_opt(opt);
		}
	} else {
		print_opt(opt);
	}
}

void static_marg_usage(const struct marg *const marg)
{
	printf("Usage: [OPTIONS...] %s\n", marg->args_doc);
}

void static_marg_help(const struct marg *const marg)
{
	static_marg_usage(marg);
	print_options(marg, NULL);
}

void static_marg_error(const struct marg *const marg, const struct marg_option *opt, const char* error_msg)
{
	if (error_msg != NULL)
		fprintf(stderr, "%s\n", error_msg);
	static_marg_usage(marg);
	print_options(marg, opt);
	exit(1);
}
