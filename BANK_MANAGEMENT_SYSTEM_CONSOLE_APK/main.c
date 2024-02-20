#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "user_acc.c"
#include "user_creden.c"
#include "declare.h"

int main()
{
    int choice;
    user_t user[50];
    do
    {
        printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
        printf("Enter your choice: \n");
        printf("0. Exit\n1. Sign In\n2. Sign up\nChoice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1 :
            sign_in(user);
            break;
        
        case 2 :
            sign_up(user);
            break;
        
        default :
            break;
        }
    }while (choice != 0);
    
}