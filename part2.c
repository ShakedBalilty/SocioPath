#include "part2.h"
#include "shared3.h"
#include "module1.h"


profile* createProfile(char* username, char* status, char* friends, profile* pending_head)// create a new profile from 4 strings try to use strcpy_s
{
	profile* returned = (profile*)malloc(sizeof(profile));
	if (returned == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	returned->friends = (char*)calloc(strlen(friends) + 10, sizeof(char));
	if (returned->friends == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	strcpy(returned->username, username);
	strcpy(returned->status, status);
	strcpy(returned->friends, friends);
	returned->requestHead = pending_head;
	return returned;
}

profileN* createProfileNode(char* username, char* status, char* friends, profile* pending_head, profileN* next_profilen)// create a profile node containing profileStruct and next node IS IT OK CHOOSE THE NULL ARGUMENT ????
{
	profileN* new_profile_node = (profileN*)malloc(sizeof(profileN));
	if (new_profile_node == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	new_profile_node->profile_strct = *(createProfile(username, status, friends, pending_head));
	new_profile_node->next = next_profilen;
	return new_profile_node;
}
profileN* findUser(char* username, profileN *head)// return a pointer to the node of the user with this username
{
	profileN *ptr = head;
	while (ptr != NULL)
	{
		if (strcmp(ptr->profile_strct.username, username) == 0)
			return ptr;
		ptr = ptr->next;
	}
	return ptr;
}
int numOfUsers(profileN *head)
{
	int counter = 0;
	profileN* temp;
	while (head != NULL)
	{
		counter++;
		temp = head->next;
		head = temp;
	}

	return counter;
}
profile* createLinkedPendingList(char* pending_str, profileN* head)// create a linked list of profiles pending request
{
	profile* returned_head;
	profile* temp_profile;
	profile* temp_next_profile;
	profileN* temp_node, *orig_head;
	char* str_ptr;
	char curr_friend[USERNAME_LENGTH];
	int n;

	if (strcmp(pending_str, "") == 0)
		return NULL;
	orig_head = head;
	str_ptr = strstr(pending_str, "_$_");
	if (str_ptr == NULL)
	{
		strcpy(curr_friend, pending_str);
		temp_node = findUser(curr_friend, head);
		returned_head = createProfile(temp_node->profile_strct.username, "", "", NULL);
	}
	else
	{
		n = str_ptr - pending_str;
		memset(curr_friend, 0, USERNAME_LENGTH);
		strncpy(curr_friend, pending_str, n);
		temp_node = findUser(curr_friend, head);
		head = orig_head;
		returned_head = createProfile(temp_node->profile_strct.username, "", "", NULL);
		temp_profile = returned_head;
		pending_str = str_ptr + 3;
		str_ptr = str_ptr + 3;
		str_ptr = strstr(pending_str, "_$_");

		while (str_ptr != NULL)
		{
			n = str_ptr - pending_str;
			memset(curr_friend, 0, USERNAME_LENGTH);
			strncpy(curr_friend, pending_str, n);
			temp_node = findUser(curr_friend, head);
			head = orig_head;
			temp_next_profile = createProfile(temp_node->profile_strct.username, "", "", NULL);
			temp_profile->requestHead = temp_next_profile;
			temp_profile = temp_next_profile;
			pending_str = str_ptr + 3;
			str_ptr = str_ptr + 3;
			str_ptr = strstr(pending_str, "_$_");

		}
		head = orig_head;
		memset(curr_friend, 0, USERNAME_LENGTH);
		strcpy(curr_friend, pending_str);
		temp_node = findUser(curr_friend, head);
		head = orig_head;
		temp_next_profile = createProfile(temp_node->profile_strct.username, "", "", NULL);
		temp_profile->requestHead = temp_next_profile;
	}
	return returned_head;
}
profileN* createDB(FILE* fPtrRead, profileN* headN, valid_user* val_head)// create DB of a linked list of profile nodes
{
	profileN* front, *original_head, *temp;
	char* str_ptr = NULL, *temp_friends, *help_friends, *temp_pending, *pending, *help_pending;
	char temp_username[USERNAME_LENGTH + 11] = { 0 };
	char temp2_username[USERNAME_LENGTH + 11] = "", temp_status[STATUS_LENGTH + 7], temp2_status[STATUS_LENGTH + 7] = "";
	int len_of_db, n;

	// part 1 of inputing usernames in DB  
	headN = createProfileNode(val_head->username, "", "", NULL, NULL);
	original_head = headN;
	val_head = val_head->next;

	while (val_head != NULL)
	{
		temp = createProfileNode(val_head->username, "", "", NULL, NULL);
		headN->next = temp;
		headN = headN->next;
		val_head = val_head->next;
	}
	headN = original_head;
	//end part 1

	while (!feof(fPtrRead))//part 2 of inputing status and friends in DB
	{
		fgets(temp_username, USERNAME_LENGTH + 11, fPtrRead);
		if (*temp_username == 0)
			return headN;
		if (temp_username )
		str_ptr = temp_username + 9;
		strcpy(temp_username, str_ptr);
		n = strlen(temp_username) - 1;
		memset(temp2_username, 0, USERNAME_LENGTH + 11);
		strncpy(temp2_username, temp_username, n);
		front = findUser(temp2_username, headN);
		headN = original_head;

		fgets(temp_status, STATUS_LENGTH + 7, fPtrRead);
		str_ptr = temp_status + 7;
		strcpy(temp_status, str_ptr);
		n = strlen(temp_status) - 1;
		memset(temp2_status, 0, STATUS_LENGTH + 7);
		strncpy(temp2_status, temp_status, n);
		strcpy(front->profile_strct.status, temp2_status);

		len_of_db = numOfUsers(headN);
		headN = original_head;
		len_of_db = len_of_db*USERNAME_LENGTH + 5 + 3 * len_of_db;
		temp_friends = (char*)calloc(len_of_db, sizeof(char));
		if (temp_friends == NULL)
		{
			printf("failed allocation Memory!\n");
			exit(1);
		}
		fgets(temp_friends, len_of_db, fPtrRead);
		help_friends = temp_friends + 8;
		len_of_db = strlen(help_friends) + 2;
		front->profile_strct.friends = (char*)calloc(len_of_db, sizeof(char));
		if (front->profile_strct.friends == NULL)
		{
			printf("failed allocation Memory!\n");
			exit(1);
		}
		strcpy(front->profile_strct.friends, help_friends);
		*(front->profile_strct.friends + strlen(front->profile_strct.friends) - 1) = '\0';
		free(temp_friends);//end of part 2

		len_of_db = numOfUsers(headN);//part 3 input linked list of pending requsets
		headN = original_head;
		len_of_db = len_of_db*USERNAME_LENGTH + 5 + 3 * len_of_db;
		temp_pending = (char*)calloc(len_of_db, sizeof(char));
		if (temp_friends == NULL)
		{
			printf("failed allocation Memory!\n");
			exit(1);
		}
		fgets(temp_pending, len_of_db, fPtrRead);
		help_pending = temp_pending + 8;
		len_of_db = strlen(temp_pending) + 2;
		pending = (char*)calloc(len_of_db, sizeof(char));
		if (pending == NULL)
		{
			printf("failed allocation Memory!\n");
			exit(1);
		}
		if (strcmp(help_pending, "") != 0)
		{
			if (!feof(fPtrRead))
				n = strlen(help_pending) - 1;
			else
				n = strlen(help_pending);
		}
		else
			n = strlen(help_pending);
		strncpy(pending, help_pending, n);
		free(temp_pending);
		headN = original_head;
		front->profile_strct.requestHead = createLinkedPendingList(pending, headN);
		headN = original_head;
		free(pending);//end of part 3
	}
	return headN;
}
void loginProfile(char* username, profileN* head, char* argi, char* argi2, valid_user* val_head, int is_new)//print the profile at the login 
{
	profileN* original_head = head;
	profileN* user = findUser(username, head);
	printf("\n\n========================================================================\n\n\n");
	printf("Hey %s and welcome to your profile!\n", username);
	if (strcmp(user->profile_strct.status, "") == 0)
	{
		printf("You don't have a status yet.\n");
	}
	else
	{
		printf("Your status: %s\n", user->profile_strct.status);
	}
	printf("See what's on your friends' minds:\n");
	head = original_head;
	printStatuses(user, original_head);
	printf("\n\n========================================================================\n\n\n");
	printOptions();
	takeInput(user, original_head, val_head, argi, argi2, is_new);
}

void printStatuses(profileN* user, profileN* original_head)// print the statuses of the user's friends in the format----free not good
{
	char* friends_string, *orig_friends_string;
	char *str_ptr;
	char friend_username[USERNAME_LENGTH];
	char friend_firstname[USERNAME_LENGTH];
	profileN* temp_friend;
	int size;

	size = strlen(user->profile_strct.friends) + 1;
	friends_string = (char*)calloc(size, sizeof(char));
	if (friends_string == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	orig_friends_string = friends_string;
	strcpy(friends_string, user->profile_strct.friends);

	if (strcmp(friends_string, "") == 0)
	{
		printf("You don't have friends yet.\n");
	}
	else
	{
		str_ptr = strstr(friends_string, "_$_");

		while (str_ptr != NULL)
		{
			*str_ptr = '\0';
			strcpy(friend_username, friends_string);

			temp_friend = findUser(friend_username, original_head);
			if (strcmp(temp_friend->profile_strct.status, "") == 0)
			{
				get_first_name(friend_username, friend_firstname);
				printf("%s has no status.\n", friend_firstname);
			}
			else
			{
				printf("%s: %s\n", temp_friend->profile_strct.username, temp_friend->profile_strct.status);

			}
			friends_string = str_ptr + 3;
			str_ptr += 2;
			str_ptr = strstr(friends_string, "_$_");
		}
		strcpy(friend_username, friends_string);
		temp_friend = findUser(friend_username, original_head);
		if (strcmp(temp_friend->profile_strct.status, "") == 0)
		{
			get_first_name(friend_username, friend_firstname);
			printf("%s has no status.\n", friend_firstname);
		}
		else
		{
			printf("%s: %s\n", temp_friend->profile_strct.username, temp_friend->profile_strct.status);
		}
	}

	free(orig_friends_string);
}

void printOptions(void)// print options menu
{
	printf("What would you like to do next? Choose the option by entering the matched string.\n");
	printf("\"profile\" - show your profile.\n");
	printf("\"status\" -update your current status.\n");
	printf("\"checkStatus <friend_username>\" - check the current status of a specific friend, whose username you enter in the place of <friend_username>.\n");
	printf("\"checkRequests\" - check incoming friend requests.\n");
	printf("\"printFriends\" - print the list of your friends (usernames).\n");
	printf("\"search\" - search SocioPath network for a username.\n");
	printf("\"request <username>\" - send a friend request to the given username\n");
	printf("\"unfriend <friend_username>\" - unfriend the entered friend.\n");
	printf("\"printNetwork\" - print your social network.\n");
	printf("\"#\" - log out and return to the first App screen.\n");
	printf("\"$\" - exit the app.\n\n");
	printf("Input: ");
}


void takeInput(profileN* user, profileN* original_head, valid_user* valid_list, char* argi, char* argi2, int is_new)// the function choose which operation to make in the profile
{
	FILE* valid_fptr;
	FILE* prof_fPtr2;
	int check = 0;
	char input_str[INPUT_LENGTH];
	char* str_ptr;
	profileN* head;
	char private_name[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, private_name);
	gets(input_str);
	while (1)
	{
		if (strcmp(input_str, "profile") == 0)
		{
			printProfile(user, original_head);
		}
		else if (strcmp(input_str, "status") == 0)
		{
			printf("\n\n========================================================================\n\n\n");
			updateStatus(user);
			printProfile(user, original_head);
		}
		else if ((str_ptr = strstr(input_str, "checkStatus")) != NULL)
		{
			str_ptr = str_ptr + 12;
			if ((strcmp(input_str, "checkStatus") == 0))
				printf("\nDear %s, you don't have a friend with the username: %s. You are being transferred to the profile menu to choose again.\n", private_name, "");
			else
				checkUserStatus(user, str_ptr, original_head);
		}
		else if (strcmp(input_str, "checkRequests") == 0)
		{
			checkRequests(user, original_head);
		}
		else if (strcmp(input_str, "printFriends") == 0)
		{
			printf("\n\n========================================================================\n\n\n");
			printFriendsList(user, original_head);
			printf("\n");
		}
		else if ((str_ptr = strstr(input_str, "search")) != NULL)
		{
			printf("\n\n========================================================================\n\n\n");
			str_ptr = str_ptr + 7;
			searchUser(str_ptr, user, original_head);
		}
		else if ((str_ptr = strstr(input_str, "request")) != NULL&&strcmp(input_str, "checkRequests") != 0)
		{
			printf("\n\n========================================================================\n\n\n");
			str_ptr = str_ptr + 8;
			if ((strcmp(input_str, "request") == 0))
				printf("\nDear %s, you don't have a friend with the username: %s. You are being transferred to the profile menu to choose again.\n", private_name, "");
			else
				sendFriendRequset(user, original_head, str_ptr);
		}
		else if ((str_ptr = strstr(input_str, "unfriend")) != NULL)
		{
			printf("\n\n========================================================================\n\n\n");
			str_ptr = str_ptr + 9;
			unfriend(str_ptr, user, original_head);
		}
		else if (strcmp(input_str, "printNetwork") == 0)
		{
			printf("\n\n========================================================================\n\n\n");
			printNetwork(user, original_head);
		}

		else if (strcmp(input_str, "#") == 0)
		{
			valid_fptr = fopen(argi, "w");
			if (valid_fptr == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			prof_fPtr2 = fopen(argi2, "w");
			if (prof_fPtr2 == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			WriteToFile(valid_fptr, valid_list);
			if (is_new == 0)
				uploadData(prof_fPtr2, original_head);
			else
				uploadDataNewUser(prof_fPtr2, original_head);
			free_valid_user_list(valid_list);
			free_profileNode_list(original_head);//
			fclose(valid_fptr);
			fclose(prof_fPtr2);
			valid_fptr = fopen(argi, "r");
			if (valid_fptr == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			valid_list = readFromFile(valid_fptr, NULL);
			fclose(valid_fptr);
			printf("\n\n========================================================================\n\n\n");
			login_menu(valid_list, argi, argi2);
			break;
		}
		else if (strcmp(input_str, "$") == 0)
		{
			printf("\n\n\n========================================================================\n\n\nThank you for using 'SocioPath', we hope to see you again!\nGoodbye!\n");
			valid_fptr = fopen(argi, "w");
			if (valid_fptr == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			prof_fPtr2 = fopen(argi2, "w");
			if (prof_fPtr2 == NULL)
			{
				printf("There was error using files\n");
				exit(1);
			}
			WriteToFile(valid_fptr, valid_list);
			if (is_new == 0)
				uploadData(prof_fPtr2, original_head);
			else
				uploadDataNewUser(prof_fPtr2, original_head);
			free_valid_user_list(valid_list);
			free_profileNode_list(original_head); //
			fclose(valid_fptr);
			fclose(prof_fPtr2);
			exit(0);
		}
		else
		{
			printf("\nDear %s, you have entered an illegal input. ", private_name);
			printf("You are being transferred to the profile menu to choose again.\n");
		}
		printf("\n\n========================================================================\n\n\n");
		printOptions();
		gets(input_str);
	}

}


void printProfile(profileN* user, profileN* original_head)//print the profile of the user
{
	printf("\n\n========================================================================\n\n\n");
	printf("Hey %s and welcome to your profile!\n", user->profile_strct.username);
	if (strcmp(user->profile_strct.status, "") == 0)
	{
		printf("You don't have a status yet.\n");
	}
	else
	{
		printf("Your status: %s\n", user->profile_strct.status);
	}
	printf("Your friends' statuses:\n");
	printStatuses(user, original_head);
}
void updateStatus(profileN* user)// update the user's status
{
	char input_status[STATUS_LENGTH];

	printf("Update your SocioPath status to share with your friends.\n");
	printf("Input:");
	gets(input_status);
	strcpy(user->profile_strct.status, input_status);
}

void checkUserStatus(profileN* user, char* friend_username, profileN* head)//check friend's status
{
	profileN* user_found;
	int isFriend;
	char user_firstname[USERNAME_LENGTH];
	char friend_firstname[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, user_firstname);
	isFriend = findFriendInFriendsList(user, friend_username);
	if (strcmp(friend_username, "") == 0)
	{
		printf("\nDear %s, you don't have a friend with the username: %s. You are being transferred to the profile menu to choose again.\n", user_firstname, friend_username);
	}
	else
	{
		if (isFriend == 1)
		{
			user_found = findUser(friend_username, head);
			if (strcmp(user_found->profile_strct.status, "") != 0)
			{
				get_first_name(user_found->profile_strct.username, friend_firstname);
				printf("\n%s status is: %s\n", friend_firstname, user_found->profile_strct.status);
			}
			if (strcmp(user_found->profile_strct.status, "") == 0)
			{
				get_first_name(user_found->profile_strct.username, friend_firstname);
				printf("\n%s doesn't have a status.\n", friend_firstname);
			}
		}
		if (isFriend == 0)
		{
			printf("\nDear %s, you don't have a friend with the username: %s. You are being transferred to the profile menu to choose again.\n", user_firstname, friend_username);
		}
	}
}

int findFriendInFriendsList(profileN* user, char* friend_username) // check if a user is my friend
{
	char* ptr_help, *orig_temp_friends, *temp_friends;
	int size = strlen(user->profile_strct.friends) + 1;

	temp_friends = (char*)calloc(size, sizeof(char));
	if (temp_friends == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}

	strcpy(temp_friends, user->profile_strct.friends);

	orig_temp_friends = temp_friends;
	ptr_help = strstr(temp_friends, "_$_");
	while (ptr_help != NULL)
	{
		*ptr_help = '\0';
		if (strcmp(temp_friends, friend_username) == 0)
		{
			free(orig_temp_friends);
			return 1;
		}
		ptr_help = ptr_help + 3;
		temp_friends = ptr_help;
		ptr_help = strstr(temp_friends, "_$_");
	}
	if (strcmp(temp_friends, friend_username) == 0)
	{
		free(orig_temp_friends);
		return 1;
	}
	free(orig_temp_friends);
	return 0;


}
int howManyFriends(char* friends_str)// return the number of friends in a friends list
{
	char* str_ptr;
	int counter = 0;

	str_ptr = strstr(friends_str, "_$_");

	while (str_ptr != NULL)
	{
		counter++;
		str_ptr = str_ptr + 2;
		str_ptr = strstr(str_ptr, "_$_");
	}
	if (strcmp(friends_str, "") == 0)
	{
		return 0;

	}
	counter++;
	return counter;
}


void printFriendsList(profileN* user, profileN* head)// print lecsicographic friends list --- possibly the free is not working because of the problem
{
	char* friends_arr;
	int num_of_friends, i;
	char* str_ptr;
	char* friends_str;
	char* orig_friends_str;
	char temp_username[USERNAME_LENGTH];
	char private_name[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, private_name);
	num_of_friends = howManyFriends(user->profile_strct.friends);
	friends_arr = (char*)calloc((num_of_friends*USERNAME_LENGTH), sizeof(char));
	if (friends_arr == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	orig_friends_str = (char*)calloc(strlen(user->profile_strct.friends) + 2 * num_of_friends, sizeof(char));
	if (orig_friends_str == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	friends_str = orig_friends_str;
	strcpy(friends_str, user->profile_strct.friends);
	if (num_of_friends == 0)
	{
		printf("\nDear %s, you don't have any friends yet.\n", private_name);
	}
	else if (num_of_friends == 1)
	{
		printf("%s", user->profile_strct.friends);
	}
	else
	{
		for (i = 0; i<num_of_friends - 1; i++)
		{

			str_ptr = strstr(friends_str, "_$_");
			*str_ptr = '\0';
			strcpy(temp_username, friends_str);
			strcpy((friends_arr + i*USERNAME_LENGTH), temp_username);
			str_ptr = str_ptr + 3;
			friends_str = str_ptr;
		}

		strcpy(temp_username, friends_str);
		strcpy((friends_arr + i*USERNAME_LENGTH), temp_username);
		sortAndPrint(friends_arr, num_of_friends);
		free(friends_arr);
	}
}


void sortAndPrint(char* friends_arr, int arr_size)// sort the array of friends and print it
{
	char temp[USERNAME_LENGTH];
	int i;
	int j;

	for (i = arr_size - 1; i>0; --i)
	{
		for (j = 0; j<i; ++j)
		{
			if (strcmp((friends_arr + j*USERNAME_LENGTH), (friends_arr + (j + 1)*USERNAME_LENGTH))>0)
			{
				strcpy(temp, (friends_arr + j*USERNAME_LENGTH));
				strcpy((friends_arr + j*USERNAME_LENGTH), (friends_arr + (j + 1)*USERNAME_LENGTH));
				strcpy((friends_arr + (j + 1)*USERNAME_LENGTH), temp);
			}

		}
	}
	for (i = 0; i<arr_size; i++)
	{
		printf("%s\n", (friends_arr + i*USERNAME_LENGTH));
	}
}
void searchUser(char* search_string, profileN* user, profileN* head)// print users who have this string in their username
{
	int found = 0, i, is_user;
	char* exist_ptr;
	char copy_search_string[USERNAME_LENGTH];
	char temp_lowered_name[USERNAME_LENGTH];
	char *lower_string_ptr;
	char private_name[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, private_name);
	strcpy(copy_search_string, search_string);
	lowerString(copy_search_string, copy_search_string);
	while (head != NULL)
	{
		lowerString(head->profile_strct.username, temp_lowered_name);
		exist_ptr = strstr(temp_lowered_name, copy_search_string);

		is_user = strcmp(user->profile_strct.username, head->profile_strct.username);
		if ((exist_ptr != NULL) && (is_user != 0))
		{
			if (found == 0)
			{
				printf("The search for \"%s\" has found the following users within the SocioPath network:\n", search_string);
				found = 1;
			}
			printf("* %s\n", head->profile_strct.username);
		}
		head = head->next;
	}

	if (found == 0)
	{
		printf("Dear %s, no results have been found.\n", private_name);
	}
}
char* lowerString(char tolower_string[], char lowered_str[])//lowercase a string
{
	int i = 0;

	while (tolower_string[i])
	{
		lowered_str[i] = tolower(tolower_string[i]);

		i++;
	}
	lowered_str[i] = '\0';
	return lowered_str;
}


void unfriend(char* unfriend_name, profileN* user, profileN* head)//unfriend user 
{
	char* str_ptr;
	char* new_friends_str;
	int exist;
	char private_name[USERNAME_LENGTH];
	char unfriend_private_name[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, private_name);
	get_first_name(unfriend_name, unfriend_private_name);
	exist = findFriendInFriendsList(user, unfriend_name);
	new_friends_str = (char*)calloc(strlen(user->profile_strct.friends), sizeof(char));
	if (new_friends_str == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	if (exist == 0)
	{
		printf("Dear %s, you don't have any friend with the username: %s. You are being transferred to the profile menu to choose again.\n", private_name, unfriend_private_name);
	}
	else if (exist == 1)
	{
		printf("Dear %s, %s in no longer your friend.\n", private_name, unfriend_private_name);
		if (strcmp(user->profile_strct.friends, unfriend_name) == 0)//only name in list
		{
			strcpy(user->profile_strct.friends, "");
		}
		else
		{
			str_ptr = strstr(user->profile_strct.friends, unfriend_name);
			if (str_ptr == user->profile_strct.friends)//first name in list
			{
				str_ptr = str_ptr + 3 + strlen(unfriend_name);
				strcpy(user->profile_strct.friends, str_ptr);
			}
			else if (*(str_ptr + strlen(unfriend_name)) == '\0')//last name in list
			{
				str_ptr = str_ptr - 3;
				*str_ptr = '\0';
			}
			else
			{
				*str_ptr = '\0';//in the middle of the list
				new_friends_str = strcpy(new_friends_str, user->profile_strct.friends);
				str_ptr = str_ptr + 3 + strlen(unfriend_name);
				new_friends_str = strcat(new_friends_str, str_ptr);
				strcpy(user->profile_strct.friends, new_friends_str);
			}
		}
		unfriendHelper(user->profile_strct.username, findUser(unfriend_name, head), head);
	}
	free(new_friends_str);
}


void unfriendHelper(char* unfriend_name, profileN* user, profileN* head)//unfriend the unfriender from unfriended's list 
{
	char* str_ptr;
	char* new_friends_str;

	new_friends_str = (char*)calloc(strlen(user->profile_strct.friends), sizeof(char));
	if (new_friends_str == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	if (strcmp(user->profile_strct.friends, unfriend_name) == 0)//only name in list
	{
		strcpy(user->profile_strct.friends, "");
	}
	else
	{
		str_ptr = strstr(user->profile_strct.friends, unfriend_name);
		if (str_ptr == user->profile_strct.friends)//first name in list
		{
			str_ptr = str_ptr + 3 + strlen(unfriend_name);
			strcpy(user->profile_strct.friends, str_ptr);
		}
		if (*(str_ptr + strlen(unfriend_name)) == '\0')//last name in list
		{
			str_ptr = str_ptr - 3;
			*str_ptr = '\0';
		}
		else
		{
			*str_ptr = '\0';//in the middle of the list
			new_friends_str = strcpy(new_friends_str, user->profile_strct.friends);
			str_ptr = str_ptr + 3 + strlen(unfriend_name);
			new_friends_str = strcat(new_friends_str, str_ptr);
			strcpy(user->profile_strct.friends, new_friends_str);
		}
	}
	free(new_friends_str);
}


void checkRequests(profileN* user, profileN* head)//check pending requests of the user and decide what to do with them
{
	char* help_ptr;
	char* help_ptr2;
	char str_ptr[USERNAME_LENGTH + 9];
	char choose[11];
	char private_name[USERNAME_LENGTH];

	get_first_name(user->profile_strct.username, private_name);
	printf("Dear %s, you have %d friend request.\nThe pending requests are from the users:\n", private_name, howManyRequests(user->profile_strct.requestHead));
	printPendingRequests(user->profile_strct.requestHead);
	printf("To approve the request, enter \"approve::<username>\".\nTo refuse and delete the request, enter \"refuse::<username>\".\nTo go back to the main selection menu, enter \"&\".\n\nInput: ");
	gets(str_ptr);
	help_ptr2 = strstr(str_ptr, "::");

	if (strcmp(str_ptr, "&") == 0)
	{
		//return to main menu
		return;
	}

	else if (help_ptr2 == NULL)
	{
		printf("Dear %s, you have entered an illegal input.You are being transferred to the beginning of this stage in order to choose again.\n", private_name);
		checkRequests(user, head);
		return;
	}

	help_ptr2 = help_ptr2 + 2;


	if (checkLegalInputPending(user->profile_strct.requestHead, help_ptr2) == 0 || help_ptr2 == NULL)
	{
		printf("\nDear %s, no such pending friend request exists. ", private_name);
		printf("You are being transferred to the beginning of this stage in order to choose again.\n");
		checkRequests(user, head);
		return;
	}
	else
	{
		help_ptr = strstr(str_ptr, "::");
		*help_ptr = '\0';
		strcpy(choose, str_ptr);
		help_ptr += 2;
		if (strcmp(choose, "approve") == 0)
		{
			approvePending(user, help_ptr, head);
		}
		else if (strcmp(choose, "refuse") == 0)
		{
			refusePending(user, help_ptr);
		}
		else
		{
			printf("Dear %s, you have entered an illegal input.You are being transferred to the beginning of this stage in order to choose again.\n", private_name);
			checkRequests(user, head);
			return;
		}
	}
}


int howManyRequests(profile* user)//check how many pending requests
{
	int counter = 0;
	while (user != NULL)
	{
		counter++;
		user = user->requestHead;
	}
	return counter;
}
void printPendingRequests(profile* user)//print all the pending requests
{
	if (user == NULL)
	{
		return;
	}
	else
	{
		printPendingRequests(user->requestHead);
		printf("* %s\n", user->username);
	}
}
int checkLegalInputPending(profile *user, char* input)// check if the input is ok
{
	while (user != NULL)
	{
		if (strcmp(user->username, input) == 0)
			return 1;
		user = user->requestHead;
	}
	return 0;
}
void approvePending(profileN* user, char* input, profileN* head)// approve the request and update the data and file
{
	char first_name_adder[USERNAME_LENGTH];
	char first_name_added[USERNAME_LENGTH];
	profile* help_ptr = user->profile_strct.requestHead;
	profile* list = user->profile_strct.requestHead;


	if (strcmp(list->username, input) == 0)
	{
		user->profile_strct.requestHead = user->profile_strct.requestHead->requestHead;
		addFriendString(user, input);
		addFriendString(findUser(input, head), user->profile_strct.username);
		free(help_ptr);
	}
	else
	{
		list = list->requestHead;
		while (list != NULL)
		{
			if (strcmp(list->username, input) == 0)
			{
				help_ptr->requestHead = list->requestHead;
				addFriendString(user, input);
				addFriendString(findUser(input, head), user->profile_strct.username);
				free(list);
				break;
			}
			else
			{
				help_ptr = help_ptr->requestHead;
				list = list->requestHead;
			}
		}
	}

	get_first_name(user->profile_strct.username, first_name_adder);
	get_first_name(input, first_name_added);
	printf("\nDear %s, %s has been added to your friends list.\n", first_name_adder, first_name_added);

}
void refusePending(profileN* user, char* input)// refuse the request and update the data file
{
	char first_name_deleter[USERNAME_LENGTH];
	char first_name_deleted[USERNAME_LENGTH];
	profile* help_ptr = user->profile_strct.requestHead;
	profile* list = user->profile_strct.requestHead;

	if (strcmp(list->username, input) == 0)
	{
		user->profile_strct.requestHead = user->profile_strct.requestHead->requestHead;
		free(list);
	}
	else
	{
		list = list->requestHead;
		while (list != NULL)
		{
			if (strcmp(list->username, input) == 0)
			{
				help_ptr->requestHead = list->requestHead;
				free(list);
				break;
			}
			else
			{
				help_ptr = help_ptr->requestHead;
				list = list->requestHead;
			}
		}
	}

	get_first_name(user->profile_strct.username, first_name_deleter);
	get_first_name(input, first_name_deleted);
	printf("\nDear %s, %s request has been refused.\n", first_name_deleter, first_name_deleted);

}
void addFriendString(profileN* user, char* new_friend)//add new friend to the friend list of the user
{
	int mem_size = strlen(user->profile_strct.friends) + strlen(new_friend) + 3;

	user->profile_strct.friends = (char*)realloc(user->profile_strct.friends, mem_size*sizeof(char));
	if (user->profile_strct.friends == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}

	if (strlen(user->profile_strct.friends) != 0)
		strcat(user->profile_strct.friends, "_$_");
	strcat(user->profile_strct.friends, new_friend);
}
void sendFriendRequset(profileN* user, profileN* head, char* wtbfriend)// send a friend request to this user if possible
{
	char user_first[USERNAME_LENGTH];
	char wtbfriend_first[USERNAME_LENGTH];
	profileN *wtbfriend_profileN = findUser(wtbfriend, head);
	profile* request;

	get_first_name(user->profile_strct.username, user_first);
	get_first_name(wtbfriend, wtbfriend_first);
	if (wtbfriend_profileN == NULL)
	{
		printf("Dear %s, SocioPath doesn't have a member by the username: %s. You are being transferred to the profile menu to choose again.\n", user_first, wtbfriend_first);
	}
	else
	{
		if (isInPending(wtbfriend_profileN->profile_strct.requestHead, user->profile_strct.username) == 1)
		{
			printf("Dear %s, a friend request has already been sent to: %s. You are being transferred to the profile menu to choose again.\n", user_first, wtbfriend_first);
		}
		else if (isInPending(user->profile_strct.requestHead, wtbfriend) == 1)
		{
			printf("Dear %s, %s has already sent you a friend request. Please check your \"checkRequests\" menu.\n", user_first, wtbfriend_first);
		}
		else if (findFriendInFriendsList(user, wtbfriend) == 1)
		{
			printf("Dear %s, %s is already your friend. You are being transferred to the profile menu to choose again.\n", user_first, wtbfriend_first);
		}
		else
		{

			printf("Dear %s, a friend request has been sent to: %s.\n", user_first, wtbfriend_first);
			request = createProfile(user->profile_strct.username, "", "", wtbfriend_profileN->profile_strct.requestHead);
			wtbfriend_profileN->profile_strct.requestHead = request;
		}
	}
}
int isInPending(profile* user, char* adder)//check if the adder is in the user's pending request list
{
	while (user != NULL)
	{
		if (strcmp(user->username, adder) == 0)
			return 1;
		user = user->requestHead;
	}
	return 0;
}
void printNetwork(profileN* user, profileN* head)// print the social network of the user
{
	profile* list_of_friends;
	profileN* orig_head_copy_list;
	profileN* head_copy_list = NULL;
	char* user_copied_friends, orig_user_copied_friends;
	int degree = 0;
	char* temp_print_line, *temp_print_line_helper = NULL;
	int  check = 1, size;

	temp_print_line = (char*)calloc(USERNAME_LENGTH + 1, sizeof(char));
	if (temp_print_line == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	temp_print_line[0] = '\0';

	size = strlen(user->profile_strct.friends) +1;
	user_copied_friends = (char*)calloc(size, sizeof(char));// SORTING FRIENDS BY DEGREE
	if (user_copied_friends == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	strcpy(user_copied_friends, user->profile_strct.friends);

	head_copy_list = copyUsersList(head, head_copy_list);
	orig_head_copy_list = head_copy_list;

	orig_user_copied_friends = user_copied_friends;

	list_of_friends = makeListOfFriends(user_copied_friends, head);
	user = findUser(user->profile_strct.username, head_copy_list);
	strcpy(user->profile_strct.status, "0");

	recursiveNetwork(degree + 1, head_copy_list, list_of_friends, head);

	free(user_copied_friends);
	printf("Dear %s, your social network is:\n", user->profile_strct.username);// PRINTING
	printf("You: %s\n", user->profile_strct.username);
	
	if(existingNetwork(head_copy_list, 1)==1)//111111111111111111111111111111
		printf("Your friends: ");
	while (head_copy_list != NULL)
	{
		if (atoi(head_copy_list->profile_strct.status) == 1)
		{
			temp_print_line_helper = (char*)calloc(strlen(temp_print_line) + 3, sizeof(char));
			if (temp_print_line_helper == NULL)
			{
				printf("failed allocation Memory!\n");
				exit(1);
			}
			strcpy(temp_print_line_helper, temp_print_line);
			free(temp_print_line);
			temp_print_line = (char*)calloc(strlen(temp_print_line_helper) + strlen(head_copy_list->profile_strct.username) + 3, sizeof(char));
			if (temp_print_line == NULL)
			{
				printf("failed allocation Memory!\n");
				exit(1);
			}
			strcpy(temp_print_line, temp_print_line_helper);
			free(temp_print_line_helper);
			temp_print_line = strcat(temp_print_line, head_copy_list->profile_strct.username);
			temp_print_line = strcat(temp_print_line, ", ");
		}
		head_copy_list = head_copy_list->next;
	}
	temp_print_line[strlen(temp_print_line) - 2] = '\0';
	printf("%s\n", temp_print_line);

	head_copy_list = orig_head_copy_list;//222222222222222222222222222

	//free(temp_print_line);
	temp_print_line = (char*)calloc(USERNAME_LENGTH +1, sizeof(char));
	if (temp_print_line == NULL)
	{
		printf("failed allocation Memory!\n");
		exit(1);
	}
	temp_print_line[0] = '\0';
	if(existingNetwork(head_copy_list, 2)==1)
		printf("Friends of your friends: ");
	while (head_copy_list != NULL)
	{
		if (atoi(head_copy_list->profile_strct.status) == 2)
		{
			temp_print_line_helper = (char*)calloc(strlen(temp_print_line) + 3, sizeof(char));
			if (temp_print_line_helper == NULL)
			{
				printf("failed allocation Memory!\n");
				exit(1);
			}
			strcpy(temp_print_line_helper, temp_print_line);
			free(temp_print_line);
			temp_print_line = (char*)calloc(strlen(temp_print_line_helper) + strlen(head_copy_list->profile_strct.username) + 3, sizeof(char));
			if (temp_print_line == NULL)
			{
				printf("failed allocation Memory!\n");
				exit(1);
			}
			strcpy(temp_print_line, temp_print_line_helper);
			free(temp_print_line_helper);
			temp_print_line = strcat(temp_print_line, head_copy_list->profile_strct.username);
			temp_print_line = strcat(temp_print_line, ", ");
		}
		head_copy_list = head_copy_list->next;
	}
	temp_print_line[strlen(temp_print_line) - 2] = '\0';
	printf("%s\n", temp_print_line);


	degree = 3;//33333333333++++++++++++++
	while (check != 0)
	{
		check = 0;
		head_copy_list = orig_head_copy_list;
		//free(temp_print_line);
		temp_print_line = (char*)calloc(USERNAME_LENGTH +1, sizeof(char));
		if (temp_print_line == NULL)
		{
			printf("failed allocation Memory!\n");
			exit(1);
		}
		temp_print_line[0] = '\0';

		while (head_copy_list != NULL)
		{
			if (atoi(head_copy_list->profile_strct.status) == degree)
			{
				check = 1;
				temp_print_line_helper = (char*)calloc(strlen(temp_print_line) + 3, sizeof(char));
				if (temp_print_line_helper == NULL)
				{
					printf("failed allocation Memory!\n");
					exit(1);
				}
				strcpy(temp_print_line_helper, temp_print_line);
				free(temp_print_line);
				temp_print_line = (char*)calloc(strlen(temp_print_line_helper) + strlen(head_copy_list->profile_strct.username) + 3, sizeof(char));
				if (temp_print_line == NULL)
				{
					printf("failed allocation Memory!\n");
					exit(1);
				}
				strcpy(temp_print_line, temp_print_line_helper);
				free(temp_print_line_helper);
				temp_print_line = strcat(temp_print_line, head_copy_list->profile_strct.username);
				temp_print_line = strcat(temp_print_line, ", ");
			}
			head_copy_list = head_copy_list->next;
		}

		if (strcmp(temp_print_line, "") != 0)
		{
			printf("%drd circle friends: ", degree);
			temp_print_line[strlen(temp_print_line) - 2] = '\0';
			printf("%s\n", temp_print_line);
		}
		degree++;
	}
	free(temp_print_line);

}


profileN* copyUsersList(profileN* head, profileN* copy_head)//create new list only with names, status as -1 and friends list
{
	char status_minus[STATUS_LENGTH] = "-1";

	if (head == NULL)
	{
		return NULL;
	}
	copy_head = createProfileNode(head->profile_strct.username, status_minus, head->profile_strct.friends, NULL, copyUsersList(head->next, copy_head));
	return copy_head;

}
void recursiveNetwork(int degree, profileN* list, profile* check_deg_friends_head, profileN* head)//state the friend degree of every user
{
	profileN* curr_n = NULL, *condition;
	char str_deg[STATUS_LENGTH], *friends_list_temp;
	profile* orig_deg_friends_head, *marked_deg_friends_head, *help_ptr;
	int one, two, size;

	orig_deg_friends_head = check_deg_friends_head;
	itoa(degree, str_deg, 10);
	while (check_deg_friends_head != NULL)
	{

		curr_n = findUser(check_deg_friends_head->username, list);
		if (strcmp(curr_n->profile_strct.status, "-1") == 0)
		{
			strcpy(curr_n->profile_strct.status, str_deg);
			strcpy(check_deg_friends_head->status, "1");
		}
		check_deg_friends_head = check_deg_friends_head->requestHead;
	}

	check_deg_friends_head = orig_deg_friends_head;

	while (check_deg_friends_head != NULL)
	{

		if (strcmp(check_deg_friends_head->status, "1") == 0)
		{
			size = strlen(check_deg_friends_head->friends) + 1;
			friends_list_temp = (char*)calloc(size, sizeof(char));
			if (friends_list_temp == NULL)
			{
				printf("failed allocation Memory!\n");
				exit(1);
			}
			strcpy(friends_list_temp, check_deg_friends_head->friends);
			recursiveNetwork(degree + 1, list, makeListOfFriends(friends_list_temp, head), head);
			free(friends_list_temp);
		}

		help_ptr = check_deg_friends_head;
		check_deg_friends_head = check_deg_friends_head->requestHead;
		//free(help_ptr->friends);
		free(help_ptr);
	}
}


profile* makeListOfFriends(char* str_friends, profileN* head)// make list of al the users in the friends string- 0 for status friends str in friends
{
	profileN* tempN = NULL;
	profile* list_head = NULL, *orig_list_head;
	char* str_ptr;

	str_ptr = strstr(str_friends, "_$_");
	if (strcmp(str_friends, "") == 0)
		return NULL;
	if (str_ptr == NULL)
	{
		tempN = findUser(str_friends, head);
		list_head = createProfile(tempN->profile_strct.username, "0", tempN->profile_strct.friends, NULL);
		return list_head;
	}
	else
	{
		*str_ptr = '\0';
		tempN = findUser(str_friends, head);
		list_head = createProfile(tempN->profile_strct.username, "0", tempN->profile_strct.friends, NULL);
		orig_list_head = list_head;
		str_friends = str_ptr + 3;
		str_ptr = strstr(str_friends, "_$_");

		while (str_ptr != NULL)
		{
			*str_ptr = '\0';

			tempN = findUser(str_friends, head);
			list_head->requestHead = createProfile(tempN->profile_strct.username, "0", tempN->profile_strct.friends, NULL);
			list_head = list_head->requestHead;
			str_friends = str_ptr + 3;
			str_ptr = strstr(str_friends, "_$_");
		}
		tempN = findUser(str_friends, head);
		list_head->requestHead = createProfile(tempN->profile_strct.username, "0", tempN->profile_strct.friends, NULL);
		return orig_list_head;
	}
}

int existingNetwork(profileN* head, int degree)//return 1 if the there are users in this degree in the list 0 otherwise
{
	profileN* copy_head=head;
	char degString[6]={0};

	itoa(degree, degString, 10);
	while(copy_head!=NULL)
	{
		if (strcmp(copy_head->profile_strct.status, degString) == 0)
			return 1;
		copy_head=copy_head->next;
	}
	return 0;
}



void uploadData(FILE *fPtr, profileN* original_head)
{

	//prints to the file
	profileN* head_helper = original_head;

	while (head_helper != NULL)
	{
		if (head_helper->next == NULL)
		{
			fprintf(fPtr, "Username_%s\n", head_helper->profile_strct.username);
			fprintf(fPtr, "Status_%s\n", head_helper->profile_strct.status);
			fprintf(fPtr, "Friends_%s\n", head_helper->profile_strct.friends);
			free(head_helper->profile_strct.friends);
			fprintf(fPtr, uploadData_help(head_helper->profile_strct.requestHead));
			break;
		}
		fprintf(fPtr, "Username_%s\n", head_helper->profile_strct.username);
		fprintf(fPtr, "Status_%s\n", head_helper->profile_strct.status);
		fprintf(fPtr, "Friends_%s\n", head_helper->profile_strct.friends);
		fprintf(fPtr, uploadData_help(head_helper->profile_strct.requestHead));
		free(head_helper->profile_strct.friends);//
		fprintf(fPtr, "\n");
		head_helper = head_helper->next;
	}
}



void uploadDataNewUser(FILE *fPtr, profileN* original_head)
{

	//prints to the file
	profileN* head_helper = original_head;

	fprintf(fPtr, "Username_%s\n", head_helper->profile_strct.username);
	fprintf(fPtr, "Status_%s\n", head_helper->profile_strct.status);
	fprintf(fPtr, "Friends_%s\n", head_helper->profile_strct.friends);
	fprintf(fPtr, uploadData_help(head_helper->profile_strct.requestHead));

	if(numOfUsers(head_helper)>1)
		fprintf(fPtr, "\n");
	head_helper = head_helper->next;
	while (head_helper != NULL)
	{
		if (head_helper->next == NULL)
		{
			fprintf(fPtr, "Username_%s\n", head_helper->profile_strct.username);
			fprintf(fPtr, "Status_%s\n", head_helper->profile_strct.status);
			fprintf(fPtr, "Friends_%s\n", head_helper->profile_strct.friends);
			if (head_helper->profile_strct.friends != 0  && *head_helper->profile_strct.friends != 0)
			free(head_helper->profile_strct.friends);
			fprintf(fPtr, uploadData_help(head_helper->profile_strct.requestHead));
			break;
		}
		fprintf(fPtr, "Username_%s\n", head_helper->profile_strct.username);
		fprintf(fPtr, "Status_%s\n", head_helper->profile_strct.status);
		fprintf(fPtr, "Friends_%s\n", head_helper->profile_strct.friends);
		fprintf(fPtr, uploadData_help(head_helper->profile_strct.requestHead));
		free(head_helper->profile_strct.friends);
		fprintf(fPtr, "\n");
		head_helper = head_helper->next;
	}
}


char* uploadData_help(profile* pending_head)
{

	// the pending

	int size_of_name, length;
	char *pending;
	profile* pending_helper = pending_head;

	size_of_name = USERNAME_LENGTH + 4;
	length = howManyRequests(pending_head)*size_of_name;
	pending = (char*)malloc(length*sizeof(char));
	if (pending == NULL)
	{
		printf("Fatal error: memory allocation failed!\n");
		exit(1);
	}
	strcpy(pending, "Pending_");

	if (pending_helper != NULL)
	{

		while (pending_helper->requestHead != NULL)
		{
			strcat(pending, pending_helper->username);
			strcat(pending, "_$_");
			pending_helper = pending_helper->requestHead;
		}
		strcat(pending, pending_helper->username);
	}
	return pending;

}


void free_profileNode_list(profileN* head)
{
	/*
	free the profileNode list
	*/
	profileN* temp;

	while (head != NULL)
	{
		temp = head;
		free_profile_list(temp->profile_strct.requestHead);
		head = head->next;
		free(temp);
	}
}

void free_profile_list(profile* head)
{

	//free the profile list


	profile* temp;

	while (head != NULL)
	{
		temp = head;
		head = head->requestHead;
		free(temp);
	}
}


int main(int argc, char* argv[])
{

	FILE *fPtrRead1, *fPtrRead2;
	valid_user *users_list = NULL;
	profileN* headN = NULL;
	char line[MAX_LINE_LENGTH];

	fPtrRead1 = fopen(argv[1], "r");
	if (fPtrRead1 == NULL)
	{
		printf("There was error using files\n");
		exit(1);
	}

	users_list = readFromFile(fPtrRead1, users_list);
	fclose(fPtrRead1);
	login_menu(users_list, argv[1], argv[2]);



}







