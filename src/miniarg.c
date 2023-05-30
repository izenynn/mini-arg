#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "miniarg.h"

size_t ma_strlen(const char *s);
int ma_strncmp(const char *s1, const char *s2, size_t n);
void print_option(const struct miniarg_option *opt);
void print_usage(const struct miniarg *miniarg, const struct miniarg_option *opt);
void error_exit(const struct miniarg *miniarg, const struct miniarg_option *opt, const char* error_msg);

void handle_long_option(const struct miniarg* miniarg, const char* arg, struct miniarg_state* state) {
	struct miniarg_option *opt;
	size_t len = ma_strlen(arg + 2) + 1;

	for(opt = miniarg->options; opt != NULL; ++opt) {
		if(ma_strncmp(opt->name, arg + 2, len) == 0) {
			if((opt->flags & OPTION_ARG_REQUIRED || opt->flags & OPTION_ARG) && state->next < state->argc && state->argv[state->next][0] != '-') {
				opt->arg = state->argv[++state->next];
			} else if(opt->flags & OPTION_ARG_REQUIRED) {
				error_exit(miniarg, opt, "Error: Missing value for argument");
			} else {
				opt->arg = NULL;
			}
			opt->is_set = true;
			if (miniarg->parse_opt(opt->key, opt->arg, state))
				error_exit(miniarg, opt, "Error: Parser failed for option");
			return;
		}
	}
	if (opt == NULL)
		error_exit(miniarg, NULL, "Error: Unknown long option");
}

void handle_short_option(const struct miniarg* miniarg, const char* arg, struct miniarg_state* state) {
	struct miniarg_option *opt;

	for (const char *p = arg + 1; *p != '\0'; ++p) {
		for(opt = miniarg->options; opt != NULL; ++opt) {
			if(opt->key == *p) {
				if((opt->flags & OPTION_ARG_REQUIRED || opt->flags & OPTION_ARG) && *(p + 1) == '\0' && state->next < state->argc && state->argv[state->next][0] != '-') {
					opt->arg = state->argv[++state->next];
				} else if(opt->flags & OPTION_ARG_REQUIRED) {
					error_exit(miniarg, opt, "Error: Unexpected argument or missing value");
				} else {
					opt->arg = NULL;
				}
				opt->is_set = true;
				if (miniarg->parse_opt(opt->key, opt->arg, state))
					error_exit(miniarg, opt, "Error: Parser failed for option");
				break;
			}
		}
		if (opt == NULL)
			error_exit(miniarg, NULL, "Error: Unknown short option");
	}
}

void miniarg_parse(struct miniarg* miniarg, int argc, char** argv, void* input)
{
	struct miniarg_state state = {
		.argc = argc,
		.argv = argv,
		.next = 1,
		.arg_num = 0,
		.input = input
	};

	// Handle options
	for(; state.next < argc; ++state.next) {
		if(ma_strncmp(argv[state.next], "--", 3) == 0) {
			state.next++;
			break;
		}
		
		if(ma_strncmp(argv[state.next], "-h", 3) == 0 || ma_strncmp(argv[state.next], "--help", 7) == 0) {
			print_usage(miniarg, NULL);
			exit(0);
		} else if(ma_strncmp(argv[state.next], "-V", 3) == 0 || ma_strncmp(argv[state.next], "--version", 10) == 0) {
			printf("%s\n", miniarg_program_version);
			exit(0);
		} else if(argv[state.next][0] == '-') {
			if(argv[state.next][1] == '-') {
				handle_long_option(miniarg, argv[state.next], &state);
			} else {
				handle_short_option(miniarg, argv[state.next], &state);
			}
		} else {
			if (miniarg->parse_opt(MINIARG_ARG, argv[state.next], &state))
				error_exit(miniarg, NULL, "Error: Parser failed for argument");
			state.arg_num++;
		}
	}
	
	// Handle normal arguments after "--"
	for(; state.next < argc; ++state.next) {
		if (miniarg->parse_opt(MINIARG_ARG, argv[state.next], &state))
			error_exit(miniarg, NULL, "Error: Parser failed for argument");
		state.arg_num++;
	}

	// Check if required argumentss are set
	for(struct miniarg_option *opt = miniarg->options; opt != NULL; ++opt) {
		if((opt->flags & OPTION_REQUIRED) && !opt->is_set) {
			error_exit(miniarg, opt, "Error: Missing required argument");
		}
	}

	// Signal the end of parsing
	miniarg->parse_opt(MINIARG_END, NULL, &state);
		error_exit(miniarg, NULL, "Error: Something went wrong parsing arguments");
}
