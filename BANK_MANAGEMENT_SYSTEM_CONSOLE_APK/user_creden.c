#include "declare.h"

int count_user()
{
    int count = 0;
    user_t buffer;
    FILE* fp;
    fp = fopen("user_info.bin", "rb");
    if(fp == NULL)
        return 0;
    else
    {
        while((fread(&buffer, sizeof(user_t),1, fp)) != 0)
        {
            count++;
        }
    }
    return count;
}


void sign_up(user_t *user)
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    int ret;
    int count;
    // char buffer[100];
    FILE* fp;    
    count = count_user();
    if(count == 0)
        user[count].emp_id =1;
    
    else
        user[count].emp_id = count + 1;

    printf("Enter Your first name : ");
    scanf("%*c%[^\n]s", user[count].f_name);
    printf("Enter Your last name : ");
    scanf("%*c%[^\n]s", user[count].l_name);
    printf("Enter Your designation at bank: ");
    scanf("%*c%[^\n]s",user[count].desgn);
    printf("Enter the email: ");
    scanf("%*c%[^\n]s", user[count].email);
    printf("Enter the password: ");
    scanf("%*c%[^\n]s", user[count].pass);
    fp = fopen("user_info.bin", "ab+");
    if(fp==NULL)
    {
        printf("open file failed\n");
        exit(1);
    }

    ret = fwrite(&user[count],sizeof(user_t), 1, fp);
    fclose(fp);
    printf("Your Login is created sucessfully\n");
        
}

void sign_in(user_t *user)
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    int id;
    int count=0;
    char pass[20];
    char email[20];
    user_t temp;
    FILE* fp;
    printf("Email : ");
    scanf("%*c%[^\n]s", email);
    printf("pass : ");
    scanf("%*c%[^\n]s", pass);
    fp = fopen("user_info.bin", "ab+");
    if(fp == NULL)
    {
        printf("open failed\n");
    }

    else
    {
        while(fread(&temp, sizeof(user_t), 1, fp)!= 0)
        {
            if(strcmp(temp.email, email) == 0 && strcmp(temp.pass, pass) == 0)
            {
                printf("Congrats :) !\nLogin credentials matched with id : %d\n", temp.emp_id);
                emp_login(temp);
            }
            else
            {
                printf("Invalid Credentials\n");
            }            
        }
        
        
    }

}


void my_profile(user_t user)
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    user_t temp;
    FILE* fp;
    int choice;
    char ch;
    char f_name[20];
    char l_name[20];
    char email[20];
    char desgn[20];
    char old_pass[20];
    char new_pass[20];
    printf("Enter Your Choice:\n");
    printf("0. Exit\n1. My Info\n2. Edit Profile\n3. Change Password\nChoice: ");
    scanf("%d", &choice);
    switch(choice)
    {
        case 0:
            break;
        case 1:
            printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
            printf("----------------------- MY INFO --------------------------\n");
            fp = fopen("user_info.bin", "rb");
            while(fread(&temp, sizeof(user_t), 1, fp) != 0)
            {
                if(temp.emp_id == user.emp_id)
                {
                    printf("First Name: %s\nLast Name: %s\nEmployee ID: %d\nDesignation: %s\nEmail: %s\n", temp.f_name, temp.l_name, temp.emp_id,temp.desgn, temp.email);
                    fclose(fp);
                    break;
                }
            }
            fclose(fp);
            break;
        case 2:
            printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
            printf("--------------------- EDIT PROFILE -----------------------\n");
            fp = fopen("user_info.bin", "rb+");
            while(fread(&temp, sizeof(user_t), 1, fp) != 0)
            {
                if(temp.emp_id == user.emp_id)
                {
                    printf("Do you want to change First name(Y/N) : ");
                    scanf("%*c%[^\n]c", &ch);
                    if(ch=='Y' || ch =='y')
                    {
                        printf("Enter new first name: ");
                        scanf("%*c%[^\n]s", f_name);
                        strcpy(temp.f_name, f_name);
                    }
                    
                    printf("Do you want to change Last name(Y/N) : ");
                    scanf("%*c%[^\n]c", &ch);
                    if(ch=='Y' || ch =='y')
                    {
                        printf("Enter new last name: ");
                        scanf("%*c%[^\n]s", l_name);
                        strcpy(temp.l_name, l_name);  
                    }

                    printf("Do you want to change Designation(Y/N) : ");
                    scanf("%*c%[^\n]c", &ch);
                    if(ch=='Y' || ch =='y')
                    {
                        printf("Enter new designation at your bank: ");
                        scanf("%*c%[^\n]s", desgn);
                        strcpy(temp.desgn, desgn);
                    }

                    printf("Do you want to change Email(Y/N) : ");
                    scanf("%*c%[^\n]c", &ch);
                    if(ch=='Y' || ch =='y')
                    {
                        printf("Enter new email: ");
                        scanf("%*c%[^\n]s", email);
                        strcpy(temp.email, email);
                    }

                    fseek(fp, -sizeof(user_t), SEEK_CUR);
                    fwrite(&temp, sizeof(user_t), 1, fp);
                    fclose(fp);
                    printf("Your Information is Updated Succesfully\n");
                }
            }
            break;

        case 3:
            printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
            printf("--------------------- CHANGE PASSWORD -----------------------\n");
            printf("enter old password: ");
            scanf("%*c%[^\n]s", old_pass);
            printf("enter new password: ");
            scanf("%*c%[^\n]s", new_pass);
            fp=fopen("user_info.bin", "rb+");
            while(fread(&temp, sizeof(user_t), 1, fp) != 0)
            {
                if(strcmp(temp.pass, old_pass)==0)
                {
                    strcpy(temp.pass, new_pass);
                    fseek(fp, -sizeof(user_t), SEEK_CUR);
                    fwrite(&temp, sizeof(user_t), 1, fp);
                    fclose(fp);
                    printf("Your Password is changed succesfully....!\n");
                    goto NEXT;
                }
            }
            printf("Old password is incorrect :( \n");
        NEXT:
            break;
    }
    

}