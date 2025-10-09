#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int userIsStaff = 0;
static const char STAFF_PASSWORD[] = "flukelnwza";

static void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    } else {
        buf[0] = '\0';
    }
}

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void delete() {
    if (!userIsStaff) {
        printf("Permission denied: only staff can delete records.\n");
        return;
    }
    char deleteID[50];
    printf("Enter Payment ID or Name to delete: ");
    flush_stdin();
    read_line(deleteID, sizeof(deleteID));

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char lines[1000][256];
    int line_count = 0;
    while (line_count < 1000 && fgets(lines[line_count], sizeof(lines[line_count]), fp)) line_count++;
    fclose(fp);

    int matches[1000];
    int match_count = 0;
    for (int i = 0; i < line_count; ++i) {
        char paymentID[64], payerName[128], paymentDate[64];
        float fineAmount;
        if (sscanf(lines[i], "%63[^,],%127[^,],%f,%63s", paymentID, payerName, &fineAmount, paymentDate) < 1) continue;
        if (strcasecmp(paymentID, deleteID) == 0 || strcasecmp(payerName, deleteID) == 0) {
            matches[match_count++] = i;
        }
    }

    if (match_count == 0) {
        printf("Payment ID or Name %s not found.\n", deleteID);
        return;
    }

    printf("Found %d matching record(s):\n", match_count);
    for (int i = 0; i < match_count; ++i) {
        int idx = matches[i];
        char paymentID[64], payerName[128], paymentDate[64];
        float fineAmount;
        sscanf(lines[idx], "%63[^,],%127[^,],%f,%63s", paymentID, payerName, &fineAmount, paymentDate);
        printf("%d) ID:%s | Name:%s | Amount:%.2f | Date:%s\n", i+1, paymentID, payerName, fineAmount, paymentDate);
    }

    printf("Choose action: (a=delete all, s=select, n=cancel): ");
    char action[4];
    scanf(" %3s", action);

    int to_delete[1000] = {0};
    if (action[0] == 'a' || action[0] == 'A') {
        for (int i = 0; i < match_count; ++i) to_delete[matches[i]] = 1;
    } else if (action[0] == 's' || action[0] == 'S') {
        flush_stdin();
        char selbuf[256];
        printf("Enter numbers separated by space (e.g. 1 2 3): ");
        read_line(selbuf, sizeof(selbuf));
        char *tok = strtok(selbuf, " \t,;");
        while (tok) {
            int num = atoi(tok);
            if (num >= 1 && num <= match_count) {
                int orig_idx = matches[num-1];
                to_delete[orig_idx] = 1;
            }
            tok = strtok(NULL, " \t,;");
        }
    } else {
        printf("Cancelled. No changes made.\n");
        return;
    }

    FILE *tempFp = fopen("temp.csv", "w");
    if (!tempFp) { printf("Error creating temp file.\n"); return; }
    int deleted = 0;
    for (int i = 0; i < line_count; ++i) {
        if (to_delete[i]) { deleted++; continue; }
        fputs(lines[i], tempFp);
    }
    fclose(tempFp);

    if (deleted > 0) {
        remove("debtPayment.csv");
        rename("temp.csv", "debtPayment.csv");
        printf("Deleted %d record(s).\n", deleted);
    } else {
        remove("temp.csv");
        printf("No records selected.\n");
    }
}

