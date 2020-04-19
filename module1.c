#include "part2.h"
#include "module1.h"
#include "shared3.h"

valid_user* readFromFile(FILE *fPtrRead, valid_user *users_list)
{
	/*
	create a new linked list with all the users from the validation file.
	gets pointers for the head of the list.
	*/

	char needle[] = "_$_";
	char line[MAX_LINE_LENGTH] = { 0 };
	char username[MAX_NAME_LENGTH];
	char password[ENCODING_PASSWORD_LENGTH];
	char* ret;
	char* helper_ptr;
	char* end_of_file;
	int num, length;
	valid_user *user;

	while (!feof(fPtrRead))
	{
		fgets(line, MAX_LINE_LENGTH, fPtrRead);
		if (*line == 0)
			return NULL;
		length = strlen(line);
		ret = line;
		end_of_file = line;
		helper_ptr = strstr(line, needle);
		ret = helper_ptr + 3;
		*helper_ptr = '\0';
		strcpy(username, line);
		helper_ptr = strstr(ret, needle);
		*helper_ptr = '\0';
		strcpy(password, ret);
		ret = helper_ptr + 3;
		helper_ptr = strstr(ret, needle);
		*helper_ptr = '\0';
		num = atoi(ret);
		ret = helper_ptr + 3;
		helper_ptr = line + length - 1;
		end_of_file = line + length;
		if (*helper_ptr != '\n')
		{
			*end_of_file = '\0';
			user = create_user(username, password, num, ret);
			user->next = users_list;
			users_list = user;
			return users_list;
		}
		*helper_ptr = '\0';
		user = create_user(username, password, num, ret);
		user->next = users_list;
		users_list = user;
	}

	return users_list;
}

valid_user* create_user(char* username, char* password, int random, char* answer)
{

	valid_user *new_user;
	new_user = (valid_user*)malloc(sizeof(valid_user));
	if (new_user == NULL)
	{
		printf("Fatal error: memory allocation failed!\n");
		exit(1);
	}
	strcpy(new_user->username, username);
	strcpy(new_user->password, password);
	new_user->randomNum = random;
	strcpy(new_user->securityAnswer, answer);
	new_user->next = NULL;

	return new_user;
}


valid_user* add_user(valid_user *head, char* username, char* password, int random, char* answer)
{
	valid_user *new_user = create_user(username, password, random, answer);
	if (new_user == NULL)
		return NULL;
	new_user->next = head;
	return new_user;
}


valid_user* find_user(valid_user *head, char* name)
{
	/*
	find a user in the list by his username and returns a pointer to the user
	*/
	while (head != NULL && (strcmp(head->username, name) != 0))
		head = head->next;
	return head;
}


void new_account_menu(valid_user* head, char* argi, char* argi2)
{
	/*
	create a new account and update the linked list
	*/
	int chars_in_password, random;
	char username[MAX_NAME_LENGTH];
	char first_name[MAX_NAME_LENGTH];
	char password[PASSWORD_LENGTH];
	char encoding_password[ENCODING_PASSWORD_LENGTH];
	char answer[MAX_ANSWER_LENGTH];
	profileN* headNm = NULL;
	valid_user* new_user;
	FILE* fPtrRead2;

	printf("Hello dear guest, please choose a username.\nInput: ");
	gets(username);
	while ((name_check(head, username) == 1) || (is_legal_name(username) != 0))
	{
		if (name_check(head, username) == 1) // check if the name appear in validation 
		{
			printf("\nWe are truly sorry - The username you chose is already used. Please choose again.\nInput: ");
		}
		else if (is_legal_name(username) != 0) // check if the name is legal
		{
			printf("\nDear guest, your username must contain only letters and at least one space. Please try again.\nInput: ");
		}
		gets(username);
	}

	choose_password(username, first_name, password);
	random = get_random_num();
	encoding(password, random, encoding_password);
	choose_answer(first_name, answer);
	new_user = add_user(head, username, encoding_password, random, answer);
	fPtrRead2 = fopen(argi2, "r");
	if (fPtrRead2 == NULL)
	{
		printf("There was error using files\n");
		exit(1);
	}
	headNm = createDB(fPtrRead2, headNm, new_user);
	fclose(fPtrRead2);
	loginProfile(username, headNm, argi, argi2, new_user, 1);
	return;
}


