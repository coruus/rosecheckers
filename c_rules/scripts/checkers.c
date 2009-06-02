#include "main.h"

#define TYPE "char|short|size_t|int|long|long long|float|double"
#define OPERATOR "\\+|/|-"
#define CONDITIONAL "if|else|while|switch|do"
#define CONDBEGIN "\\(|\\{"
#define RULET "[[:alnum:]][[:alnum:]][[:alnum:]][0-9][0-9]-[[:alnum:]]\\.?"
#define SPACE "[[:space:]]*"
#define STRING "([[:print:]]*)"
#define RISK_FIELD SPACE""STRING""SPACE
#define RED "red"
#define GREEN "green"
#define YELLOW "#cc9900"

int generic_seek_and_destroy(char *buf, char const *violation_regex, char const *exception_regex, char *matches[], int lengths[], char *fmt, int len, int *vec, int num, char const *match_msg);

int check_ellipses(char *buf, size_t len) {
#if CURRENT_LANG == LANG_CPP
	char const *correct_ellipses = "\\/\\/|\\([[:alnum:]]*\\.\\.\\.";
	char *replacement = "// %";
#else
	char const *correct_ellipses = "\\/\\*|\\([[:alnum:]]*\\.\\.\\.[[:alnum:]]*\\)";
	char *replacement = "/* % */";
#endif
	char *ellipses_regex = "(\\.\\.\\.)";
	
	char match[10];
	char *matches[] = { match, NULL };
	int lengths[] = { 10 };
	int vec[] = {0};
	
	return generic_seek_and_destroy(buf, ellipses_regex, correct_ellipses, matches, lengths, replacement, len, vec, 1, "Uncommented ellipsis found");
}

int check_leftmost_const(char *buf, size_t len) {
	char const *leftmost_const_regex = "("TYPE")[[:space:]]+(const)";
	
	char match1[8];
	char match2[8];
	char *matches[] = { match1, match2, NULL };
	int lengths[] = { 8, 8 };
	int vec[] = {1, 0};
	
	return generic_seek_and_destroy(buf, leftmost_const_regex, NULL, matches, lengths, "% %", len, vec, 2, "Non-leftmost const found");
}

int check_references(char *buf, size_t len) {
	char const *references_regex = "\\* ([[:print:]]*\\])([[:print:]]*)";

	char match1[512];
	char match2[512];
	char *matches[] = { match1, match2, NULL };
	int lengths[] = { 512, 512 };
	int vec[] = { 0, 1 };

	return generic_seek_and_destroy(buf, references_regex, NULL, matches, lengths, "\\[%\\]%", len, vec, 2, "Bulleted reference section found");
}

int check_asterisk_placement(char *buf, size_t len) {
	char const *check_asterisk_regex = "("TYPE"|const)\\* ";

	char match[10];
	char *matches[] = { match, NULL };
	int lengths[] = { 10 };
	int vec[] = {0};

	return generic_seek_and_destroy(buf, check_asterisk_regex, NULL, matches, lengths, "% *", len, vec, 1, "Incorrect placement of asterisk in point declaration found");
}

int check_conditional_whitespace(char *buf, size_t len) {
	char const *check_conditional_regex = "("CONDITIONAL")("CONDBEGIN")";

	char match1[10];
	char match2[10];
	char *matches[] = { match1, match2, NULL };
	int lengths[] = { 10, 10 };
	int vec[] = {0, 1};
//return -1;
	return generic_seek_and_destroy(buf, check_conditional_regex, NULL, matches, lengths, "% %", len, vec, 2, "No whitespace found betweem conditional and starting character");
}

int check_commas(char *buf, size_t len) {

	char const *check_comma_regex = "\\,([[:alnum:]])";
	
	char match[10];
	char *matches[] = { match, NULL };
	int lengths[] = { 10 };
	int vec[] = {0};
	
	return generic_seek_and_destroy(buf, check_comma_regex, NULL, matches, lengths, ", %", len, vec, 1, "Space missing between commas and parameters");
}

int check_link_consistency(char *buf, size_t len) {
//	char const *regex = "\\[[:alpha:][:alpha:][:alpha:][:digit:][:digit:]-[AC]\\.[:space:]*\\|[:space:]*(.*)\\]";
	char const *regex = "\\[("RULET")[a-zA-Z0-9 \\.\\-]*\\|[[:space:]]*([[:print:]]+)\\]";
	char match[10];
	char match2[1024];
	char *matches[] = { match, match2, NULL };
	int lengths[] = { 10, 1024 };
	int vec[] = {1};
	
	return generic_seek_and_destroy(buf, regex, NULL, matches, lengths, "[%]", len, vec, 1, "Inconsistent link found");
}

enum { RISK_FIELD_WIDTH = 50 };
enum { RULEF, SEVERITY, LIKELIHOOD, REMEDIATION, PRIORITY, LEVEL, RISK_FIELD_SIZE };

