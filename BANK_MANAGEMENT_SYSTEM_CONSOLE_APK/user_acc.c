#include "declare.h"

int emp_login(user_t user)
{
    
    int choice;
    do
    {
        printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
        printf("Enter Your Choice: \n");
        printf("0. Exit\n1. Add New Customer\n2. Deposit\n3. Withdraw\n4. Transaction History\n5. Search Customer\n6. Display All customer info\n7. Close Customer Account\n8. My Profile\nChoice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 0:
                break;
            case 1:
                add_new_customer();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                transaction_history();
                break;
            case 5:
                search_customer();
                break;
            case 6:
                display_customer_info();
                break;
            case 7:
                close_customer_acc();
                break;
            case 8:
                my_profile(user);
                return 0;
        }
    } while (choice != 0);
    
}

int count_customer()
{
    int count = 0;
    customer_t buffer;
    FILE* fp;
    fp = fopen("customer_info.bin", "rb");
    if(fp == NULL)
        return 0;
    else
    {
        while((fread(&buffer, sizeof(customer_t),1, fp)) != 0)
        {
            count++;
        }
    }
    return count;
}

int is_delete_avail()
{
    int ret_id;
    acc_t temp;
    FILE* fp = fopen("deleted_id.bin", "rb");
    if(fp == NULL)
        return 0;

    while(fread(&temp, sizeof(acc_t), 1,fp) != 0)
    {
        if(temp.customer_id != 0)
        {
            ret_id = temp.customer_id;
            fclose(fp);
            goto LABEL;
        }
    }

    return 0;

LABEL:
    fp = fopen("deleted_id.bin", "rb");
    FILE* np = fopen("new_deleted_id.bin", "wb");
    while(fread(&temp, sizeof(acc_t), 1,fp) != 0)
    {
        if(temp.customer_id == ret_id)
        {
            //do nothing
        }
        else
        {
            fwrite(&temp, sizeof(acc_t), 1, np);
        }
    }
    fclose(fp);
    fclose(np);

    fp = fopen("deleted_id.bin", "wb");
    np = fopen("new_deleted_id.bin", "rb");
    while(fread(&temp, sizeof(acc_t), 1,np) != 0)
    {
        fwrite(&temp, sizeof(acc_t), 1, fp);
    }
    fclose(fp);
    fclose(np);
    return ret_id;
}

void add_new_customer()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    time_t now;
    char* time_now;
    time(&now);
    time_now = ctime(&now);
    t_history_t transaction;
      
    int count;
    int ret;
    FILE* fp;
    customer_t person;
    count = is_delete_avail();
    if(count != 0)
    {
        person.customer_id = count;    
    }
    
    else
    {
        count = count_customer();
        person.customer_id = count+1;
    }
    
    printf("Enter the name of customer: ");
    scanf("%*c%[^\n]s", person.name);
    printf("Enter the account type of customer(saving/current) : ");
    scanf("%*c%[^\n]s", person.acc_type);
    printf("Enter the opening balance of customer: ");
    scanf("%d", &person.balance);
    fp = fopen("customer_info.bin", "ab+");
    ret = fwrite(&person, sizeof(customer_t), 1, fp);

    FILE* np = fopen("transaction_history.bin", "ab+");
    transaction.customer_id = person.customer_id;
    transaction.transaction_id = rand();
    transaction.balance = person.balance;
    strcpy(transaction.operation, "Account Created");
    strcpy(transaction.timestamp, time_now);
    ret = fwrite(&transaction, sizeof(t_history_t), 1, np);
    fclose(np); 
    fclose(fp);
    printf("Customer account is opened succesfully with customer id :%d\n", person.customer_id);
   
}

void display_customer_info()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    customer_t person;
    FILE* fp = fopen("customer_info.bin", "rb");
    while(fread(&person, sizeof(customer_t), 1, fp) != 0)
    {
        printf("customer id: %d\n", person.customer_id);
        printf("Customer Name: %s\n", person.name);
        printf("Customer acc type: %s\n", person.acc_type);
        printf("Customer Account balance: %d\n", person.balance);
        printf("------------------------------------------------\n");
    }
    fclose(fp);
}

int deposit()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    customer_t person;
    int id, deposit, ret;
    time_t now;
    time(&now);
    char* time_now;
    time_now = ctime(&now);
    t_history_t transaction;
    printf("Enter your customer id : ");
    scanf("%d", &id);
    printf("Enter value of money for deposit: ");
    scanf("%d", &deposit);
    FILE* fp = fopen("customer_info.bin", "rb+");
    while(fread(&person, sizeof(customer_t), 1, fp) != 0)
    {   
        if(person.customer_id == id)
        {
           
           person.balance = deposit + person.balance;
           printf("Your money is deposited succesfully... :)\n");
           printf("Your current balance is : %d\n", person.balance); 
           fseek(fp, -sizeof(customer_t), SEEK_CUR);
           ret = fwrite(&person, sizeof(customer_t), 1, fp);
           FILE* np = fopen("transaction_history.bin", "ab+");
           transaction.customer_id = person.customer_id;
           transaction.balance = person.balance;
           transaction.transaction_id = rand();
           strcpy(transaction.operation, "Deposit");
           strcpy(transaction.timestamp, time_now);
           ret = fwrite(&transaction, sizeof(t_history_t), 1, np);
           fclose(np); 
           printf("------------------------------------------------\n");
           fclose(fp);
           return 0;
        }
    }
    printf("Oops! Invalid customer id :( \n");
    printf("------------------------------------------------\n");
    fclose(fp);
    return -1;
}

