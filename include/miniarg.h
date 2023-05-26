#ifndef MINIARG_MINIARG_H_
#define MINIARG_MINIARG_H_

// TODO add values -> --long=VALUE, -s VALUE

#define MAX_OPTIONS 10

extern const char *miniarg_program_version;
extern const char *miniarg_program_bug_address;

struct miniarg_option {
	const char short_name;
	const char* long_name;
	bool is_mandatory;
	bool is_set;
	const char* description;
};

struct miniarg_state {
	int argc;
	char **argv;
	void *input;
};

struct miniarg {
	struct miniarg_option options;
	void (*parse_opt)(int key, char *arg, struct miniarg_state *state);
	const char *args_doc;
	const char *doc;
};

void miniarg_parse(struct miniarg *miniarg, int argc, char **argv, void *input);

#endif // MINIARG_MINIARG_H_
