#include <stdio.h>
#include <string.h>

void displayAllPayments();

int main() {
    FILE *out = freopen("test_out.txt", "w", stdout);
    if (!out) { perror("freopen"); return 2; }

    displayAllPayments();

    fflush(stdout);
    freopen("CON", "w", stdout);

    FILE *f = fopen("test_out.txt", "r");
    if (!f) { perror("fopen"); return 3; }
    char buf[2048];
    size_t n = fread(buf,1,sizeof(buf)-1,f);
    buf[n] = '\0';
    fclose(f);
}
