#ifndef SHARED3_H
#define SHARED3_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>


int is_legal_name(char* name);
int is_legal_password(char* password);
int is_legal_answer(char* answer);
void get_first_name(char* name, char* first_name);

#endif