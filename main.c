// Marmara University - Computer Engineering
// CSE225 Data Structures - Text Representation with High-Order Paths
// Ahmet Lekesiz - 150118509

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

struct Document {
    char DocumentName[50];
    char CategoryName[50];
    struct Document *NextDocument;
}typedef Document;

struct Occurrence {
    struct Term* term;
    struct Occurrence *nextTerm;
}typedef Occurrence;

struct Term {
    char Word[100];
    int DocumentCounter;
    Occurrence *FirstOrder, *SecondOrder, *ThirdOrder;
    Document *Document;
    struct Term *NextTerm;
}typedef Term;

void getFilesRecursively(char *path, Term **root);
void getCategoryandDocument(char *path, char *category, char *documentName);
void printFile(char *filePath, Term **root);
void addWordIntoMasterLinkedList(Term **root, char* word, char *documentName, char *categoryName);
int checkIfWordAlreadyExist(Term *root, char *word, char *documentName, char *categoryName);
void printMasterLinkedList(Term *root);
void printDocumentList(Document *document);
void addDocument(Document* node, char *document, char *category);
void firstOccurrence(Term *root);
void addFirstOccurrence(Term *baseTerm, Term *additionTerm);
void printFirstOccurrence(Term *root);
void addSecondOccurrence(Term *root);

int main() {
    setlocale(LC_ALL, "Turkish");
    Term *root = NULL;

    // Directory path of categories
    char path[500] = "C:\\Users\\ahmet\\Desktop\\2019-2020\\DataStructures\\Project1\\DataSet\\Deneme";

    // Input path from user
   // printf("Enter the path of categories: ");
  //  scanf("%s", path);

    getFilesRecursively(path, &root);
  //  printf("*****");
    firstOccurrence(root);
   // printMasterLinkedList(root);
    printFirstOccurrence(root);
    addSecondOccurrence(root);
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
    char documentName[50] = "";
    strcpy(path, filePath);

    in_file = fopen(filePath, "r");

    if (in_file == NULL)
        printf("Can't open %s for reading.\n", filePath);
    else
    {
        while (fscanf(in_file, "%s", word) != EOF)
        {
            getCategoryandDocument(path, category, documentName);
            printf("Kelime: %s, Kategori: %s, Document: %s\n", word, category, documentName);
            addWordIntoMasterLinkedList(root, word, documentName, category);
            memset(word, 0, sizeof(word));
            memset(documentName, 0, sizeof(documentName));
        }
        fclose(in_file);
    }
}

void getCategoryandDocument(char *path, char *category, char *document){
    //Strings of array for the words in path.
    char words[30][100];
    memset(words, 0, sizeof(words));
    char FilePath[200] = "";
    strcpy(FilePath, path);

    int wordCounter = 0;
    char delim[] = "\\";

    char *ptr = strtok(FilePath, delim);

    while (ptr != NULL)
    {
        strncpy(words[wordCounter], ptr, strlen(ptr));
        ptr = strtok(NULL, delim);
        wordCounter++;
    }

    strcpy(category, words[wordCounter-2]);
    strcpy(document, words[wordCounter-1]);
}

void addWordIntoMasterLinkedList(Term **root, char *word, char *documentName, char *categoryName){
    Term *temp = (Term*)malloc(sizeof(Term));
    strcpy(temp->Word, word);
    temp->Document = NULL;
    temp->FirstOrder = NULL;
    temp->SecondOrder = NULL;
    temp->ThirdOrder = NULL;
    //If MLL is empty.
    if(*root == NULL){
        (*root) = (Term*)malloc(sizeof(Term));
        strcpy((*root)->Word, word);
        (*root)->Document = malloc(sizeof(Document));
        strcpy((*root)->Document->DocumentName, documentName);
        strcpy((*root)->Document->CategoryName, categoryName);
        (*root)->DocumentCounter = 1;
        (*root)->Document->NextDocument =NULL;
        (*root)->NextTerm = NULL;
        (*root)->FirstOrder = NULL;
        (*root)->SecondOrder = NULL;
        (*root)->ThirdOrder = NULL;
        return;
    }else if(checkIfWordAlreadyExist(*root, word, documentName, categoryName) == 0){
        //Add document
        temp->Document = malloc(sizeof(Document));
        temp->DocumentCounter = 1;
        strcpy(temp->Document->DocumentName, documentName);
        strcpy(temp->Document->CategoryName, categoryName);
        temp->Document->NextDocument = NULL;
        //ilk elemandan kücük durumu
        if(strcmp(word, (*root)->Word) < 0){
            //change the root
            temp->NextTerm = (*root);
            *root = temp;
            return;
        }
        Term *iter = *root;
        while(iter->NextTerm != NULL && strcmp(word, iter->NextTerm->Word) > 0){
            iter = iter->NextTerm;
        }
        temp->NextTerm = iter->NextTerm;
        iter->NextTerm = temp;
        return;
    }
}

