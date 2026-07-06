#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024

int stack[STACK_SIZE];
int sp = 0;

/* ---------- stack helpers ---------- */

void push(int v) {
    if (sp >= STACK_SIZE) {
        printf("Mash error: stack overflow\n");
        exit(1);
    }
    stack[sp++] = v;
}

int pop() {
    if (sp <= 0) {
        printf("Mash error: stack underflow\n");
        exit(1);
    }
    return stack[--sp];
}

int top() {
    if (sp <= 0) return 0;
    return stack[sp - 1];
}

/* ---------- trim newline ---------- */

void trim_newline(char *s) {
    for (int i = 0; s[i]; i++) {
        if (s[i] == '\n' || s[i] == '\r') {
            s[i] = '\0';
            return;
        }
    }
}

/* ---------- main interpreter ---------- */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s file.mash\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Mash error opening file");
        return 1;
    }

    char line[256];

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);

        /* ignore empty lines */
        if (line[0] == '\0') continue;

        /* comments */
        if (line[0] == '#') continue;

        /* PUSH n */
        if (strncmp(line, "PUSH ", 5) == 0) {
            int v = atoi(line + 5);
            push(v);
        }

        else if (strcmp(line, "POP") == 0) {
            pop();
        }

        else if (strcmp(line, "DUP") == 0) {
            push(top());
        }

        else if (strcmp(line, "ADD") == 0) {
            int b = pop();
            int a = pop();
            push(a + b);
        }

        else if (strcmp(line, "SUB") == 0) {
            int b = pop();
            int a = pop();
            push(a - b);
        }

        else if (strcmp(line, "MUL") == 0) {
            int b = pop();
            int a = pop();
            push(a * b);
        }

        else if (strcmp(line, "DIV") == 0) {
            int b = pop();
            int a = pop();
            if (b == 0) {
                printf("Mash error: division by zero\n");
                exit(1);
            }
            push(a / b);
        }

        else if (strcmp(line, "PRINT") == 0) {
            printf("%d\n", top());
        }

        else if (strcmp(line, "CHAR") == 0) {
            printf("%c", (char)top());
        }

        else {
            printf("Mash error: unknown command '%s'\n", line);
            exit(1);
        }
    }

    fclose(f);
    return 0;
}