static int check_test(char *field, char **check){

	char const *generic = "[^[:digit:]]*([[:digit:]])[^[:digit:]]*[[:space:]]*[^[:digit:]]*\\(([[:print:]]+*)\\)";

	regex_t reg;
	regmatch_t *match_regs;
	char gmatch[15];
	char gmatch1[15];
	char spaces[15];
	char *matches[] = { gmatch, gmatch1, spaces, NULL };
	int lengths[] = { 15, 15, 15};
	unsigned long i = 0;
	regex_compile(&reg, generic);
	if((match_regs = regex_match(&reg, field, matches, lengths))) {
		i = Strtol(matches[0], NULL, 0);
		if(i <= 0 || i > 3) {
			logger("\tInvalid range for risk table field detected");
			i = 3;
		}
		if(!strcmp(matches[1], check[i-1]) == 0) {
			logger("\t\tInconsistent risk field naming detected, preferring number");
			strcpy(matches[1], check[i-1]);
		}
		sprintf(field, "*%i* (%s) ", (int)i, matches[1]);
		
	}

	return i;
}

int check_color_section(int product, char *priority, char *level) {
	int color_index;
	char *level_color[] = { RED, YELLOW, GREEN };

	if(product <= 4) {
		color_index = 2;
	}
	else if(product > 4 && product <= 9) {
		color_index = 1;
	}
	else {
		color_index = 0;
	}
	
	sprintf(priority, "{color:%s}{*}P%d{*}{color}", level_color[color_index], product);
	sprintf(level, "{color:%s}{*}L%d{*}{color}", level_color[color_index], color_index+1);
	return 0;
}

static int check_fields(char **matched) {
	char *severity_check[] = { "low", "medium", "high" };
	char *likelihood_check[] = { "unlikely", "probable", "likely" };
	char *remediation_check[] = { "high", "medium", "low" };
	int toreturn = -1;
	int sev;
	int lik;
	int rem;
	//regmatch_t *match_regs;

	sev = check_test(matched[1], severity_check);
	lik = check_test(matched[2], likelihood_check);
	rem = check_test(matched[3], remediation_check);
	
	check_color_section(sev*lik*rem, matched[4], matched[5]);
	return toreturn;
}

int check_risk_table(char *buf, size_t len, char *rulename, int *is) {
	char const *regex = "\\|("SPACE""RULET""SPACE")\\|"RISK_FIELD"\\|"RISK_FIELD"\\|"RISK_FIELD"\\|"RISK_FIELD"\\|"RISK_FIELD"\\|";
	int i;
	regex_t reg;
	regmatch_t *match_regs;

	char ru[RISK_FIELD_WIDTH];
	char se[RISK_FIELD_WIDTH];
	char li[RISK_FIELD_WIDTH];
	char re[RISK_FIELD_WIDTH];
	char pr[RISK_FIELD_WIDTH];
	char le[RISK_FIELD_WIDTH];
	char *matches[] = {ru, se, li, re, pr, le, NULL };
	int lengths[RISK_FIELD_SIZE];
	int vec[] = { 0, 1, 2, 3, 4, 5 };

	for(i = 0; i < RISK_FIELD_SIZE; ++i) {
		lengths[i] = RISK_FIELD_WIDTH;
	}

	regex_compile(&reg, regex);
	if((match_regs = regex_match(&reg, buf, matches, lengths))) {
		char dest[len];
		logger("Check risk table matched");
		if(is[A_RULE]) {
			if(strncmp(matches[0]+1, rulename, 8) != 0) {
				logger("\tReplaced rule name in risk table");
				sprintf(matches[0], " %s ", rulename);
			}
		}
		if(check_fields(matches) != 0) {
			replace(dest, buf, "|%| %| %| %| % | % |", &match_regs, matches, len, 6, vec);
			logger("\tReplaced risk table entry");
			snprintf(buf, len, "%s", dest);
			free(match_regs);
		}
	}

	return -1;
}

int generic_seek_and_destroy(char *buf, char const *violation_regex, char const *exception_regex, char *matches[], int lengths[], char *fmt, int len, int *vec, int num, char const *match_msg) {

	regex_t reg;
	regmatch_t *match_regs;

	if(exception_regex != NULL) {
		if(regex_check(buf, exception_regex) == 0)
			return -1;
	}
	
	regex_compile(&reg, violation_regex);
	if((match_regs = regex_match(&reg, buf, matches, lengths))) {
		char dest[len];
		logger(match_msg);
		replace(dest, buf, fmt, &match_regs, matches, len, num, vec);
		logger("\tReplacement occurred");
		snprintf(buf, len, "%s", dest);
		free(match_regs);
	}
	else
		return -1;

	return 0;
}
