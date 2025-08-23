#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h> 

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) 
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) 
    {
        // Process each line of the file
        printf("Read line: %s", line);
    }

    fclose(file);
    return EXIT_SUCCESS;
}