#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void delete() {
    char deleteID[20];
    printf("Enter Payment ID or Name to delete: ");
    scanf("%s", deleteID);

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    FILE *tempFp = fopen("temp.csv", "w");
    if (tempFp == NULL) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        char paymentID[20], payerName[50], paymentDate[15];
        float fineAmount;
        sscanf(line, "%[^,],%[^,],%f,%s", paymentID, payerName, &fineAmount, paymentDate);
        if (strcmp(paymentID, deleteID) == 0 || strcmp(payerName, deleteID) == 0) {
            found = 1;
            continue;
        }
        fprintf(tempFp, "%s", line);
    }

    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove("debtPayment.csv");
        rename("temp.csv", "debtPayment.csv");
        printf("Payment ID or Name %s deleted successfully.\n", deleteID);
    } else {
        remove("temp.csv");
        printf("Payment ID or Name %s not found.\n", deleteID);
    }
}

void update() {
    char updateID[20];
    printf("Enter Payment ID or Name to update: ");
    scanf("%s", updateID);

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    FILE *tempFp = fopen("temp.csv", "w");
    if (tempFp == NULL) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        char paymentID[20], payerName[50], paymentDate[15];
        float fineAmount;
        sscanf(line, "%[^,],%[^,],%f,%s", paymentID, payerName, &fineAmount, paymentDate);
        if (strcmp(paymentID, updateID) == 0) {
            printf("Enter new Payer Name: ");
            scanf("%s", payerName);
            printf("Enter new Fine Amount: ");
            scanf("%f", &fineAmount);
            printf("Enter new Payment Date (YYYY-MM-DD): ");
            scanf("%s", paymentDate);
            fprintf(tempFp, "%s,%s,%.2f,%s\n", paymentID, payerName, fineAmount, paymentDate);
            found = 1;
        } else if(strcmp(payerName,updateID)==0) {
            printf("Enter new Payer Name: ");
            scanf("%s", payerName);
            printf("Enter new Fine Amount: ");
            scanf("%f", &fineAmount);
            printf("Enter new Payment Date (YYYY-MM-DD): ");
            scanf("%s", paymentDate);
            fprintf(tempFp, "%s,%s,%.2f,%s\n", paymentID, payerName, fineAmount, paymentDate);
            found = 1;
        } else {
            fprintf(tempFp, "%s", line);
        }
    }

    fclose(fp);
    fclose(tempFp);

    if (found) {
        remove("debtPayment.csv");
        rename("temp.csv", "debtPayment.csv");
        printf("Payment ID %s updated successfully.\n", updateID);
    } else {
        remove("temp.csv");
        printf("Payment ID %s not found.\n", updateID);
    }
}

void search() {
    char searchID[20];
    printf("Enter Payment ID or Name to search: ");
    scanf("%s", searchID);

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        char paymentID[20], payerName[50], paymentDate[15];
        float fineAmount;
        sscanf(line, "%[^,],%[^,],%f,%s", paymentID, payerName, &fineAmount, paymentDate);
        if (strcmp(paymentID, searchID) == 0) {
            printf("Payment Found:\n");
            printf("ID: %s\nName: %s\nAmount: %.2f\nDate: %s\n", paymentID, payerName, fineAmount, paymentDate);
            found = 1;
            break;
        }else if(strcmp(payerName, searchID) == 0){
            printf("Payment Found:\n");
            printf("ID: %s\nName: %s\nAmount: %.2f\nDate: %s\n", paymentID, payerName, fineAmount, paymentDate);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Payment ID or Name %s not found.\n", searchID);
    }

    fclose(fp);
}

void addPayment() {
    char paymentID[20];
    char payerName[50];
    float fineAmount;
    char paymentDate[15];

    int nextNum = 0;
    FILE *fpRead = fopen("debtPayment.csv", "r");
    if (fpRead != NULL) {
        char line[256];
        int lastNum = 0;
        while (fgets(line, sizeof(line), fpRead)) {
            char pid[20];
            if (sscanf(line, "%[^,],", pid) == 1) {
            }
        }
        fclose(fpRead);
        nextNum = lastNum + 1;
    }

    sprintf(paymentID, "%03d", nextNum);
    printf("Your Payment ID is (***จำให้ได้***): %s\n", paymentID);
    printf("Enter Payer Name: ");
    scanf("%s", payerName);
    printf("Enter Fine Amount: ");
    scanf("%f", &fineAmount);
    printf("Enter Payment Date (YYYY-MM-DD): ");
    scanf("%s", paymentDate);

    FILE *fp = fopen("debtPayment.csv", "a");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    fprintf(fp, "%s,%s,%.2f,%s\n", paymentID, payerName, fineAmount, paymentDate);
    fclose(fp);
    printf("Payment %s added successfully!\n", paymentID);
}

void displayAllPayments() {
    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char line[100];
    printf("\nAll Payments:\n");
    printf("--------------------------------------------------\n");
    printf("ID\tName\t\tAmount\t\tDate\n");
    printf("--------------------------------------------------\n");
    (fgets(line, sizeof(line), fp));
    while (fgets(line, sizeof(line), fp)) {
        char paymentID[20], payerName[50], paymentDate[15];
        float fineAmount;
        sscanf(line, "%[^,],%[^,],%f,%s", paymentID, payerName, &fineAmount, paymentDate);
        printf("%s\t%-15s\t%.2f\t\t%s\n", paymentID, payerName, fineAmount, paymentDate);
    }

    fclose(fp);
}

void displayMenu() {
    int choice;
    do {
        printf("\nDebt Payment Management\n");
        printf("1. Add Payment\n");
        printf("2. Display All Payments\n");
        printf("3. Search Payments\n");
        printf("4. Update Payments\n");
        printf("5. Delete Payments\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPayment();
                break;
            case 2:
                displayAllPayments();
                break;
            case 3:
                search();
                break;
            case 4:
                update();
                break;
            case 5:
                delete();
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice, Please try again.\n");
        }
    } while (choice != 6);
}

int main() {
    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        fp = fopen("debtPayment.csv", "w");
        fprintf(fp, "PaymentID,PayerName,FineAmount,PaymentDate\n");
    }
    fclose(fp);
    displayMenu();
    return 0;
}
