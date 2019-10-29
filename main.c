// Marmara University - Computer Engineering
// CSE225 Data Structures - Text Representation with High-Order Paths
// Ahmet Lekesiz - 150118509

#include <stdio.h>
#include <dirent.h>
#include <string.h>

struct Document {
    char DocumentName[50];
    struct Document *NextDocument;
}typedef Document;

struct Order {
    char Word[100];
    struct Order *NextWord;
}typedef Order;

struct Term {
    char Word[100];
    Document *Document;
    Order *FirstOrder, *SecondOrder, *ThirdOrder;
}typedef Term;

void getFilesRecursively(char *path);
void printFile(char *filePath);

int main() {
    // Directory path of categories
    char path[200];

    // Input path from user
    printf("Enter the path of categories: ");
    scanf("%s", path);

    getFilesRecursively(path);

    return 0;
}

void getFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            int length = strlen(dp->d_name);
            char extension[5];

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "\\");
            strcat(path, dp->d_name);

            //printf("%s\n", dp->d_name);
            printf("%s\n", path);

            length = strlen(path);
            for (int i = 0; i < 4 ; i++) {
                int dec = 4 - i;
                extension[i] = path[length-dec];
            }

            if(strncmp(extension, ".txt", 4) == 0){
                printFile(path);
            }

            getFilesRecursively(path);
        }
    }
    closedir(dir);
}

void printFile(char *filePath){
    FILE *in_file;
    char word[50];

    in_file = fopen(filePath, "r");

    if (in_file == NULL)
        printf("Can't open %s for reading.\n", filePath);
    else
    {
        while (fscanf(in_file, "%s", word) != EOF)
        {
            printf("%s\n", word);
        }
        fclose(in_file);
    }
}