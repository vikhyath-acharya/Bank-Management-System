#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account
{
    int accNo;
    char name[50];
    float balance;
};

void createAccount();
void displayAccounts();
void searchAccount();
void depositMoney();
void withdrawMoney();
void deleteAccount();

int main()
{
    int choice;

    while (1)
    {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                createAccount();
                break;

            case 2:
                displayAccounts();
                break;

            case 3:
                searchAccount();
                break;

            case 4:
                depositMoney();
                break;

            case 5:
                withdrawMoney();
                break;

            case 6:
                deleteAccount();
                break;

            case 7:
                printf("Thank You!\n");
                exit(0);

            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}

void createAccount()
{
    FILE *fp;
    struct Account a;

    fp = fopen("bank.dat", "ab");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &a.accNo);

    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", a.name);

    printf("Enter Initial Balance: ");
    scanf("%f", &a.balance);

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("Account Created Successfully!\n");
}

void displayAccounts()
{
    FILE *fp;
    struct Account a;

    fp = fopen("bank.dat", "rb");

    if (fp == NULL)
    {
        printf("No Records Found!\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    while (fread(&a, sizeof(a), 1, fp))
    {
        printf("\nAccount Number : %d", a.accNo);
        printf("\nName           : %s", a.name);
        printf("\nBalance        : %.2f\n", a.balance);
    }

    fclose(fp);
}

void searchAccount()
{
    FILE *fp;
    struct Account a;
    int acc, found = 0;

    fp = fopen("bank.dat", "rb");

    if (fp == NULL)
    {
        printf("No Records Found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            printf("\nAccount Found\n");
            printf("Account Number : %d\n", a.accNo);
            printf("Name           : %s\n", a.name);
            printf("Balance        : %.2f\n", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account Not Found!\n");

    fclose(fp);
}

void depositMoney()
{
    FILE *fp;
    struct Account a;
    int acc, found = 0;
    float amount;

    fp = fopen("bank.dat", "rb+");

    if (fp == NULL)
    {
        printf("No Records Found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            a.balance += amount;

            fseek(fp, -sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            printf("Deposit Successful!\n");
            printf("Updated Balance: %.2f\n", a.balance);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account Not Found!\n");

    fclose(fp);
}

void withdrawMoney()
{
    FILE *fp;
    struct Account a;
    int acc, found = 0;
    float amount;

    fp = fopen("bank.dat", "rb+");

    if (fp == NULL)
    {
        printf("No Records Found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount > a.balance)
            {
                printf("Insufficient Balance!\n");
            }
            else
            {
                a.balance -= amount;

                fseek(fp, -sizeof(a), SEEK_CUR);
                fwrite(&a, sizeof(a), 1, fp);

                printf("Withdrawal Successful!\n");
                printf("Remaining Balance: %.2f\n", a.balance);
            }

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account Not Found!\n");

    fclose(fp);
}

void deleteAccount()
{
    FILE *fp, *temp;
    struct Account a;
    int acc, found = 0;

    fp = fopen("bank.dat", "rb");

    if (fp == NULL)
    {
        printf("No Records Found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");

    printf("Enter Account Number to Delete: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            found = 1;
            continue;
        }

        fwrite(&a, sizeof(a), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        printf("Account Deleted Successfully!\n");
    else
        printf("Account Not Found!\n");
}