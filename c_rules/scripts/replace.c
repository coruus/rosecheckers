#include "main.h"

/* int vector for which arg the %'s map to */
int replace(char *dest, char *src, char *format_str, regmatch_t **match_regs, char **match_strs, size_t s, int numargs, int *vec) {
	char match[(strlen(format_str)+1)];
	regmatch_t *t = *match_regs;
	char *matcher = match;
	size_t i;
	char *temp;
	int flag = 0;
	dest[0] = '\0';
	strcpy(matcher, format_str);

	//was 1
	strncat(dest, src, t[0].rm_so);
	
	if(format_str[0] == '%') {
		temp = "";
		flag = 1;
	} 
	else {
		temp = strtok(matcher, "%");
	}
	for(i = 0; i < numargs; ++i) {
		if(temp == NULL)
			break;

		strcat(dest, temp);
		strcat(dest, match_strs[vec[i]]);
		if(flag) {
			temp = strtok(matcher, "%");
			flag = 0;
		}
		else
			temp = strtok(NULL, "%");
	}
	
	if(temp!=NULL) {
		if(strcmp(temp, " ") != 0 || temp[strlen(temp)-1] != '%')
			strcat(dest, temp);
	}

	if(t[numargs].rm_eo < strlen(src))
		strcat(dest, src + t[0].rm_eo);

	return 0;
}