void login(valid_user* head, char* argi, char* argi2)
{
	/*
	if the user choose '1' at the first menu (try to login as a valid user)
	*/
	int ind = 0, input_check, random;
	char identity[MAX_IDENTITY_LENGTH];
	char name[MAX_NAME_LENGTH];
	char encoding_password[ENCODING_PASSWORD_LENGTH];
	char needle[] = "::";
	char* password;
	char* helper_ptr;
	profileN* headNm = NULL;
	FILE* fPtrRead2;

	printf("Please enter your username and password in the format: 'username::password'.\n");
	printf("To go back to the first screen enter '#'\n");
	printf("To exit the app please enter '$'.\n\nInput: ");
	while (ind == 0)
	{
		gets(identity);
		printf("\n");
		if (strcmp(identity, "#") == 0)
		{
			printf("\n\n\n========================================================================\n\n\n");
			login_menu(head, argi, argi2);
			break;
		}
		if (strcmp(identity, "$") == 0)
		{
			exit_app_before_login(argi, head);
			free_valid_user_list(head);
			exit(0);

		}
		else
		{
			input_check = format_check(identity);
			if (input_check != 0) // the user entered wrong input
			{
				printf("\n\nDear guest, please enter your username and password in the following way: username::password.\n");
				printf("Username does not exist in our memory banks. Please try again.\n\n");
				printf("\n\n========================================================================\n\n\n");
				login_menu(head, argi, argi2);
				return;
			}
			password = identity;
			helper_ptr = strstr(identity, needle);
			password = helper_ptr + 2;
			*helper_ptr = '\0';
			strcpy(name, identity);
			if (name_check(head, name) == 1) // check if the name appear in validation 
			{
				random = random_of_user(head, name);
				if (is_legal_password(password) == 0)
				{
					encoding(password, random, encoding_password);
					if (password_check(head, name, encoding_password) == 1)
					{
						fPtrRead2 = fopen(argi2, "r");
						if (fPtrRead2 == NULL)
						{
							printf("There was error using files\n");
							exit(1);
						}
						headNm = createDB(fPtrRead2, headNm, head);
						fclose(fPtrRead2);
						loginProfile(name, headNm, argi, argi2, head, 0);
						return;
					}
				}

				else
				{
					wrong_password(head, name, password, argi, argi2);
					return;
				}
			}
			else
			{
				printf("Username does not exist in our memory banks. Please try again.\n\n");
				printf("\n\n========================================================================\n\n\n");
				login_menu(head, argi, argi2);
				return;
			}
		}
	}
}