int checkIfWordAlreadyExist(Term *root, char *word, char *documentName, char *categoryName){
    Term *iter = root;
    while(iter != NULL){
        //If the word already exist, return 1.
        if(strcmp(iter->Word, word) == 0){
            //If the word already exist in the master linked list, add document into list
            addDocument(iter->Document, documentName, categoryName);
            iter->DocumentCounter = iter->DocumentCounter + 1;
            return 1;
        }
        iter = iter->NextTerm;
    }
    //If the word does not exist in the list, return 0.
    return 0;
}

void printMasterLinkedList(Term *root){
    Term *iter = root;
    while(iter->NextTerm != NULL){
        printf("%s\n", iter->Word);
        iter = iter->NextTerm;
    }
}

void addDocument(Document* node, char* document, char* category){
    while(node->NextDocument != NULL){
        node = node->NextDocument;
    }
    Document *temp = malloc(sizeof(Document));
    temp->NextDocument = NULL;
    strcpy(temp->DocumentName, document);
    strcpy(temp->CategoryName, category);

    node->NextDocument = temp;
}
void firstOccurrence(Term *root){
    //master linkled list'i tek tek gez, her bir node'u sağındaki node larla karşılaştır ve first occurent listini doldur.
    Term *temp = root;
    Term *nextTemp;
    nextTemp = temp->NextTerm;
    Occurrence *tempOrder;
        while(nextTemp != NULL && temp->Document != NULL){
            while(nextTemp != NULL && nextTemp->Document != NULL){
                if(strcmp(temp->Document->DocumentName, nextTemp->Document->DocumentName) == 0
                && strcmp(temp->Document->CategoryName, nextTemp->Document->CategoryName) == 0)
                {
                    if(temp->FirstOrder == NULL){
                        temp->FirstOrder = (Occurrence*)malloc(sizeof(Occurrence));
                        temp->FirstOrder->term = nextTemp;
                        temp->FirstOrder->nextTerm = NULL;
                    }else{
                        addFirstOccurrence(temp, nextTemp);
                    }
                }
                if(nextTemp->Document->NextDocument == NULL){
                    nextTemp = nextTemp->NextTerm;
                }else{
                    nextTemp->Document = nextTemp->Document->NextDocument;
                }
                nextTemp = nextTemp->NextTerm;
            }
            temp = temp->NextTerm;
        }
    }

void addFirstOccurrence(Term *baseTerm, Term *additionTerm){
    Occurrence *iter;
    iter = baseTerm->FirstOrder;

    while(iter->nextTerm != NULL)
        iter = iter->nextTerm;

    iter->nextTerm = malloc(sizeof(Occurrence));
    iter->nextTerm->term = additionTerm;
    iter->nextTerm->nextTerm = NULL;
}

void printDocumentList(Document *document){
    Document *iter;
    iter = document;
    printf("%s", iter->DocumentName);
    while(iter->NextDocument != NULL){
        iter = iter->NextDocument;
        printf("%s", iter->DocumentName);
    }
}

void printFirstOccurrence(Term *root){
    printf("1st-order term co-occurrence: ");
    Occurrence *iterOccurrence = root->FirstOrder;
    Term *iterRoot = root;
    while(iterRoot != NULL){
        while(iterOccurrence != NULL){
            printf("{%s, %s}, ", iterRoot->Word, iterOccurrence->term->Word);
            iterOccurrence = iterOccurrence->nextTerm;
        }
        iterOccurrence = iterRoot->FirstOrder;
        iterRoot = iterRoot->NextTerm;
    }
}

void addSecondOccurrence(Term *root){
    Term * iterRoot = root;
    Occurrence * iterOccurrence = iterRoot->FirstOrder->term->FirstOrder;
    Occurrence * secondOccurrenceIter;

    while(iterRoot != NULL){
        while(iterOccurrence != NULL){
            if(iterRoot->SecondOrder == NULL){
                iterRoot->SecondOrder = (Occurrence*)malloc(sizeof(Occurrence));
                iterRoot->SecondOrder->term = iterOccurrence->term;
                iterRoot->SecondOrder->nextTerm = NULL;
                secondOccurrenceIter = iterRoot->SecondOrder;
            }else{
                while(secondOccurrenceIter->nextTerm != NULL){
                    secondOccurrenceIter = secondOccurrenceIter->nextTerm;
                }
                secondOccurrenceIter->nextTerm = (Occurrence*)malloc(sizeof(Occurrence));
                secondOccurrenceIter->nextTerm->term = iterOccurrence->term;
                secondOccurrenceIter->nextTerm->nextTerm = NULL;
            }
            iterOccurrence = iterOccurrence->nextTerm;
        }
        if(iterRoot->FirstOrder != NULL && iterRoot->FirstOrder->term->FirstOrder != NULL){
            iterOccurrence = iterRoot->FirstOrder->term->FirstOrder;
        }
        iterRoot = iterRoot->NextTerm;
    }


}