// Marmara University - Computer Engineering
// CSE225 Data Structures - Text Representation with High-Order Paths
// Ahmet Lekesiz - 150118509

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Document {
    char DocumentName[50];
    struct Document *NextDocument;
}typedef Document;

struct Order {
    struct Term *Term;
    struct Order *NextWord;
}typedef Order;

struct Term {
    char Word[100];
    Document *Document;
    Order *FirstOrder, *SecondOrder, *ThirdOrder;
    struct Term *NextTerm;
}typedef Term;

void getFilesRecursively(char *path, Term **root);
void getCategory(char *path, char *category);
void printFile(char *filePath, Term **root);
void addWordIntoMasterLinkedList(Term **root, char* word);
void printMasterLinkedList(Term *root);

int main() {
    Term *root = NULL;

    // Directory path of categories
    char path[500];

    // Input path from user
    printf("Enter the path of categories: ");
    scanf("%s", path);

    getFilesRecursively(path, &root);
    //char category[50];
    //getCategory(path, category);
    //printf("%s", category);

    printMasterLinkedList(root);
 /*   addWordIntoMasterLinkedList(&root, "beril");
    addWordIntoMasterLinkedList(&root, "mehmet");
    addWordIntoMasterLinkedList(&root, "ahmet");
    addWordIntoMasterLinkedList(&root, "ceyda");
    addWordIntoMasterLinkedList(&root, "zeynep");

    printMasterLinkedList(root);*/

    return 0;
}

void getFilesRecursively(char *basePath, Term **root)
{
    char path[1000];
    char category[100];
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

            //Is that folder or txt file?
            if(strncmp(extension, ".txt", 4) == 0){
                printFile(path, root);
            } else {
               // addCategory(path);
               //Kategoriler depolayacağın bir linked list implement et.
            }

            getFilesRecursively(path, root);
        }
    }
    closedir(dir);
}

void printFile(char *filePath, Term **root){
    FILE *in_file;
    char word[50] = "";
    char category[50] = "";
    char path[200] = "";
    strcpy(path, filePath);

    in_file = fopen(filePath, "r");

    if (in_file == NULL)
        printf("Can't open %s for reading.\n", filePath);
    else
    {
        while (fscanf(in_file, "%s", word) != EOF)
        {
            getCategory(path, category);
            printf("Kelime: %s, Kategori: %s\n", word, category);
            addWordIntoMasterLinkedList(root, word);
        }
        fclose(in_file);
    }
}

void getCategory(char *path, char *category){
    //Strings of array for the words in path.
    char words[30][100];

    int wordCounter = 0;
    char delim[] = "\\";

    char *ptr = strtok(path, delim);

    while (ptr != NULL)
    {
        strncpy(words[wordCounter], ptr, strlen(ptr));
        //printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
        wordCounter++;
    }

    strncpy(category, words[wordCounter-2], strlen(words[wordCounter-2]));
}

void addWordIntoMasterLinkedList(Term **root, char* word){
    //Check if MLL is empty or not.
    Term *iter = *root;
    if(*root == NULL){
        (*root) = (Term*)malloc(sizeof(Term));
        strncpy((*root)->Word, word, strlen(word));
        (*root)->NextTerm = NULL;
        return;
    }
    //If is not empty, add the word to end of the list
    else
    {
        //Create temp term to add.
        Term *temp = (Term*)malloc(sizeof(Term));
        temp->NextTerm = NULL;
        strncpy(temp->Word, word, strlen(word));

        //Add into list sorted.
        //If the word less than root, change the root.
        if(strcmp(word, (*root)->Word) < 0){
            temp->NextTerm = *root;
            *root = temp;
            return;
        }//If the word between word, find its place.
        else
        {
            while(iter->NextTerm != NULL && strcmp(word, iter->NextTerm->Word) < 0){
                iter = iter->NextTerm;
            }
            //Add to end of the list.
            if(iter->NextTerm == NULL){
                //sona ekle
                iter->NextTerm = temp;
            }else{
                temp->NextTerm = iter->NextTerm;
                iter->NextTerm = temp;
            }
        }
    }
}

void printMasterLinkedList(Term *root){
    Term *iter = root;
    while(iter->NextTerm != NULL){
        printf("%s\n", iter->Word);
        iter = iter->NextTerm;
    }
}