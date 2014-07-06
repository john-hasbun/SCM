// SCM Parser

// Copyright © 2014 Jonathan Hasbun


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PATH "pages/"  //This is the default directory for the landing pages.

const SIZE = 500;  //Max line size.


/* STRING REPLACE FUNCTION */
char * strrepl(const char *source, const char *substr, const char *replacement) {
    char *target = NULL;
    char *newstr = NULL;
    char *oldstr = NULL;
    char *head = NULL;

    //If either substr or replacement is NULL, return original string.
    if (substr == NULL || replacement == NULL)
        return strdup(source);

    newstr = strdup(source);
    head = newstr;

    //This loops in case there are multiple occasions of the substr in one line.
    while((target = strstr(head, substr))) {
        oldstr = newstr;
        newstr = malloc(strlen(oldstr) - strlen(substr) + strlen(replacement) + 1);

        if (newstr == NULL) {
            free(oldstr);
            return NULL;
        }

        memcpy(newstr, oldstr, target - oldstr);
        memcpy(newstr + (target - oldstr), replacement, strlen(replacement));
        memcpy(newstr + (target - oldstr) + strlen(replacement), target + strlen(substr), strlen(oldstr) - strlen(substr) - (target - oldstr));
        memset(newstr + strlen(oldstr) - strlen(substr) + strlen(replacement), 0, 1);

        //Move head back right after the last replacement.
        head = newstr + (target - oldstr) + strlen(replacement);

        free(oldstr);
    }

    return newstr;
}


/* STRING COUNT FUNCTION */
int strcnt(const char *string, const char *key) {
    char *currPos = NULL;
    int count = 0;
    int size = 0;

    size = strlen(key);
    currPos = strdup(string);

    //Return NULL if no key is provided.
    if (size == 0)
        return 0;

    //Step through the 'string' and increment everytime the 'key' is found.
    while(currPos = strstr(currPos, key)) {
        currPos += size;
        ++count;
    }

    return count;
}


/* STRING PARSE FUNCTION */
void strpar(const char *string, const char *key, char *container[], const size_t size) {
    char *str = strdup(string);
    char *currPos = NULL;  //This holds the current position of the 'key'.
    int x = 0;  //This stores the current index.

    while(currPos = strstr(str, key)) {
        //This prevents the possibility of a segmentation fault.
        if (x > size - 1) {
            printf("FATAL ERROR: the program tried to access restricted memory.");
            exit(EXIT_FAILURE);
        }


        int wordSize = currPos - str;  //This is the size of the word delimited by the key.

        container[x] = malloc(wordSize + 1);  //This allocates the necessary memory.
        memcpy(container[x], str, wordSize);  //This copies over the string on the left side of the 'key'.
        memset(container[x] + wordSize, 0, 1);  //This adds the NULL character to the end of the string.

        char *tmp = NULL;
        int strSize = strlen(str);

        //This updates the str to its current position.
        tmp = malloc(strSize + 1);
        memcpy(tmp, currPos + 1, strSize - strlen(container[x] - 1));
        memset(tmp + strSize - strlen(container[x]) - 1, 0, 1);
        str = strdup(tmp);


        free(tmp);
        ++x;  //This increments the index.
    }

    //This copies over the string on the right side of the last 'key'.
    container[x] = strdup(str);
}


/* MAIN FUNCTION */
int main(int argc, char *argv[])
{
    //This checks the number of arguments.
    if (argc < 4) {
        printf("ERROR: Not enough arguments were provided.");
        exit(EXIT_FAILURE);
    }
    else if (argc > 4) {
        printf("ERROR: Too many arguments were provided.");
        exit(EXIT_FAILURE);
    }


    //This initializes essential variables.
    char *string = strdup(argv[1]);  //The 2nd argument has to be the 'string'.
    char *title = strdup(argv[2]);  //The 3rd argument has to be the 'title'.
    FILE *landing_page;
    landing_page = fopen(argv[3], "r");  //The 4th argument has to be the 'landing_page'.
    int strs = strcnt(string, ",") + 1;  //Gets the number of strings (delimited by ",") to store.
    char *container[strs];
    strpar(string, ",", container, strs);  //This parses the 'string'.


    //This creates the city/state specific landing pages.
    if (landing_page != NULL) {
        char line[SIZE];  //This stores the original line.
        char *newLine = NULL;  //This stores the replacement line.
        char directory[SIZE];  //This stores the directory that the files will be saved in.
        char command[SIZE];
        char file[SIZE];  //This stores the files of each additional landing page.
        int i = 0;

        //This creates a custom directory for the landing pages based of the original file name.
        snprintf(directory, SIZE, "%s%s", PATH, title);
        snprintf(command, SIZE, "%s %s", "mkdir", directory);
        system(command);  //This creates a folder to hold the landing pages.

        for(; i < strs; (i += 2)) {
            snprintf(file, SIZE, "%s/%s.%s", directory, container[i], "html");  //This creates a unique filename for each page.

            FILE *page;
            page = fopen(file, "a");

            while(fgets(line, SIZE, landing_page) != NULL) {
                newLine = strrepl(line, "CITY", container[i]);  //This substitutes the city.
                newLine = strrepl(newLine, "STATE", container[i + 1]);  //This substitutes the state.

                fputs(newLine, page);
            }

            fclose(page);
            rewind(landing_page);
        }
    }
    else {
        printf("ERROR: the provided file could not be opened.");
        exit(EXIT_FAILURE);
    }

    fclose(landing_page);


    while(argc--)
        printf("%s\n", *argv++);

    exit(EXIT_SUCCESS);
}   