void update() {
    char updateID[50];
    printf("Enter Payment ID or Name to update: ");
    flush_stdin();
    read_line(updateID, sizeof(updateID));

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char lines[1000][256];
    int line_count = 0;
    while (line_count < 1000 && fgets(lines[line_count], sizeof(lines[line_count]), fp)) line_count++;
    fclose(fp);

    int matches[1000];
    int match_count = 0;
    for (int i = 0; i < line_count; ++i) {
        char paymentID[64], payerName[128], paymentDate[64];
        float fineAmount;
        if (sscanf(lines[i], "%63[^,],%127[^,],%f,%63s", paymentID, payerName, &fineAmount, paymentDate) < 1) continue;
        if (strcasecmp(paymentID, updateID) == 0 || strcasecmp(payerName, updateID) == 0) matches[match_count++] = i;
    }

    if (match_count == 0) {
        printf("Payment ID or Name %s not found.\n", updateID);
        return;
    }

    printf("Found %d matching record(s):\n", match_count);
    for (int i = 0; i < match_count; ++i) {
        int idx = matches[i];
        char paymentID[64], payerName[128], paymentDate[64];
        float fineAmount;
        sscanf(lines[idx], "%63[^,],%127[^,],%f,%63s", paymentID, payerName, &fineAmount, paymentDate);
        printf("%d) ID:%s | Name:%s | Amount:%.2f | Date:%s\n", i+1, paymentID, payerName, fineAmount, paymentDate);
    }

    printf("Choose action: (a=update all, s=select, n=cancel): ");
    char action[4];
    scanf(" %3s", action);

    int to_update[1000] = {0};
    if (action[0] == 'a' || action[0] == 'A') {
        flush_stdin();
        char newName[128];
        char newDate[64];
        float newAmount;
        printf("Enter new Payer Name (will apply to all matches): ");
        read_line(newName, sizeof(newName));
        printf("Enter new Fine Amount: ");
        scanf("%f", &newAmount);
        flush_stdin();
        printf("Enter new Payment Date (YYYY-MM-DD): ");
        read_line(newDate, sizeof(newDate));
        for (int i = 0; i < match_count; ++i) to_update[matches[i]] = 1;
        FILE *tempFp = fopen("temp.csv", "w");
        if (!tempFp) { printf("Error creating temp file.\n"); return; }
        for (int i = 0; i < line_count; ++i) {
            if (to_update[i]) {
                char paymentID[64];
                sscanf(lines[i], "%63[^,],%*[^,],%*f,%*s", paymentID);
                fprintf(tempFp, "%s,%s,%.2f,%s\n", paymentID, newName, newAmount, newDate);
            } else fputs(lines[i], tempFp);
        }
        fclose(tempFp);
        remove("debtPayment.csv");
        rename("temp.csv", "debtPayment.csv");
        printf("Updated %d record(s).\n", match_count);
        return;
    } else if (action[0] == 's' || action[0] == 'S') {
        flush_stdin();
        char selbuf[256];
        printf("Enter numbers separated by space (e.g. 1 2 3): ");
        read_line(selbuf, sizeof(selbuf));
        char *tok = strtok(selbuf, " \t,;");
        while (tok) {
            int num = atoi(tok);
            if (num >= 1 && num <= match_count) {
                to_update[matches[num-1]] = 1;
            }
            tok = strtok(NULL, " \t,;");
        }
        FILE *tempFp = fopen("temp.csv", "w");
        if (!tempFp) { printf("Error creating temp file.\n"); return; }
        for (int i = 0; i < line_count; ++i) {
            if (!to_update[i]) { fputs(lines[i], tempFp); continue; }
            char paymentID[64], payerName[128], paymentDate[64];
            float fineAmount;
            sscanf(lines[i], "%63[^,],%127[^,],%f,%63s", paymentID, payerName, &fineAmount, paymentDate);
            printf("Updating record: ID:%s | Name:%s | Amount:%.2f | Date:%s\n", paymentID, payerName, fineAmount, paymentDate);
            char newName[128];
            char newDate[64];
            float newAmount;
            printf("Enter new Payer Name: ");
            read_line(newName, sizeof(newName));
            printf("Enter new Fine Amount: ");
            scanf("%f", &newAmount);
            flush_stdin();
            printf("Enter new Payment Date (YYYY-MM-DD): ");
            read_line(newDate, sizeof(newDate));
            fprintf(tempFp, "%s,%s,%.2f,%s\n", paymentID, newName, newAmount, newDate);
        }
        fclose(tempFp);
        remove("debtPayment.csv");
        rename("temp.csv", "debtPayment.csv");
        printf("Selected records updated.\n");
        return;
    } else {
        printf("Cancelled. No changes made.\n");
        return;
    }
}

