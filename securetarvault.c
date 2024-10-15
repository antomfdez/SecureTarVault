#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATHS 100
#define COMMAND_SIZE 512


int is_valid_input(const char *input) {
    while (*input) {
        if (!isalnum(*input) && *input != '_' && *input != '-') {
            return 0;
        }
        input++;
    }
    return 1;
}

void encrypt(const char* publicKey, const char* outputFile, int numPaths, char* paths[]) {
    char tar[COMMAND_SIZE];
    sprintf(tar, "tar cf ./files.tar");

    for (int i = 0; i < numPaths; i++) {
        sprintf(tar + strlen(tar), " %s", paths[i]);
    }

    printf("Running: %s\n", tar);
    int tar_result = system(tar);
    
    if (tar_result != 0) {
        printf("Error creating tar archive. Exiting.\n");
        return;
    }

    char command[COMMAND_SIZE];
    sprintf(command, "gpg --output %s --encrypt --recipient %s ./files.tar", outputFile, publicKey);
    
    printf("Running: %s\n", command);
    int result = system(command);
    
    if (result == 0) {
        printf("Encryption succeeded.\n");
    } else {
        printf("Encryption failed with code: %d\n", result);
    }

    if (remove("./files.tar") != 0) {
        printf("Error removing files.tar.\n");
    } else {
        printf("Successfully removed files.tar.\n");
    }
}

void decrypt(const char* gpgFile) {
    char decryptCommand[COMMAND_SIZE];
    sprintf(decryptCommand, "gpg --output ./files.tar --decrypt %s", gpgFile);
    
    printf("Running: %s\n", decryptCommand);
    int decryptResult = system(decryptCommand);
    
    if (decryptResult != 0) {
        printf("Error decrypting the .gpg file. Exiting.\n");
        return;
    }
    
    char extractCommand[COMMAND_SIZE];
    sprintf(extractCommand, "tar xf ./files.tar");
    
    printf("Running: %s\n", extractCommand);
    int extractResult = system(extractCommand);
    
    if (extractResult == 0) {
        printf("Files successfully restored.\n");
    } else {
        printf("Error extracting the tar archive. Exiting.\n");
    }
    
    if (remove("./files.tar") != 0) {
        printf("Error removing files.tar after extraction.\n");
    } else {
        printf("Successfully removed files.tar after extraction.\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("  Encrypt: %s c <public_key> -o <output_file> <file_path1> [file_path2] ...\n", argv[0]);
        printf("  Decrypt: %s d <encrypted_file.gpg>\n", argv[0]);
        return 1;
    }

    const char *publicKey = NULL;
    const char *outputFile = NULL;
    int numPaths = 0;
    char *filePaths[MAX_PATHS];

    if (strcmp(argv[1], "c") == 0) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    outputFile = argv[i + 1];
                    i++; // Skip the output file name in the next iteration
                } else {
                    printf("Error: No output file specified after -o.\n");
                    return 1;
                }
            } else if (publicKey == NULL) {
                if (!is_valid_input(argv[i])) {
                    printf("Error: Invalid public key.\n");
                    return 1;
                }
                publicKey = argv[i]; // First non-option argument is the public key
            } else {
                if (numPaths >= MAX_PATHS) {
                    printf("Error: Too many file paths provided.\n");
                    return 1;
                }
                if (!is_valid_input(argv[i])) {
                    printf("Error: Invalid file path.\n");
                    return 1;
                }
                filePaths[numPaths++] = argv[i];
            }
        }

        if (publicKey == NULL || outputFile == NULL || numPaths == 0) {
            printf("Usage: %s c <public_key> -o <output_file> <file_path1> [file_path2] ...\n", argv[0]);
            return 1;
        }

        encrypt(publicKey, outputFile, numPaths, filePaths);
    } 
    else if (strcmp(argv[1], "d") == 0) {
        if (argc != 3) {
            printf("Usage: %s d <encrypted_file.gpg>\n", argv[0]);
            return 1;
        }

        const char *gpgFile = argv[2];

        decrypt(gpgFile);
    } 
    else {
        printf("Invalid operation. Use 'c' or 'd'.\n");
        return 1;
    }

    return 0;
}
