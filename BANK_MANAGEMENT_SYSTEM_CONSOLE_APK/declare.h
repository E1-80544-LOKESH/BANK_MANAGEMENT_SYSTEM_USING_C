#ifndef _DECLARE_H
#define _DECLARE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
// #include<errors.h>


typedef struct user_credential
{
    int emp_id;
    char f_name[20];
    char l_name[20];
    char desgn[20];
    char email[20];
    char pass[20];

}user_t;

typedef struct new_customer
{
    int customer_id;
    char name[50];
    char acc_type[20];
    int balance;
}customer_t;

typedef struct deleted_acc
{
    int customer_id;
}acc_t;

typedef struct transaction_history
{
    int customer_id;
    int transaction_id;
    char timestamp[30];
    char operation[20];
    int balance;
}t_history_t;

void transaction_history();

void sign_up(user_t *user);

void sign_in(user_t *user);

int count_user();

int emp_login(user_t user);

int count_customer();

int is_delete_avail();

void add_new_customer();

void display_customer_info();

int deposit();

int withdraw();

int search_customer();

int close_customer_acc();

void my_profile(user_t user);
#endif