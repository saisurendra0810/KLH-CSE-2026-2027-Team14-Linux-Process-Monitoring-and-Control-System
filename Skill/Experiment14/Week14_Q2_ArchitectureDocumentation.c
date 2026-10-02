#include <stdio.h>

int main()
{
    printf("OSSP Shell Architecture\n");
    printf("=======================\n\n");

    printf("User Input\n");
    printf("   |\n");
    printf("   v\n");
    printf("Command Parser\n");
    printf("   |\n");
    printf("   v\n");
    printf("Built-in / External Command\n");
    printf("   |\n");
    printf("   v\n");
    printf("Process Creation - fork()\n");
    printf("   |\n");
    printf("   v\n");
    printf("Program Execution - exec()\n");
    printf("   |\n");
    printf("   v\n");
    printf("Process Control - wait()\n");

    printf("\nImportant System Calls:\n");
    printf("fork()  - Create process\n");
    printf("exec()  - Execute program\n");
    printf("wait()  - Wait for process\n");
    printf("pipe()  - Create communication channel\n");
    printf("dup2()  - Redirect streams\n");

    printf("\nArchitecture and execution path documented.\n");

    return 0;
}
