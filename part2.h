#ifndef PART2_H
#define PART2_H
#include "module1.h"

#define STATUS_LENGTH 513
#define USERNAME_LENGTH 53
#define INPUT_LENGTH 14+USERNAME_LENGTH


typedef struct profileStruct
{
	char username[USERNAME_LENGTH];
	char status[STATUS_LENGTH];
	char* friends;
	struct profileStruct *requestHead;
}profile;

typedef struct profileNode
{
	struct profileStruct profile_strct;
	struct profileNode *next;
}profileN;




profileN* createDB(FILE* fPtrRead, profileN* headN, valid_user* val_head);
profile* createProfile(char* username, char* status, char* friends, profile* pending_head);// the arguments dont printf good
profileN* createProfileNode(char* username, char* status, char* friends, profile* pending_head, profileN* next_profilen);
profileN* findUser(char* username, profileN *head);
int numOfUsers(profileN *head);
profile* createLinkedPendingList(char* pending_str, profileN* head);
void loginProfile(char* username, profileN* head, char* argi, char* argi2, valid_user* val_head, int is_new);
void printStatuses(profileN* user, profileN* original_head);
void takeInput(profileN* user, profileN* original_head, valid_user* valid_list, char* argi, char* argi2, int is_new);
void printOptions();
void printProfile(profileN* user, profileN* original_head);
void updateStatus(profileN* user);
void checkUserStatus(profileN* user, char* friend_username, profileN* head);
int findFriendInFriendsList(profileN* user, char* friend_username);
void printFriendsList(profileN* user, profileN* head);
int howManyFriends(char* friends_str);
void sortAndPrint(char* friends_arr, int arr_size);
void searchUser(char* search_string, profileN* user, profileN* head);
char* lowerString(char tolower_string[], char lowered_str[]);
void unfriend(char* unfriend_name, profileN* user, profileN* head);
void unfriendHelper(char* unfriend_name, profileN* user, profileN* head);
void checkRequests(profileN* user, profileN* head);
int howManyRequests(profile *user);
void printPendingRequests(profile *user);
int checkLegalInputPending(profile *user, char* input);
void approvePending(profileN* user, char* input, profileN* head);
void refusePending(profileN* user, char* input);
void addFriendString(profileN* user, char* new_friend);
void sendFriendRequset(profileN* user, profileN* head, char* wtbfriend);
int isInPending(profile* user, char* adder);
void printNetwork(profileN* user, profileN* head);
profileN* copyUsersList(profileN* head, profileN* copy_head);
void recursiveNetwork(int degree, profileN* list, profile* check_deg_friends_head, profileN* head);
profile* makeListOfFriends(char* str_friends, profileN* head);
int existingNetwork(profileN* head, int degree);

void uploadData(FILE *fPtr, profileN* original_head);
void uploadDataNewUser(FILE *fPtr, profileN* original_head);
char* uploadData_help(profile* pending_head);
void free_profileNode_list(profileN* head);
void free_profile_list(profile* head);

#endif