int withdraw()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    customer_t person;
    time_t now;
    char* time_now;
    time(&now);
    time_now = ctime(&now);
    t_history_t transaction;
    strcpy(transaction.timestamp, time_now);

    int id, withdraw, ret;
    printf("Enter your customer id : ");
    scanf("%d", &id);
    printf("Enter value of money for withdraw: ");
    scanf("%d", &withdraw);
    FILE* fp = fopen("customer_info.bin", "rb+");
    while(fread(&person, sizeof(customer_t), 1, fp) != 0)
    {
        if(person.customer_id == id)
        {
           if(person.balance >= withdraw)
           {
                person.balance = person.balance - withdraw;
                fseek(fp, -sizeof(customer_t), SEEK_CUR);
                ret = fwrite(&person, sizeof(customer_t), 1, fp);

                FILE* np = fopen("transaction_history.bin", "ab+");
                transaction.customer_id = person.customer_id;
                transaction.balance = person.balance;
                transaction.transaction_id = rand();
                strcpy(transaction.operation, "Withdraw");
                // strcpy(transaction.timestamp, time_now);
                ret = fwrite(&transaction, sizeof(t_history_t), 1, np);
                fclose(np); 
                fclose(fp);
                printf("Your money is withdraw succesfully... :)\n");
                printf("Your current balance is : %d\n", person.balance); 
                printf("------------------------------------------------\n");
                return 0;
           }
           else
           {
                printf("Insufficient balance...!\n");
                printf("------------------------------------------------\n");
                return -1;
           }
        }
    }
    printf("Oops! Invalid customer id :( \n");
    printf("------------------------------------------------\n");
    return -1;
}

int search_customer()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    int id;
    customer_t person;
    printf("Enter customer id to search customer: ");
    scanf("%d", &id);
    FILE* fp = fopen("customer_info.bin", "rb");
    while(fread(&person, sizeof(customer_t), 1, fp) != 0)
    {
        if(person.customer_id == id)
        {
            printf("Customer ID: %d\n", person.customer_id);
            printf("Customer Name: %s\n", person.name);
            printf("Customer Acc Type: %s\n", person.acc_type);
            printf("Customer Account Balance: %d\n", person.balance);
            printf("------------------------------------------------\n");
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    printf("Oops! Invalid customer id :( \n");
    printf("------------------------------------------------\n");
    return -1;
}

int close_customer_acc()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    int id, ret, i=0;
    customer_t person, temp[50];
    t_history_t transaction;
    printf("Enter the customer id to delete acc: ");
    scanf("%d", &id);
    FILE* ab = fopen("transaction_history.bin", "rb");
    FILE* cd = fopen("new_transaction_history.bin", "wb");
    while(fread(&transaction, sizeof(t_history_t), 1, ab) != 0)
    {
        if(id != transaction.customer_id)
        {
            fwrite(&transaction, sizeof(t_history_t), 1, cd);
        }
        
    }
    fclose(ab);
    fclose(cd);

    cd = fopen("new_transaction_history.bin", "rb");
    ab = fopen("transaction_history.bin", "wb");
    while(fread(&transaction, sizeof(t_history_t), 1, cd) != 0)
    {
        fwrite(&transaction, sizeof(t_history_t), 1, ab);
    }
    fclose(ab);
    fclose(cd);

    FILE* fp = fopen("customer_info.bin", "rb");
    FILE* op = fopen("new_customer_info.bin", "wb");
    while(fread(&person, sizeof(customer_t), 1, fp) != 0)
    {
        if(person.customer_id == id)
        {
            //do nothing
            acc_t del_acc;
            del_acc.customer_id = person.customer_id;
            FILE* np = fopen("deleted_id.bin", "ab+");
            ret = fwrite(&del_acc, sizeof(acc_t), 1, np);
            fclose(np);
        }
        else
        {
            
            ret = fwrite(&person, sizeof(customer_t), 1, op);
                  
        }
    }
    fclose(fp);
    fclose(op);
    fp = fopen("customer_info.bin", "wb");
    op = fopen("new_customer_info.bin", "rb");
    while(fread(&person, sizeof(customer_t), 1, op) != 0)
    {
        ret = fwrite(&person, sizeof(customer_t), 1, fp);
    }
    fclose(fp);
    fclose(op);
    
    return 0;
}

void transaction_history()
{
    printf("~~~~~~~~~~~~~~~~~ BANK Management System ~~~~~~~~~~~~~~~~~\n");
    int id;
    t_history_t transaction;
    printf("Enter the customer id to see transaction history: ");
    scanf("%d", &id);
    FILE* fp = fopen("transaction_history.bin", "rb");

    while(fread(&transaction, sizeof(t_history_t), 1, fp) != 0)
    {
        if(id == transaction.customer_id)
        {
            printf("customer id: %-6dTransaction id: %-6dOperation: %-20sTime:%-4sAcc Balance:%-4d\n",transaction.customer_id,transaction.transaction_id, transaction.operation, transaction.timestamp, transaction.balance);
            printf("---------------------------------------------------------------------------------\n");
        }
    }
    fclose(fp);   
}