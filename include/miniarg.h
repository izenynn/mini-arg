#ifndef MINIARG_MINIARG_H_
#define MINIARG_MINIARG_H_

#include <stdbool.h>

#ifndef MAX_OPTIONS
#define MAX_OPTIONS 50
#endif // MAX_OPTIONS

// Provided by user
extern const char *marg_program_version;
extern const char *marg_program_bug_address;

// Provided by library
extern const char *marg_program_name;
extern int marg_err_exit_status;

enum {
	MARG_GRP = 1
};

enum marg_option_flags {
	NONE = 0,
	OPTION_REQUIRED		= (1 << 0),
	OPTION_ARG		= (1 << 1),
	OPTION_ARG_REQUIRED	= (1 << 2),
	MARG_KEY_ARG = -1,
	MARG_KEY_END = -2
};

struct marg_option {
	const int key;
	const char *name;
	const char *arg;
	int flags;
	const char* description;
};

struct marg_state {
	const struct marg *const root_marg;
	int argc;
	char **argv;
	int next;
	int arg_num;
	void *input;
};

struct marg {
	struct marg_option *options;
	int (*parse_opt)(int key, const char *arg, struct marg_state *state);
	const char *args_doc;
	const char *doc;
};

void marg_parse(struct marg *marg, int argc, char **argv, void *input);

void marg_usage(struct marg_state *state);
void marg_error(struct marg_state *state, const char *fmt, ...);

#endif // MINIARG_MINIARG_H_
