#include <stdio.h>
#include <string.h>

int main() {
    remove("debtPayment.csv");
    extern void displayAllPayments();
    FILE *out = freopen("test_out.txt", "w", stdout);
    if (!out) { perror("freopen"); return 2; }
    displayAllPayments();
    fflush(stdout);
    freopen("CON", "w", stdout);
    FILE *r = fopen("test_out.txt", "r"); if (!r) { perror("fopen"); return 3; }
    char buf[1024]; size_t n = fread(buf,1,sizeof(buf)-1,r); buf[n]=0; fclose(r);
    if (strstr(buf, "No payment data found.")) {
        printf("TEST PASSED: missing file handled\n");
        remove("test_out.txt");
        return 0;
    } else {
        printf("TEST FAILED: expected 'No payment data found.' but got:\n%s\n", buf);
        remove("test_out.txt");
        return 1;
    }
}