void free_valid_user_list(valid_user* head)
{
	/*
	free the list of all the valid users
	*/
	valid_user* temp;
	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

void exit_app_before_login(char* argi, valid_user* head)
{
	/*
	exit the app and free the list of all the valid users
	*/
	valid_user* copy_head = head;
	FILE* valid_fptr = NULL;

	printf("\n\n\n ========================================================================\n\n\n");
	printf("Thank you for using 'SocioPath', we hope to see you again!\nGoodbye!\n");
	valid_fptr = fopen(argi, "w");
	if (valid_fptr == NULL)
	{
		printf("There was error using files\n");
		exit(1);
	}
	WriteToFile(valid_fptr, head);
	fclose(valid_fptr);
}


void login_menu(valid_user* head, char* argi, char* argi2)
{
	/*
	the first app menu
	*/
	int ind = 0;
	char choice, is_enter;
	printf("Hello dear guest and welcome to 'SocioPath - Socialize in an alternative path'!\n");
	printf("What would you like to do next?\n");
	printf("1.Log in.\n2.Create a new account\n3.Exit the App\n\n");
	printf("input: ");
	while (ind == 0)
	{
		choice = getchar(); //get the user's command
		if (choice == '\n')
		{
			printf("\nInvalid input. Dear guest, please enter '1', '2' or '3'.\n\nInput: ");
			continue;
		}
		is_enter = getchar();
		//printf("\n");
		if (choice == '1' && is_enter == '\n')
		{
			printf("\n\n========================================================================\n\n\n");
			login(head, argi, argi2);
			break;
		}
		else if (choice == '2' && is_enter == '\n')
		{
			printf("\n\n========================================================================\n\n\n");
			new_account_menu(head, argi, argi2);
			break;
		}
		else if (choice == '3' && is_enter == '\n')
		{
			exit_app_before_login(argi, head);
			exit(0);
		}
		else
		{
			printf("\nInvalid input. Dear guest, please enter '1', '2' or '3'.\n\nInput: ");
			if (is_enter == '\n')
				continue;
			choice = getchar();
			while (choice != '\n')
				choice = getchar();
		}
	}
}



void forgot_pass(valid_user* head, char* name, char* password, char* argi, char* argi2)
{
	/*
	asking the security question
	*/

	int tries = 1;
	char choice[MAX_ANSWER_LENGTH];
	char first_name[MAX_NAME_LENGTH];

	get_first_name(name, first_name);

	printf("\n\n\n========================================================================\n\n\n");
	printf("Question : When was your last underground guitar jam? \n");
	printf("To go back to the first screen enter '#'\n");
	printf("To exit the app please enter '$'.\n\nInput : ");

	gets(choice);
	printf("\n");
	if (strcmp(choice, "#") == 0)
	{
		printf("\n\n========================================================================\n\n\n");
		login_menu(head, argi, argi2);
		return;
	}
	else if (strcmp(choice, "$") == 0)
	{
		exit_app_before_login(argi, head);
		free_valid_user_list(head);
		exit(0);

	}
	else
	{
		if (answer_check(head, name, choice) == 1)
		{
			printf("\n\n========================================================================\n\n\n");
			change_password(head, name, argi, argi2);
			printf("\n\n========================================================================\n\n\n");
			login_menu(head, argi, argi2);
			return;
		}
		while (answer_check(head, name, choice) != 1 && tries < 3)
		{
			printf("\nDear %s, you have entered a wrong answer, please try again.\n", first_name);
			forgot_pass_help(head, name, password, argi, argi2);
			tries++;
		}
		if (answer_check(head, name, choice) == 1)
		{
			printf("\n\n========================================================================\n\n\n");
			change_password(head, name, argi, argi2);
			return;
		}
		if (tries == 3)
		{
			printf("\nDear %s, you failed to enter the correct answer 3 times.\n", first_name);
			printf("\n\n========================================================================\n\n\n");
			login_menu(head, argi, argi2);
			return;
		}
	}
}

void forgot_pass_help(valid_user* head, char* name, char* password, char* argi, char* argi2)
{

	char choice[MAX_ANSWER_LENGTH];
	char first_name[MAX_NAME_LENGTH];

	get_first_name(name, first_name);

	printf("\n\n\n========================================================================\n\n\n");
	printf("Question : When was your last underground guitar jam? \n");
	printf("To go back to the first screen enter '#'\n");
	printf("To exit the app please enter '$'.\n\nInput : ");

	gets(choice);

	printf("\n");
	if (strcmp(choice, "#") == 0)
	{
		printf("\n\n========================================================================\n\n\n");
		login_menu(head, argi, argi2);
		return;
	}
	else if (strcmp(choice, "$") == 0)
	{
		exit_app_before_login(argi, head);
		free_valid_user_list(head);
		exit(0);
	}
	else if (answer_check(head, name, choice) == 1)
	{
		printf("\n\n========================================================================\n\n\n");
		change_password(head, name, argi, argi2);
		printf("\n\n========================================================================\n\n\n");
		login_menu(head, argi, argi2);
		return;
	}
}



void wrong_password(valid_user* head, char* name, char* password, char* argi, char* argi2)
{
	/*
	if the user enter a wrong password
	*/
	int ind = 2, random;
	char temp, is_enter;
	char choice[PASSWORD_LENGTH];
	char first_name[MAX_NAME_LENGTH];
	char encoding_password[ENCODING_PASSWORD_LENGTH];
	FILE* fPtrRead2;
	profileN* headNm = NULL;

	get_first_name(name, first_name);

	printf("\n\n========================================================================\n\n\n");
	printf("Dear %s, you have entered a wrong password.\n", first_name);
	printf("Please choose one of the following options : \n");
	printf("1.Re-enter password\n");
	printf("2.If you forgot your password, press '!'\n");
	printf("3.To go back to the first screen enter '#'\n");
	printf("4.To exit the app please enter '$'.\n\nInput : ");
	temp = getchar();
	is_enter = getchar();


	if (temp == '!' && is_enter == '\n')
	{
		forgot_pass(head, name, password, argi, argi2);
		return;
	}
	else if (temp == '#' && is_enter == '\n')
	{
		printf("\n\n========================================================================\n\n\n");
		login_menu(head, argi, argi2);
		return;
	}
	else if (temp == '$' && is_enter == '\n')
	{
		exit_app_before_login(argi, head);
		free_valid_user_list(head);
		exit(0);
	}
	else if (is_enter == '\n')
	{
		wrong_password(head, name, password, argi, argi2);
	}
	else
	{
		choice[0] = temp;
		choice[1] = is_enter;
		for (ind = 2; ind < 8; ind++)
		{
			choice[ind] = getchar();
			if (choice[ind] == '\n')
			{
				wrong_password(head, name, password, argi, argi2);
			}
		}
		is_enter = getchar();
		if (is_enter != '\n')
		{
			while (is_enter != '\n')
			{
				is_enter = getchar();
			}
			wrong_password(head, name, password, argi, argi2);
			return;
		}
		choice[ind] == '\0';
		random = random_of_user(head, name);
		encoding(choice, random, encoding_password);
		if (password_check(head, name, encoding_password) == 1)
		{
			fPtrRead2 = fopen(argi2, "r");
			if (fPtrRead2 == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			headNm = createDB(fPtrRead2, headNm, head);
			fclose(fPtrRead2);
			loginProfile(name, headNm, argi, argi2, head, 0);
			return;
		}
		else
			wrong_password(head, name, password, argi, argi2);
	}
}

void new_password_menu(valid_user* head, char* name, char* argi, char* argi2)
{
	/*
	update the password
	*/
	int random;
	char new_pass[PASSWORD_LENGTH];
	char new_encoding_pass[ENCODING_PASSWORD_LENGTH];
	char first_name[MAX_NAME_LENGTH];
	valid_user* user;

	get_first_name(name, first_name);
	printf("Hello dear %s! Please choose a new password.\nInput: ", name);
	gets(new_pass);
	printf("\n");
	if (is_legal_password(new_pass) != 0)
	{
		printf("\n\nDear %s, your password must be 8 characters long, and contain at least ", first_name);
		printf("each of the following : A digit, a lower case letter and an upper case letter. ");
		printf("Please try again.\n");
		wrong_password(head, name, new_pass, argi, argi2);
	}
	user = find_user(head, name);
	random = user->randomNum;
	encoding(new_pass, random, new_encoding_pass);
	strcpy(user->password, new_encoding_pass);
	return;
}




void change_password(valid_user* head, char* name, char* argi, char* argi2)
{
	/*
	update the password if the user forgot it
	*/
	int random;
	char new_pass[PASSWORD_LENGTH];
	char new_encoding_pass[ENCODING_PASSWORD_LENGTH];
	char first_name[MAX_NAME_LENGTH];
	valid_user* user;

	get_first_name(name, first_name);
	printf("Hello dear %s! Please choose a new password.\nInput: ", name);
	enter_password(new_pass);
	printf("\n");
	while (is_legal_password(new_pass) != 0)
	{
		printf("\n\nDear %s, your password must be 8 characters long, and contain at least ", first_name);
		printf("each of the following : A digit, a lower case letter and an upper case letter. ");
		printf("Please try again.\n");
		printf("input: ");
		enter_password(new_pass);
	}
	user = find_user(head, name);
	random = user->randomNum;
	encoding(new_pass, random, new_encoding_pass);
	strcpy(user->password, new_encoding_pass);
	return;
}




char* choose_password(char* username, char* first_name, char* password)
/*
check the new password while creating a new account
*/
{
	int chars_in_password;
	char password_copy[9];
	char* ptr_pass;

	get_first_name(username, first_name);
	printf("Welcome %s! Please choose a password.\nInput: ", username);
	chars_in_password = enter_password(password);
	while ((is_legal_password(password) != 0) || chars_in_password != 8) //wrong password
	{
		printf("\n\nDear %s, your password must be 8 characters long, and contain at least ", first_name);
		printf("each of the following : A digit, a lower case letter and an upper case letter. ");
		printf("Please try again.\n");
		chars_in_password = enter_password(password);
	}
	printf("\nPlease validate your password.\nInput: ");
	enter_password(password_copy);
	if (strcmp(password, password_copy) != 0)
	{
		printf("\n\nWe are truly sorry - The passwords do not match. Please repeat password acquisition process.\n");
		choose_password(username, first_name, password);
	}
	printf("\n");
	return password;
}



void encoding(char password[], int random, char new_pass[])
{
	/*
	encoding the password
	*/
	int i = 1, temp, result;
	temp = password[0];
	for (i = 1; i < PASSWORD_LENGTH; i++)
	{
		temp = temp & password[i];
	}
	if (temp % 2 == 0)
		temp = temp << 4;
	else
		temp = temp >> 6;
	result = temp ^ random;
	int_to_bin_str(result, new_pass);
}


void int_to_bin_str(int value, char* output)
{
	/*
	get the binary representation
	*/
	int i;
	output[32] = '\0';
	for (i = 31; i >= 0; --i, value >>= 1)
	{
		output[i] = (value & 1) + '0';
	}
}



char* choose_answer(char* first_name, char* answer)
/*
check the security answer while creating a new account
*/
{
	printf("Question: When was your last underground guitar jam?\nInput: ");
	gets(answer);
	while (is_legal_answer(answer) != 0)
	{
		printf("\nDear %s, security answer can contain only letters, numbers and spaces. ", first_name);
		printf("Please try again.\n");
		choose_answer(first_name, answer);
	}

}



int enter_password(char* password)
/*
replace password entry with asterisks
returns the number of chars in password. saves only 8 of the first chars of password.
we should make sure that the returned value is equal to 8.
*/
{
	int ch, counter = 8;
	int i = 0;

	fflush(stdout);
	while ((ch = _getch()) != EOF
		&& ch != '\n'
		&& ch != '\r'
		&& i < 8)
	{
		if (ch == '\b' && i > 0)
		{
			printf("\b \b");
			fflush(stdout);
			i--;
			password[i] = '\0';
		}
		else if (isalnum(ch))
		{
			putchar('*');
			password[i++] = (char)ch;
		}
	}
	password[i] = '\0';

	while (ch != EOF
		&& ch != '\n'
		&& ch != '\r')
	{
		if (ch != '\b')
		{
			putchar('*');
			counter++;
		}
		else
		{
			printf("\b \b");
			fflush(stdout);
			counter--;
		}
		ch = _getch();
	}
	return counter;
}

int name_check(valid_user *head, char* name)
{
	/*
	check if a name is in the validation list, and returns 1 in this case. returns 0 otherwise.
	*/
	valid_user *copy_head;
	copy_head = head;
	while (copy_head != NULL && (strcmp(copy_head->username, name) != 0))
		copy_head = copy_head->next;
	if (copy_head != NULL)
		return 1;
	return 0;
}

int password_check(valid_user *head, char* name, char* password)
{
	/*
	check if the password is correct, and returns 1 in this case. returns 0 otherwise.
	*/
	valid_user *copy_head;
	copy_head = head;
	while (copy_head != NULL && (strcmp(copy_head->username, name) != 0))
		copy_head = copy_head->next;
	if (copy_head != NULL)
	{
		if (strcmp(copy_head->password, password) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int answer_check(valid_user *head, char* name, char* answer)
{
	/*
	check if the answer is correct, and returns 1 in this case. returns 0 otherwise.
	*/
	valid_user *copy_head;
	copy_head = head;
	while (copy_head != NULL && (strcmp(copy_head->username, name) != 0))
		copy_head = copy_head->next;
	if (copy_head != NULL)
	{
		if (strcmp(copy_head->securityAnswer, answer) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int format_check(char* input)
{
	/*
	check if the user entered his username and password with '::' between them.
	returns 0 if so, and -1 otherwise.
	*/

	char needle[] = "::";
	char* ptr;
	ptr = strstr(input, needle);
	if (ptr != NULL)
		return 0;
	return -1;
}


int get_random_num(void)
{
	/*
	returns a random number for creating a new account
	*/
	int number = 0;
	srand(time(NULL));
	number = rand();
	return number;
}

int random_of_user(valid_user *head, char* name)
{
	/*
	returns the random num of the user.
	*/
	int random_num;
	while (head != NULL && (strcmp(head->username, name) != 0))
		head = head->next;
	if (head != NULL)
	{
		random_num = head->randomNum;
		return random_num;

	}
	return 0;
}

void WriteToFile(FILE* fPtrwrite, valid_user *users_list)
{
	valid_user *users_list_copy = users_list;
	while (users_list_copy != NULL)
	{
		if (users_list_copy->next == NULL)
		{
			fprintf(fPtrwrite, "%s_$_", users_list_copy->username);
			fprintf(fPtrwrite, "%s_$_", users_list_copy->password);
			fprintf(fPtrwrite, "%d_$_", users_list_copy->randomNum);
			fprintf(fPtrwrite, "%s", users_list_copy->securityAnswer);
			return;
		}
		fprintf(fPtrwrite, "%s_$_", users_list_copy->username);
		fprintf(fPtrwrite, "%s_$_", users_list_copy->password);
		fprintf(fPtrwrite, "%d_$_", users_list_copy->randomNum);
		fprintf(fPtrwrite, "%s\n", users_list_copy->securityAnswer);
		users_list_copy = users_list_copy->next;
	}
}







