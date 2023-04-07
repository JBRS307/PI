#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define TEST 0  // 1 - dla testowania,  0 - dla automatycznej oceny

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int same_digrams(const char a[2], const char b[2]){
	return a[0] == b[0] && a[1] == b[1];
}

int same_strings(const char *a, const char *b, size_t n){
	for(unsigned long i = 0; i < n; i++){
		if(a[i] != b[i]) return 0;
	}
	return 1;
}

int find_idents(FILE *stream){
	int true_ident;
    int idents = 0;
	char str[2];
	char *curr;
	curr = (char*)malloc(MAX_ID_LEN*sizeof(char));
	curr[0] = '\0';
	char **ids;
	ids = (char**)malloc(MAX_IDS*sizeof(char*));
	for(int i = 0; i < MAX_IDS; i++) ids[i] = (char*)malloc(MAX_ID_LEN*sizeof(char));
	for(int i = 0; i < MAX_IDS; i++) ids[i][0] = '\0';

	size_t curr_len;
	int str_status = 0;

	str[0] = fgetc(stream);
	if(str[0] == '"') str_status = IN_STRING;
	else if(str[0] == '_' || islower(str[0]) || isupper(str[0])){
		str_status = IN_ID;
		idents++;
		strncat(curr, &str[0], 1);
		true_ident = 1;
	}
	while((str[1] = fgetc(stream)) != EOF){
		switch(str_status){
			case IN_LINE_COMMENT:{
				if(str[1] == '\n') str_status = 0;
				break;
			}
			case IN_BLOCK_COMMENT:{
				if(same_digrams(str, "*/")){
					str_status = 0;
					str[1] = fgetc(stream);
				}
				break;
			}
			case IN_ID:{
				if(!(islower(str[1]) || isupper(str[1]) || str[1] == '_')) true_ident = 0;
				if(isspace(str[1])){
					int loop_2 = 1;
					str_status = 0;
					curr_len = strlen(curr);
					for(int i = 0; i < 32; i++){
						if(curr_len == strlen(keywords[i]) && same_strings(curr, keywords[i], curr_len)){
							idents--;
							loop_2 = 0;
							break;
						}
					}
					if(loop_2){
						for(int i = 0; i < MAX_IDS; i++){
							if(curr_len == strlen(ids[i]) && same_strings(curr, ids[i], curr_len)){
								idents--;
								break;
							}else if(strlen(ids[i]) == 0){
								strcat(ids[i], curr);
								break;
							}
						}
					}
					curr[0] = '\0';
				}else if(true_ident) strncat(curr, &str[1], 1);
				break;
			}
			case IN_STRING:{
				if(str[1] == '"') str_status = 0;
				break;
			}
			default:{
				if(str[1] == '"') str_status = IN_STRING;
				else if((str[1] == '_' || isupper(str[1]) || islower(str[1])) && (isspace(str[0]))){
					str_status = IN_ID;
					strncat(curr, &str[1], 1);
					idents++;
					true_ident = 1;
				}else if(same_digrams(str, "//")){
					str_status = IN_LINE_COMMENT;
					str[1] = fgetc(stream);
				}else if(same_digrams(str, "/*")){
					str_status = IN_BLOCK_COMMENT;
					str[1] = fgetc(stream);
				}
			}
		}
		str[0] = str[1];
	}

	for(int i = 0; i < MAX_IDS; i++) free(ids[i]);
	free(ids);
	free(curr);
	return idents;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	char file_name[40];
	FILE *stream;

  if(TEST)  stream = stdin;
  else {
      scanf("%s",file_name);
      stream = fopen(file_name,"r");
      if(stream == NULL) {
          printf("fopen failed\n");
          return -1;
      }
  }
	printf("%d\n", find_idents(stream));
	return 0;
}

