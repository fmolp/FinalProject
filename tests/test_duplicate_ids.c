#include <stdio.h>
#include <string.h>

int main() {
    FILE *f = fopen("debtPayment.csv", "r");
    if (!f) { perror("fopen"); return 2; }

    char line[512];
    /* skip header */
    if (!fgets(line, sizeof(line), f)) { fclose(f); printf("Empty file\n"); return 3; }

    char ids[2000][64];
    int counts[2000];
    int n = 0;

    while (fgets(line, sizeof(line), f)) {
        char pid[64];
        if (sscanf(line, "%63[^,],", pid) != 1) continue;
        int found = 0;
        for (int i = 0; i < n; ++i) {
            if (strcmp(ids[i], pid) == 0) { counts[i]++; found = 1; break; }
        }
        if (!found) {
            strncpy(ids[n], pid, sizeof(ids[n]) - 1);
            ids[n][sizeof(ids[n]) - 1] = '\0';
            counts[n] = 1;
            n++;
        }
    }
    fclose(f);

    int duplicate_id_count = 0;
    int duplicate_record_count = 0;
    for (int i = 0; i < n; ++i) {
        if (counts[i] > 1) {
            duplicate_id_count++;
            duplicate_record_count += (counts[i] - 1);
        }
    }

    printf("Distinct duplicate IDs: %d\n", duplicate_id_count);
    printf("Total duplicate records (beyond first occurrences): %d\n", duplicate_record_count);

    if (duplicate_id_count == 0 && duplicate_record_count == 0) {
        printf("TEST PASSED: no duplicate PaymentID found.\n");
        return 0;
    } else {
        printf("TEST FAILED: duplicates detected.\n");
        return 1;
    }
}
