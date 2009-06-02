#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <assert.h>
#include <stdarg.h>

#define LANG_CPP 1
#define LANG_C 2
#define CURRENT_LANG LANG_C

int get_next_line(char *buf, size_t size, FILE **file);
int process_line(char *buf, size_t size, int *is);
void error(char const *msg);

void regex_compile(regex_t *compiled_regex, char const *regex);
int regex_check(char *buf, char const *regex);
regmatch_t *regex_match(regex_t* compiled_regex, char* text, char* matches[], int max_match_lengths[]);

void *Malloc(size_t len);
long Strtol(char const *nptr, char **endptr, int base);
void logger(char const *msg);
int open_log(char const *filename);
void inc_line(void);

int check_ellipses(char *buf, size_t len);
int check_leftmost_const(char *buf, size_t len);
int check_asterisk_placement(char *buf, size_t len);
int check_operator_spaces(char *buf, size_t len);
int check_conditional_whitespace(char *buf, size_t len);
int check_commas(char *buf, size_t len);
int check_references(char *buf, size_t len);
int check_link_consistency(char *buf, size_t len);
int check_risk_table(char *buf, size_t len, char *rulename, int *is);

int replace(char *dest, char *src, char *format_str, regmatch_t **match_regs, char **match_strs, size_t s, int numargs, int *vec);

enum { FILENAME_LEN = 1024, BUF_SIZE = 4096};
enum { IN_CODE, IN_RISK_ASSESSMENT, IN_CS, IN_NCCE, IN_REFERENCE, IN_RISK_TABLE, IN_FUNCTION_DECLARATION, A_RULE, IS_ARR_SIZE };

#define RULE "[[:alnum:]][[:alnum:]][[:alnum:]][[:alnum:]][[:alnum:]]-[AC]\\.?"

#endif
