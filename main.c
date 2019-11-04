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

struct Category{
    char CategoryName[50];
    int Counter;
    struct Category *NextCategory;
}typedef Category;

struct Occurrence {
    struct Term* term;
    struct Occurrence *nextTerm;
}typedef Occurrence;

struct Term {
    char Word[100];
    int DocumentCounter;
    Occurrence *FirstOrder, *SecondOrder, *ThirdOrder;
    Document *Document;
    Category *Category;
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
void printSecondOccurrence(Term *root);
void printThirdOccurrence(Term *root);
int doesExistInFirstOccurrence(Term *baseTerm, Term *additionalTerm);
void secondOccurrence(Term *root);
void addSecondOccurrence(Term *baseTerm, Term *additionTerm);
void thirdOccurrence(Term *root);
int doesExistInSecondOccurrence();
void addThirdOccurrence(Term *baseTerm, Term *additionTerm);
void printOccurrence(Term *root, int occurrence);
void addCategory(Term *root, char* categoryName);

int main() {
    setlocale(LC_ALL, "Turkish");
    Term *root = NULL;

    // Directory path of categories
//    char path[500] = "C:\\Users\\ahmet\\Desktop\\2019-2020\\DataStructures\\Project1\\DataSet\\Deneme";
//    char path[500] = "C:\\myfolder";
    char path[500] = "C:\\Users\\ahmet\\Desktop\\2019-2020\\DataStructures\\Project1\\DataSet\\smalldataset2";
    // Input path from user
   // printf("Enter the path of categories: ");
  //  scanf("%s", path);

    getFilesRecursively(path, &root);
  //  printf("*****");
    firstOccurrence(root);
   // printMasterLinkedList(root);
    secondOccurrence(root);
    thirdOccurrence(root);
    printOccurrence(root, 1);
    printf("\n\n");
    printOccurrence(root, 2);
    printf("\n\n");
    printOccurrence(root, 3);


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
        (*root)->Category = NULL;
        addCategory((*root), categoryName);
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
        temp->Category = NULL;
        addCategory(temp, categoryName);
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

void addCategory(Term *root, char* categoryName){
    Term *iterRoot = root;
    Category *iterCategory = root->Category;
    if(strcmp(root->Word, "aabb") == 0){
        printf("asdas");
    }
    if(root->Category == NULL){
        root->Category = (Category*)malloc(sizeof(Category));
        strcpy(root->Category->CategoryName, categoryName);
        root->Category->Counter = 1;
        root->Category->NextCategory = NULL;
    }else{
        while(iterCategory != NULL){
            if(strcmp(iterCategory->CategoryName, categoryName) == 0 ){
                iterCategory->Counter++;
                return;
            }else if(iterCategory->NextCategory != NULL){
                iterCategory = iterCategory->NextCategory;
            }else if(iterCategory->NextCategory == NULL){
                iterCategory->NextCategory = (Category*)malloc(sizeof(Category));
                strcpy(iterCategory->NextCategory->CategoryName, categoryName);
                iterCategory->NextCategory->Counter = 1;
                iterCategory->NextCategory->NextCategory = NULL;
                return;
            }
        }

    }
}

int checkIfWordAlreadyExist(Term *root, char *word, char *documentName, char *categoryName){
    Term *iter = root;
    while(iter != NULL){
        //If the word already exist, return 1.
        if(strcmp(iter->Word, word) == 0){
            //If the word already exist in the master linked list, add document into list
            addDocument(iter->Document, documentName, categoryName);
            addCategory(iter, categoryName);
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
    Term *mainIter = root;
    Document *mainDocumentIter = root->Document;
    Term *nextIter = root->NextTerm;
    Document *nextDocumentIter = nextIter->Document;

    while(mainIter != NULL){
        while(mainDocumentIter != NULL){
            while(nextIter != NULL){
                if(strcmp(mainDocumentIter->DocumentName, nextDocumentIter->DocumentName) == 0
                   && strcmp(mainDocumentIter->CategoryName, nextDocumentIter->CategoryName) == 0)
                {
                    addFirstOccurrence(mainIter,nextIter);
                    nextIter = nextIter->NextTerm;
                    if(nextIter != NULL) {
                        nextDocumentIter = nextIter->Document;
                    }
                }
                else if(nextDocumentIter->NextDocument != NULL)
                {
                    nextDocumentIter = nextDocumentIter->NextDocument;
                }else if(nextDocumentIter->NextDocument == NULL)
                {
                    nextIter = nextIter->NextTerm;
                    if(nextIter != NULL){
                        nextDocumentIter = nextIter->Document;
                    }
                }
            }
            mainDocumentIter = mainDocumentIter->NextDocument;
            nextIter = mainIter->NextTerm;
            if(nextIter != NULL){
                nextDocumentIter = nextIter->Document;
            }
        }
        mainIter = mainIter->NextTerm;
        if(mainIter != NULL){
            mainDocumentIter = mainIter->Document;
            nextIter = mainIter->NextTerm;
            if(nextIter!=NULL){
                nextDocumentIter = nextIter->Document;
            }
        }
    }
}

void addFirstOccurrence(Term *baseTerm, Term *additionTerm){
    if(baseTerm->FirstOrder == NULL){
        baseTerm->FirstOrder = (Occurrence*)malloc(sizeof(Occurrence));
        baseTerm->FirstOrder->term = additionTerm;
        baseTerm->FirstOrder->nextTerm = NULL;
    }else{
        Occurrence *iter;
        iter = baseTerm->FirstOrder;

        while(iter->nextTerm != NULL){
            iter = iter->nextTerm;
        }

        iter->nextTerm = malloc(sizeof(Occurrence));
        iter->nextTerm->term = additionTerm;
        iter->nextTerm->nextTerm = NULL;
    }
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

void printOccurrence(Term *root, int occurrence){
    if(occurrence == 1){
        printf("1st-order term co-occurrence: ");
    }else if(occurrence == 2){
        printf("2nd-order term co-occurrence: ");
    }else if(occurrence == 3){
        printf("3rd-order term co-occurrence: ");
    }
    Term *iterRoot = root;
    Occurrence *iterOccurrence;
    while(iterRoot != NULL){
        if(occurrence == 1){
            iterOccurrence = iterRoot->FirstOrder;
        }else if(occurrence == 2){
            iterOccurrence = iterRoot->SecondOrder;
        }else if(occurrence == 3){
            iterOccurrence = iterRoot->ThirdOrder;
        }
        while(iterOccurrence != NULL){
            printf("{%s, %s}, ", iterRoot->Word, iterOccurrence->term->Word);
            iterOccurrence = iterOccurrence->nextTerm;
        }
        iterRoot = iterRoot->NextTerm;
    }
}

void secondOccurrence(Term *root){
    Term *mainIter = root;
    Occurrence *mainFirstOccurrenceIter = mainIter->FirstOrder;
    Term *nextIter = mainFirstOccurrenceIter->term;
    Occurrence *nextFirstOccurrenceIter = nextIter->FirstOrder;

    while(mainIter != NULL){
        mainFirstOccurrenceIter = mainIter->FirstOrder;
        while(mainFirstOccurrenceIter != NULL){
            nextIter = mainFirstOccurrenceIter->term;
            nextFirstOccurrenceIter = nextIter->FirstOrder;
            while(nextFirstOccurrenceIter != NULL){
                if(doesExistInFirstOccurrence(mainIter, nextFirstOccurrenceIter->term) == 0){
                    addSecondOccurrence(mainIter, nextFirstOccurrenceIter->term);
                }
                nextFirstOccurrenceIter = nextFirstOccurrenceIter->nextTerm;
            }
            mainFirstOccurrenceIter = mainFirstOccurrenceIter->nextTerm;
        }
        mainIter = mainIter->NextTerm;
    }
}

int doesExistInFirstOccurrence(Term *baseTerm, Term *additionalTerm){
    Occurrence * iterBase = baseTerm->FirstOrder;
    while(iterBase != NULL){
        if(strcmp(iterBase->term->Word, additionalTerm->Word) == 0){
            //return 1 if it does exist.
            return 1;
        }else{
            iterBase = iterBase->nextTerm;
        }
    }
    //return 0 if it does not exist.
    return 0;
}

void addSecondOccurrence(Term *baseTerm, Term *additionTerm){
    if(baseTerm->SecondOrder == NULL){
        baseTerm->SecondOrder = (Occurrence*)malloc(sizeof(Occurrence));
        baseTerm->SecondOrder->term = additionTerm;
        baseTerm->SecondOrder->nextTerm = NULL;
    }else{
        Occurrence *iter = baseTerm->SecondOrder;
        while(iter->nextTerm != NULL){
            iter = iter->nextTerm;
        }
        iter->nextTerm = (Occurrence*)malloc(sizeof(Occurrence));
        iter->nextTerm->term = additionTerm;
        iter->nextTerm->nextTerm = NULL;
    }
}

void thirdOccurrence(Term *root){
    Term *mainIter = root;
    Occurrence *mainSecondOccurrenceIter = mainIter->SecondOrder;
    Term *nextIter = mainSecondOccurrenceIter->term;
    Occurrence *nextSecondOccurrenceIter = nextIter->SecondOrder;

    while(mainIter != NULL){
        mainSecondOccurrenceIter = mainIter->SecondOrder;
        while(mainSecondOccurrenceIter != NULL){
            nextIter = mainSecondOccurrenceIter->term;
            nextSecondOccurrenceIter = nextIter->FirstOrder;
            while(nextSecondOccurrenceIter != NULL){
                if(doesExistInSecondOccurrence(mainIter, nextSecondOccurrenceIter->term) == 0){
                    addThirdOccurrence(mainIter, nextSecondOccurrenceIter->term);
                }
                nextSecondOccurrenceIter = nextSecondOccurrenceIter->nextTerm;
            }
            mainSecondOccurrenceIter = mainSecondOccurrenceIter->nextTerm;
        }
        mainIter = mainIter->NextTerm;
    }
}

//FirstOrder ve SecondOrder exist methodları birleştirilebilir.
int doesExistInSecondOccurrence(Term *baseTerm, Term *additionalTerm){
    Occurrence * iterBase = baseTerm->SecondOrder;
    while(iterBase != NULL){
        if(strcmp(iterBase->term->Word, additionalTerm->Word) == 0){
            //return 1 if it does exist.
            return 1;
        }else{
            iterBase = iterBase->nextTerm;
        }
    }
    //return 0 if it does not exist.
    return 0;
}

//FirstOrder ve SecondOrder add methodları birleştirilebilir.
void addThirdOccurrence(Term *baseTerm, Term *additionTerm){
    if(baseTerm->ThirdOrder == NULL){
        baseTerm->ThirdOrder = (Occurrence*)malloc(sizeof(Occurrence));
        baseTerm->ThirdOrder->term = additionTerm;
        baseTerm->ThirdOrder->nextTerm = NULL;
    }else{
        Occurrence *iter = baseTerm->ThirdOrder;
        while(iter->nextTerm != NULL){
            iter = iter->nextTerm;
        }
        iter->nextTerm = (Occurrence*)malloc(sizeof(Occurrence));
        iter->nextTerm->term = additionTerm;
        iter->nextTerm->nextTerm = NULL;
    }
}

void printMostFrequentTenWords(Term *root){

}