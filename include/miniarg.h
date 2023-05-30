#ifndef MINIARG_MINIARG_H_
#define MINIARG_MINIARG_H_

#include <stdbool.h>

extern const char *miniarg_program_version;
extern const char *miniarg_program_bug_address;

enum miniarg_option_flags {
	NONE = 0,
	OPTION_REQUIRED		= (1 << 0),
	OPTION_ARG		= (1 << 1),
	OPTION_ARG_REQUIRED	= (1 << 2),
	MINIARG_ARG = -1,
	MINIARG_END = -2
};

struct miniarg_option {
	const int key;
	const char *name;
	const char *arg;
	int flags;
	bool is_set;
	const char* description;
};

struct miniarg_state {
	int argc;
	int arg_num; // keep track of number of normal arguments
	char **argv;
	int next;
	void *input;
};

struct miniarg {
	struct miniarg_option *options;
	int (*parse_opt)(int key, const char *arg, struct miniarg_state *state);
	const char *args_doc;
	const char *doc;
};

void miniarg_parse(struct miniarg *miniarg, int argc, char **argv, void *input);

#endif // MINIARG_MINIARG_H_
