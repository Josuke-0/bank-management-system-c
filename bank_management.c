#include <stdio.h>
#include <string.h>
#include <time.h>

//functions which will be used in the menu
void createAccount();
void searchAccount();
void deposit();
void withdraw();
void deleteAccount();

//structure to store the user information
struct account {
	int acc_no;
	float balance;
	char name[50];
	int pin;
	int isActive;
};

//structure to store transactions related data 
struct transaction {
    int acc_no;
    char type[10];   
    float amount;
    float new_balance;
    char timestamp[30];
};

int main() {
	int choice;
	
	//menu
	while(1) {
		printf("<===== Bank Management System =====>\n");
		printf("1) Create an account\n");
		printf("2) Search an account\n");
		printf("3) Deposit amount\n");
		printf("4) Withdraw amount\n");
		printf("5) Delete an account\n");
		printf("6) Exit!\n");
		printf("Enter your choice: ");
		if(scanf("%d", &choice) != 1) {
			while (getchar() != '\n');
			printf("Invalid input: please try again!\n");
			continue;
		}
		
		switch(choice) {
			case 1:
				createAccount();
				break;
			case 2:
				searchAccount();
				break;
			case 3:
				deposit();
				break;
			case 4:
				withdraw();
				break;
			case 5:
				deleteAccount();
				break;
			case 6:
				printf("Exited the program successfully!");
				return 0;
			default:
				printf("Invalid input: please try again!\n");
		}
	}
	return 0;
}

//function to create an account 
void createAccount() {
	struct account a;
	
	FILE *fptr;
	fptr = fopen("account.txt", "a+");
	if(fptr == NULL) {
		printf("Error opening file!\n");
		return;
	}
	rewind(fptr);
	
	int last_acc_no = 0;
	while(fscanf(fptr, "%d\t%f\t%49s\t%d\t%d", &a.acc_no, &a.balance, a.name, &a.pin, &a.isActive) == 5)
		last_acc_no = a.acc_no;
		
	printf("Enter your Name: ");
	scanf("%49s", a.name);
	
	printf("Enter your PIN: ");
	scanf("%d", &a.pin);
	
	printf("Enter initial Deposit: ");
	scanf("%f", &a.balance);
	
	a.isActive = 1;
	
	a.acc_no = (last_acc_no == 0) ? 5000 : last_acc_no + 1;
	fprintf(fptr, "%d\t%.2f\t%s\t%d\t%d\n", a.acc_no, a.balance, a.name, a.pin, a.isActive);
	printf("Account created successfully! Your Account Number is %d\n", a.acc_no);

	fclose(fptr);
}

//function to search an account with PIN verification
void searchAccount() {
	int target, entered_pin, found = 0;
	struct account a;
	FILE *fptr = fopen("account.txt", "r");
	if(fptr == NULL) {
		printf("Error: No accounts found!\n");
		return;
	}

	printf("Enter your Account Number: ");
	scanf("%d", &target);
	printf("Enter your PIN: ");
	scanf("%d", &entered_pin);
	
	while(fscanf(fptr, "%d\t%f\t%49s\t%d\t%d", 
		&a.acc_no, &a.balance, a.name, &a.pin, &a.isActive) == 5) {
		
		if(a.acc_no == target && a.isActive == 1) {
			found = 1;
			if(entered_pin == a.pin) {
				printf("Account found!\n");
				printf("Account No: %d | Name: %s | Balance: %.2f\n", 
					   a.acc_no, a.name, a.balance);
			} else {
				printf("Incorrect PIN! Access denied.\n");
			}
			fclose(fptr);
			return;
		}
	}
	printf("No account found with that account number!\n");
	fclose(fptr);
}

//function to deposit an amount with PIN verification
void deposit() {
    int target, found = 0, entered_pin;
    float amount;
    struct account a;
    
    FILE *fptr = fopen("account.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (fptr == NULL || temp == NULL) {
        printf("Error: No accounts found!\n");
        return;
    }

    printf("Enter your Account Number: ");
    scanf("%d", &target);
    printf("Enter your PIN: ");
    scanf("%d", &entered_pin);
    
    while (fscanf(fptr, "%d\t%f\t%49s\t%d\t%d", 
                  &a.acc_no, &a.balance, a.name, &a.pin, &a.isActive) == 5) {
        if (a.acc_no == target && a.isActive == 1) {
            found = 1;
            if (entered_pin == a.pin) {
                printf("Account found!\n");
                printf("Enter the amount you want to deposit: ");
                scanf("%f", &amount);
                a.balance += amount;
                printf("Deposited successfully! New balance: %.2f\n", a.balance);
            } else {
                printf("Incorrect PIN! Access denied.\n");
            }
        }
        fprintf(temp, "%d\t%.2f\t%s\t%d\t%d\n", 
                a.acc_no, a.balance, a.name, a.pin, a.isActive);
    }

    fclose(fptr);
    fclose(temp);

    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("Account not found.\n");
}

//function to withdraw an amount with PIN verification
void withdraw() {
    int target, found = 0, entered_pin;
    float amount;
    struct account a;
    
    FILE *fptr = fopen("account.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (fptr == NULL || temp == NULL) {
        printf("Error: No accounts found!\n");
        return;
    }

    printf("Enter your Account Number: ");
    scanf("%d", &target);
    printf("Enter your PIN: ");
    scanf("%d", &entered_pin);
    
    while (fscanf(fptr, "%d\t%f\t%49s\t%d\t%d", 
                  &a.acc_no, &a.balance, a.name, &a.pin, &a.isActive) == 5) {
        if (a.acc_no == target && a.isActive == 1) {
            found = 1;
            if (entered_pin == a.pin) {
                printf("Account found!\n");
                printf("Enter the amount you want to withdraw: ");
                scanf("%f", &amount);
                if(a.balance >= amount){
                    a.balance -= amount;
                    printf("Withdrew successfully! New balance: %.2f\n", a.balance);
                } else {
                    printf("Not enough sufficient balance to withdraw!\n");
                }
            } else {
                printf("Incorrect PIN! Access denied.\n");
            }
        }
        fprintf(temp, "%d\t%.2f\t%s\t%d\t%d\n", 
                a.acc_no, a.balance, a.name, a.pin, a.isActive);
    }

    fclose(fptr);
    fclose(temp);

    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("Account not found.\n");
}

//function to delete an account with PIN verification
void deleteAccount() {
    int target, entered_pin, found = 0;
    struct account a;
    
    FILE *fptr = fopen("account.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (fptr == NULL || temp == NULL) {
        printf("Error: No accounts found!\n");
        return;
    }

    printf("Enter your Account Number: ");
    scanf("%d", &target);
    printf("Enter your PIN: ");
    scanf("%d", &entered_pin);

    while (fscanf(fptr, "%d\t%f\t%49s\t%d\t%d",
                  &a.acc_no, &a.balance, a.name, &a.pin, &a.isActive) == 5) {

        if (a.acc_no == target && a.isActive == 1) {
            found = 1;
            if (entered_pin == a.pin) {
                a.isActive = 0; // mark account as inactive instead of deleting line
                printf("Account deleted successfully!\n");
            } else {
                printf("Incorrect PIN! Access denied.\n");
            }
        }
        fprintf(temp, "%d\t%.2f\t%s\t%d\t%d\n",
                a.acc_no, a.balance, a.name, a.pin, a.isActive);
    }

    fclose(fptr);
    fclose(temp);

    remove("account.txt");
    rename("temp.txt", "account.txt");

    if (!found)
        printf("Account not found.\n");
}



