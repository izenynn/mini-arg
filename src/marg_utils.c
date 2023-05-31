#include <stdio.h>
#include <stdlib.h>
#include "miniarg.h"

size_t marg_strlen(const char *s)
{
	const char *p;

	for (p = s; *p != '\0'; ++p);
	return p - s;
}

int marg_strncmp(const char *s1, const char *s2, size_t n)
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

char *marg_strchr(const char *s, int c)
{
	do {
		if (*s == c)
			return (char*)s;
	} while (*s++);
	return 0;
}

static inline void print_opt(const struct marg_option *opt)
{
	int len;

	if(opt->flags & OPTION_ARG || opt->flags & OPTION_ARG_REQUIRED) {
		len = marg_strlen(opt->name);
		printf("  -%c, --%s%-*s %s\n", opt->key, opt->name, 20 - len, "=VALUE", opt->description);
	} else {
		printf("  -%c, --%-20s %s\n", opt->key, opt->name, opt->description);
	}
}

static void print_options(const struct marg *const marg, const struct marg_option *opt)
{
	printf("%s\n\n", marg->doc);
	if (opt == NULL) {
		for (opt = marg->options; opt->key != 0; ++opt) {
			print_opt(opt);
		}
	} else {
		print_opt(opt);
	}
	printf("\nMandatory or optional arguments to long options are also mandatory or optional\nfor any corresponding short options.\n");
	printf("\nReport bugs to %s.\n", marg_program_bug_address);
}

void marg_usage_str(const struct marg *const marg)
{
	printf("Usage: [OPTION...] %s\n", marg->args_doc);
}

void marg_help(const struct marg *const marg)
{
	marg_usage_str(marg);
	print_options(marg, NULL);
}

/*
 * Unused function
 *
void static_marg_error(const struct marg *const marg, const struct marg_option *opt, const struct marg_state *const state, const char* error_msg)
{
	if (error_msg != NULL)
		fprintf(stderr, "%s: %s\n", state->argv[0], error_msg);
	static_marg_usage(marg);
	print_options(marg, opt);
	exit(marg_err_exit_status);
}
*/
