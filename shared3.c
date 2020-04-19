#include "shared3.h"


int is_legal_name(char* name)
{
	/*
	check if a username is legal, returns 0 if so, and -1 otherwise.
	*/
	int i = 0, size;

	size = strlen(name);
	for (i = 0; i < size; i++)
	{
		if (isalpha(name[i]) == 0 && name[i] != ' ')
		{
			return -1;
		}
	}
	for (i = 0; i < size; i++)
	{
		if (name[i] == ' ')
		{
			return 0;
		}
	}
	return -1;
}

int is_legal_password(char* password)
{
	/*
	check if a password is legal, returns 0 if so, and -1 otherwise.
	*/
	int i = 0, counter = 0, size;

	size = strlen(password);
	if (size != 8)
	{
		return -1;
	}
	for (i = 0; i < size; i++)
	{
		if (isupper(password[i]) != 0)
		{
			counter++;
			break;
		}
	}
	for (i = 0; i < size; i++)
	{
		if (islower(password[i]) != 0)
		{
			counter++;
			break;
		}
	}
	for (i = 0; i < size; i++)
	{
		if (isdigit(password[i]) != 0)
		{
			counter++;
			break;
		}
	}
	if (counter != 3)
	{
		return -1;
	}
	for (i = 0; i < size; i++)
	{
		if (isalnum(password[i]) == 0)
		{
			return -1;
		}
	}
	return 0;
}

int is_legal_answer(char* answer)
{
	/*
	check if an answer is legal, returns 0 if so, and -1 otherwise.
	*/
	int i = 0, counter = 0, size;

	size = strlen(answer);
	for (i = 0; i < size; i++)
	{
		if (isalnum(answer[i]) == 0 && answer[i] != ' ')
		{
			return -1;
		}
	}
	return 0;
}

void get_first_name(char* name, char* first_name)
{
	/*
	get the first word of a username
	*/

	char needle[2] = " ";
	char* helper_ptr;

	strcpy(first_name, name);
	helper_ptr = strstr(first_name, needle); //get user's first name
	if (helper_ptr != NULL)
	{
		*helper_ptr = '\0';
	}
}