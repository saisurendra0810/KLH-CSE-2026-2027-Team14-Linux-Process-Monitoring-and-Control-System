#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_LEN 100

// Token types
typedef enum {
    COMMAND,
    ARGUMENT,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    SEMICOLON
} TokenType;

// Token structure
typedef struct {
    char value[MAX_LEN];
    TokenType type;
} Token;

// Convert token type to string
const char* getTokenType(TokenType type) {
    switch (type) {
        case COMMAND:      return "COMMAND";
        case ARGUMENT:     return "ARGUMENT";
        case PIPE:         return "PIPE";
        case REDIRECT_IN:  return "REDIRECT_IN";
        case REDIRECT_OUT: return "REDIRECT_OUT";
        case SEMICOLON:    return "SEMICOLON";
        default:           return "UNKNOWN";
    }
}

// Check if character is a delimiter
int isDelimiter(char ch) {
    return ch == '|' || ch == '<' || ch == '>' || ch == ';';
}

// Tokenize input
int tokenize(char *input, Token tokens[]) {
    int count = 0;
    int i = 0;
    int firstWord = 1;

    while (input[i] != '\0') {

        // Handle whitespace
        if (isspace(input[i])) {
            i++;
            continue;
        }

        // Handle delimiters
        if (isDelimiter(input[i])) {

            tokens[count].value[0] = input[i];
            tokens[count].value[1] = '\0';

            if (input[i] == '|')
                tokens[count].type = PIPE;
            else if (input[i] == '<')
                tokens[count].type = REDIRECT_IN;
            else if (input[i] == '>')
                tokens[count].type = REDIRECT_OUT;
            else
                tokens[count].type = SEMICOLON;

            count++;
            i++;
            firstWord = 1;
            continue;
        }

        // Read normal word
        int j = 0;

        while (input[i] != '\0' &&
               !isspace(input[i]) &&
               !isDelimiter(input[i])) {

            if (j < MAX_LEN - 1)
                tokens[count].value[j++] = input[i];

            i++;
        }

        tokens[count].value[j] = '\0';

        // First word after delimiter is command
        if (firstWord) {
            tokens[count].type = COMMAND;
            firstWord = 0;
        } else {
            tokens[count].type = ARGUMENT;
        }

        count++;
    }

    return count;
}

// Validate token stream
int validateTokens(Token tokens[], int count) {

    if (count == 0) {
        printf("Error: Empty command.\n");
        return 0;
    }

    // Command cannot start with pipe
    if (tokens[0].type == PIPE) {
        printf("Error: Command cannot start with '|'.\n");
        return 0;
    }

    // Command cannot end with pipe
    if (tokens[count - 1].type == PIPE) {
        printf("Error: Command cannot end with '|'.\n");
        return 0;
    }

    // Two consecutive pipes are invalid
    for (int i = 0; i < count - 1; i++) {
        if (tokens[i].type == PIPE &&
            tokens[i + 1].type == PIPE) {

            printf("Error: Consecutive pipes are not allowed.\n");
            return 0;
        }
    }

    return 1;
}

// Display tokens for debugging
void debugTokens(Token tokens[], int count) {

    printf("\n--- TOKEN STREAM ---\n");

    for (int i = 0; i < count; i++) {
        printf("Token %d: %-10s Type: %s\n",
               i + 1,
               tokens[i].value,
               getTokenType(tokens[i].type));
    }
}

// Generate simple parse/execution structure
void generateExecutionStructure(Token tokens[], int count) {

    printf("\n--- EXECUTION STRUCTURE ---\n");

    printf("Command: ");

    for (int i = 0; i < count; i++) {

        if (tokens[i].type == PIPE) {
            printf("\n       |\n       v\nCommand: ");
        }
        else if (tokens[i].type == REDIRECT_IN) {
            printf(" [INPUT REDIRECTION] ");
        }
        else if (tokens[i].type == REDIRECT_OUT) {
            printf(" [OUTPUT REDIRECTION] ");
        }
        else if (tokens[i].type == SEMICOLON) {
            printf("\n       ;\nCommand: ");
        }
        else {
            printf("%s ", tokens[i].value);
        }
    }

    printf("\n");
}

// Main function
int main() {

    char input[500];
    Token tokens[MAX_TOKENS];

    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline
    input[strcspn(input, "\n")] = '\0';

    // Tokenization
    int count = tokenize(input, tokens);

    // Debug token output
    debugTokens(tokens, count);

    // Validate syntax
    if (!validateTokens(tokens, count)) {
        return 1;
    }

    printf("\nSyntax validation successful.\n");

    // Generate execution structure
    generateExecutionStructure(tokens, count);

    return 0;
}
