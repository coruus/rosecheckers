#include "main.h"

char const *in_filename = "content.txt";
char const *success_filename = "success.txt";

char filename[FILENAME_LEN];
char rulename[8];

#define CHECK(arg) while(check_##arg(buf, size) == 0)

int main(int argc, char *argv[]) {
	FILE *infile = NULL;
	FILE *success_file = NULL;
	char buf[BUF_SIZE];
	int is[IS_ARR_SIZE];
	int i;
	
	/* Program doesn't accept arguments */
	if(argc != 1) {
		fprintf(stderr, "No parameters allowed\n");
		exit(-1);
	}

	/* Open the input file in read-binary mode */
	infile = fopen(in_filename, "rb");
	if(infile == 0) {
		error("Failed to open input file");
	}

	success_file = fopen(success_filename, "w");
	if(success_file == NULL) {
		error("Failed to open sucess reporting file");
	}
	/* Flag that at the moment the conversion has been a failure */
	fprintf(success_file, "O_o");
	fflush(success_file);

	/* Set the current states to false */
	for(i = 0; i < IS_ARR_SIZE; ++i) {
		is[i] = 0;
	}

	/* First grab the filename from the top of the file*/
	if(get_next_line(filename, FILENAME_LEN, &infile) == 0) {
		snprintf(rulename, 8, "%s", filename);
		if(regex_check(filename, RULE) == 0) {
			is[A_RULE] = 1;
		}
		/* Then loop through processing line by line */
		while(get_next_line(buf, BUF_SIZE, &infile) == 0) {
			inc_line();
			process_line(buf, BUF_SIZE, is);
		}
	}

	/* Now that everything is finished, we can flag it as a success */
	rewind(success_file);
	fprintf(success_file, "^______________^");
	fflush(success_file);

	return 0;
}

/*
 * This function updates a set of variables that defined what portion of the
 * wiki code we are in, e.g., in a risk assessment, in the reference section,
 * etc.
 */
void ascertain_environment(char *buf, int *is) {
	char const *risk_assessment = "h2\\. Risk Assessment";
	char const *risk_assessment_negate = "h2\\.";
	char const *reference = "h2\\. Reference";
	char const *reference_negate = "h2\\.";
	char const *risk_table = "\\|\\| Severity \\|\\| Lik";
	char const *risk_table_negate = "\\|";
	char const *function_dec = "\\(";

	char const *code = "\\{code";
	char const *cs = "ccccff";
	char const *ncce = "ffcccc";

	if(is[IN_RISK_ASSESSMENT]) {
		if(regex_check(buf, risk_assessment_negate) == 0)
			is[IN_RISK_ASSESSMENT] = 0;
	}
	else {
		if(regex_check(buf, risk_assessment) == 0)
			is[IN_RISK_ASSESSMENT] = 1;
	}

	if(is[IN_REFERENCE]) {
		if(regex_check(buf, reference_negate) == 0)
			is[IN_REFERENCE] = 0;
	}
	else {
		if(regex_check(buf, reference) == 0)
			is[IN_REFERENCE] = 1;
	}


	if(is[IN_RISK_TABLE]) {
		if(regex_check(buf, risk_table_negate) != 0) {
			is[IN_RISK_TABLE] = 0;
		}
	}
	else {
		if(regex_check(buf, risk_table) == 0) {
			is[IN_RISK_TABLE] = 1;
		}
	}

	if(regex_check(buf, code) == 0) {
		is[IN_CODE] = !is[IN_CODE];
		if(!is[IN_CODE]) {
			is[IN_CS] = 0;
			is[IN_NCCE] = 0;
		}
		else {
			if(regex_check(buf, cs) == 0) {
				is[IN_CS] = 1;
			}
			if(regex_check(buf, ncce) == 0) {
				is[IN_NCCE] = 1;
			}
		}
	}

	if(regex_check(buf, function_dec) == 0) {
		is[IN_FUNCTION_DECLARATION] = 1;
	}
	else {
		is[IN_FUNCTION_DECLARATION] = 0;
	}
}

/* Run the checkers as appropriate */
int process_line(char *buf, size_t size, int *is) {
	ascertain_environment(buf, is);

	if(is[IN_CODE]) {
		if(!is[IN_FUNCTION_DECLARATION]) {
			CHECK(ellipses);
		}
		CHECK(conditional_whitespace);
		CHECK(commas);
	}

	CHECK(asterisk_placement);
	CHECK(leftmost_const);
	CHECK(link_consistency);

	if(is[IN_RISK_TABLE]) {
	  // UNCOMMENT
	  //		check_risk_table(buf, size, rulename, is);
	}

	if(is[IN_REFERENCE]) {
		CHECK(references);
	}

	fprintf(stdout, "%s", buf);

	return 0;
}
