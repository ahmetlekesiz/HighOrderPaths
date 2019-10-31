// Marmara University - Computer Engineering
// CSE225 Data Structures - Text Representation with High-Order Paths
// Ahmet Lekesiz - 150118509

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<locale.h>

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
int checkIfWordAlreadyExist(Term *root, char *word);
void printMasterLinkedList(Term *root);

int main() {
    setlocale(LC_ALL, "Turkish");
    Term *root = NULL;

    // Directory path of categories
    char path[500];

    // Input path from user
    printf("Enter the path of categories: ");
    scanf("%s", path);

    getFilesRecursively(path, &root);
    printMasterLinkedList(root);

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
            int length;
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
            memset(word, 0, sizeof(word));
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
        ptr = strtok(NULL, delim);
        wordCounter++;
    }

    strncpy(category, words[wordCounter-2], strlen(words[wordCounter-2]));
}

void addWordIntoMasterLinkedList(Term **root, char *word){
    //If MLL is empty.
    if(*root == NULL){
        (*root) = (Term*)malloc(sizeof(Term));
        strcpy((*root)->Word, word);
        (*root)->NextTerm = NULL;
        return;
    }else if(checkIfWordAlreadyExist(*root, word) == 0){
        //ilk elemandan kücük durumu
        if(strcmp(word, (*root)->Word) < 0){
            //change the root
            Term *temp = (Term*)malloc(sizeof(Term));
            strcpy(temp->Word, word);
            temp->NextTerm = (*root);
            *root = temp;
            return;
        }
        Term *iter = *root;
        while(iter->NextTerm != NULL && strcmp(word, iter->NextTerm->Word) > 0){
            iter = iter->NextTerm;
        }
        Term *temp = (Term*)malloc(sizeof(Term));
        strcpy(temp->Word, word);
        temp->NextTerm = iter->NextTerm;
        iter->NextTerm = temp;
        return;
    }

}

int checkIfWordAlreadyExist(Term *root, char *word){
    Term *iter = root;
    while(iter->NextTerm != NULL){
        //If the word already exist, return 1.
        if(strcmp(iter->Word, word) == 0){
            return 1;
        }
        iter = iter->NextTerm;
    }
    //If the word does not exist in the list, return 0.
    return 0;
}

void addDocument(Term *term, char *documentName){
    Document* temp = (Document*)malloc(sizeof(Document));
    strcpy(temp->DocumentName, documentName);
    temp->NextDocument = NULL;

    if(term->Document == NULL){
        term->Document = temp;
    }else{
        Document* iter;
        iter = term->Document;
        while(iter->NextDocument != NULL){
            iter = iter->NextDocument;
        }
        iter->NextDocument = temp;
    }
}

void printMasterLinkedList(Term *root){
    Term *iter = root;
    while(iter->NextTerm != NULL){
        printf("%s\n", iter->Word);
        iter = iter->NextTerm;
    }
}

