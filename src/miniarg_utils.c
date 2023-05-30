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

void print_option(const struct miniarg_option *opt)
{
	printf("    -%c, --%-20s %s\n", opt->key, opt->name, opt->description);
}

void print_usage(const struct miniarg *miniarg, const struct miniarg_option *opt)
{
	printf("Usage: [OPTIONS...] %s\n", miniarg->args_doc);
	if (opt == NULL) {
		for (opt = miniarg->options; opt->name != NULL; ++opt) {
			print_option(opt);
		}
	} else {
		print_option(opt);
	}
}

void error_exit(const struct miniarg *miniarg, const struct miniarg_option *opt, const char* error_msg)
{
	if (error_msg != NULL)
		fprintf(stderr, "%s\n", error_msg);
	print_usage(miniarg, opt);
	exit(1);
}
