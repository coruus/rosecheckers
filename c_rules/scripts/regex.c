#include "main.h"

int regex_match_extract(char* dest, char* matched_string, regmatch_t* match, size_t size) {
  char* match_start = matched_string + match->rm_so;
  char* match_end = matched_string + match->rm_eo;
  char* cur_dest = dest;
  char* cur_src = match_start;

  if (match->rm_eo - match->rm_so > size)
      return 0;

    while (cur_src < match_end)
    *(cur_dest++) = *(cur_src++);

  *cur_dest = 0;

    return 1;
}

void regex_compile(regex_t *compiled_regex, char const *regex) {
	if(regcomp(compiled_regex, regex, REG_EXTENDED) != 0) {
		error("Regex compilation failed");
	}
}

regmatch_t *regex_match(regex_t* compiled_regex, char* text, char **matches, int max_match_lengths[]) {
	int num_matches = 0;

	while (matches[num_matches] != NULL)
		num_matches++;

	regmatch_t* regmatches = calloc(sizeof(regmatch_t), num_matches+1);
	int regexec_success = regexec(compiled_regex, text, num_matches+1, regmatches, 0);

	if (regexec_success == 0) {
		int i;
		for(i = 0; i < num_matches; ++i) {
			regex_match_extract(matches[i], text, &regmatches[i+1], max_match_lengths[i]);
		}
		return regmatches;
	}
	else {
		free(regmatches);
		return NULL;
	}
}

int regex_check(char *buf, char const *regex) {
	regmatch_t *match_regs;
	regex_t ex;
	char match0[25];
	char match1[25];
	char match2[25];
	char match3[25];
	char *matches[] = { match0, match1, match2, match3, NULL };
	int lengths[] = { 25, 25, 25, 25 };

	regex_compile(&ex, regex);
	
	if((match_regs = regex_match(&ex, buf, matches, lengths))) {
		free(match_regs);
		return 0;
	}

	return 1;
}
