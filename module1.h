#ifndef MODULE1_H
#define MODULE1_H
# include "shared3.h"
# include "part2.h"


#define MAX_NAME_LENGTH 50
#define PASSWORD_LENGTH 9
#define ENCODING_PASSWORD_LENGTH 33 
#define MAX_ANSWER_LENGTH 100
#define MAX_INT_LENGTH 10
#define MAX_LINE_LENGTH MAX_NAME_LENGTH + ENCODING_PASSWORD_LENGTH + MAX_ANSWER_LENGTH + MAX_INT_LENGTH + 12
#define MAX_IDENTITY_LENGTH MAX_NAME_LENGTH + PASSWORD_LENGTH + 6


typedef struct valid_user {
	char   username[MAX_NAME_LENGTH];
	char   password[ENCODING_PASSWORD_LENGTH];
	int    randomNum;
	char   securityAnswer[MAX_ANSWER_LENGTH];
	/* A pointer to the next node in the list */
	struct valid_user *next;
} valid_user;

valid_user* readFromFile(FILE* fPtrRead, valid_user *users_list);
valid_user* create_user(char* username, char* password, int random, char* answer);
valid_user* add_user(valid_user *head, char* username, char* password, int random, char* answer);
valid_user new_user(valid_user* head);
valid_user* find_user(valid_user *head, char* name);
void new_account_menu(valid_user* head, char* argi, char* argi2);
void login(valid_user* head, char* argi, char* argi2);
void free_valid_user_list(valid_user* head);
void exit_app(valid_user* head);
void exit_app_before_login(char* argi, valid_user* head);
void login_menu(valid_user* head, char* argi, char* argi2);
void forgot_pass(valid_user* head, char* name, char* password, char* argi, char* argi2);
void forgot_pass_help(valid_user* head, char* name, char* password, char* argi, char* argi2);
void wrong_password(valid_user* head, char* name, char* password, char* argi, char* argi2);
void new_password_menu(valid_user* head, char* name, char* argi, char* argi2);
void change_password(valid_user* head, char* name, char* argi, char* argi2);
char* choose_password(char* username, char* first_name, char* password);
char* choose_answer(char* first_name, char* answer);
int enter_password(char* password);
int name_check(valid_user *head, char* name);
int password_check(valid_user *head, char* name, char* password);
int answer_check(valid_user *head, char* name, char* answer);
int format_check(char* input);
int get_random_num(void);
int random_of_user(valid_user *head, char* name);
void encoding(char password[], int random, char new_pass[]);
void int_to_bin_str(int value, char* output);
void WriteToFile(FILE* fPtrwrite, valid_user *users_list);
#endif