void search() {
    if (!userIsStaff) {
        printf("Permission denied: only staff can search records.\n");
        return;
    }
    char searchID[20];
    printf("Enter Payment ID or Name to search: ");
    scanf("%19s", searchID);

    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        char paymentID[50], payerName[50], paymentDate[50];
        float fineAmount;
        if (sscanf(line, "%19[^,],%49[^,],%f,%19s", paymentID, payerName, &fineAmount, paymentDate) < 1) {
            continue;
        }
        if (strcasecmp(paymentID, searchID) == 0 || strcasecmp(payerName, searchID) == 0) {
            printf("\nPayment Found!\n");
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
    char paymentID[50];
    char payerName[50];
    float fineAmount;
    char paymentDate[50];

    int nextNum = 0;
    FILE *fpRead = fopen("debtPayment.csv", "r");
    if (fpRead != NULL) {
        char line[256];
        int lastNum = 0;
        while (fgets(line, sizeof(line), fpRead)) {
            char pid[20];
            if (sscanf(line, "%[^,],", pid) == 1)  {
                int num = atoi(pid);
                if (num > lastNum) {
                    lastNum = num;
                }
            }
        }
        fclose(fpRead);
        nextNum = lastNum + 1;
    }

    sprintf(paymentID, "%03d", nextNum);
    printf("Your Payment ID is (***Remember***): %s\n", paymentID);
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
    if (!userIsStaff) {
        printf("Permission denied: only staff can view all payments.\n");
        return;
    }
    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        printf("No payment data found.\n");
        return;
    }

    char line[512];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        printf("No payment data found.\n");
        return;
    }

    printf("\nAll Payments:\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("%-6s %-25s %12s %12s\n", "ID", "Name", "Amount", "Date");
    printf("-------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        char paymentID[64], payerName[128], paymentDate[32];
        float fineAmount;

        if (sscanf(line, "%63[^,],%127[^,],%f,%31s", paymentID, payerName, &fineAmount, paymentDate) != 4) {
            continue;
        }
        printf("%-6s %-25s %12.2f \t%12s\n", paymentID, payerName, fineAmount, paymentDate);
    }

    fclose(fp);
}

void displayMenu() {
    int choice;
    do {
        if (userIsStaff) {
            printf("\nDebt Payment Management (Staff)\n");
            printf("1. Add Payment\n");
            printf("2. Display All Payments\n");
            printf("3. Search Payments\n");
            printf("4. Update Payments\n");
            printf("5. Delete Payments\n");
            printf("6. Exit\n");
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) { flush_stdin(); choice = -1; }

            switch (choice) {
                case 1: addPayment(); break;
                case 2: displayAllPayments(); break;
                case 3: search(); break;
                case 4: update(); break;
                case 5: delete(); break;
                case 6: printf("Exiting the program.\n"); break;
                default: printf("Invalid choice, Please try again.\n");
            }
        } else {
            printf("\nDebt Payment Management (Customer)\n");
            printf("1. Add Payment\n");
            printf("2. Update Payments\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            if (scanf("%d", &choice) != 1) { flush_stdin(); choice = -1; }

            switch (choice) {
                case 1: addPayment(); break;
                case 2: update(); break;
                case 3: printf("Exiting the program.\n"); break;
                default: printf("Invalid choice, Please try again.\n");
            }
        }
    } while ((userIsStaff && choice != 6) || (!userIsStaff && choice != 3));
}

#ifndef UNIT_TEST
int main() {
    FILE *fp = fopen("debtPayment.csv", "r");
    if (fp == NULL) {
        fp = fopen("debtPayment.csv", "w");
        fprintf(fp, "PaymentID,PayerName,FineAmount,PaymentDate\n");
    }
    fclose(fp);
    int roleChoice = 0;
    do {
        printf("Select role:\n1) Staff\n2) Customer\nEnter choice: ");
        if (scanf("%d", &roleChoice) != 1) { flush_stdin(); roleChoice = 0; continue; }
        flush_stdin();
    } while (roleChoice != 1 && roleChoice != 2);

    if (roleChoice == 1) {
        char passbuf[128];
        int attempt;
        int auth_ok = 0;
        for (attempt = 1; attempt <= 3; ++attempt) {
            printf("Enter staff password (attempt %d of 3): ", attempt);
            read_line(passbuf, sizeof(passbuf));
            if (strcmp(passbuf, STAFF_PASSWORD) == 0) { auth_ok = 1; break; }
            printf("Incorrect password.\n");
        }
        if (auth_ok) {
            printf("Authenticated as staff.\n");
            userIsStaff = 1;
        } else {
            printf("Authentication failed. Continuing as customer.\n");
            userIsStaff = 0;
        }
    } else {
        userIsStaff = 0;
    }
    displayMenu();
    return 0;
}
#endif