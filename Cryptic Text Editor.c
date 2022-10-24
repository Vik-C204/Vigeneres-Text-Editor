#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Better to inspect the code bottom-up starting from the main function as some
//repeated code won't get commented later on.

#define MAX_CHARS_PER_LINE 320 // Big enough buffer size for fgets to include up
                               // to 80(the usual limit) 4 byte characters
                               // but not more not to waste memory.
int lineNumber(char* name);
int num_validation(char* num,char* name) {
    int i;
    for(i = 0;i<strlen(num);i++){ // input has to be a positive integer
        if(isdigit(num[i]) == 0){
            perror("Line number was not inputted correctly,please rerun the"
                   "program making sure this input is a positive character");
            exit(EXIT_FAILURE);
        }

    }
    // check if number line is more than current lines in text or less than 1
    if(atoi(num) > lineNumber(name) && atoi(num) < 1){
      // num is checked so can use atoi without worrying
        perror("Error! Input was bigger/smaller than the number of lines in the"
        "file,please rerun the program");
        exit(EXIT_FAILURE);
    }

    return 1;
}
void filenameValidation(char* filename){
    int i;
    // can't have filename of more than 255 bytes in unix
    if(strlen(filename) > 255){
        perror("Filename bigger than the allowed 255 bytes");
        exit(EXIT_FAILURE);
    }
    for(i=0;i<strlen(filename);i++){
        if((int) filename[i] == 0 || // input validation for illegal characters
           (int) filename[i] == 47 ||// to have in a filename on Windows or Unix.
           (int) filename[i] == 92 ||
           (int) filename[i] == 58 ||
           (int) filename[i] == 42 ||
           (int) filename[i] == 63 ||
           (int) filename[i] == 60 ||
           (int) filename[i] == 62 ||
           (int) filename[i] == 124 ||
           (int) filename[i] == 34) {
            perror("Illegal character in the filename");
            exit(EXIT_FAILURE);
        }
}}
/*Counts the number of X where X is 3rd argument of the program
 * Done by reading the file and increasing the 'countable' variable
 * depending on conditions specific to each x*/
void count(char* name, char* toCount){
    int toCount_validation = 0;
    FILE *file;
    int countable = 0;
    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }
    /*For words for each "space that follow up with a char" increment the
    countable and add 1 at the end*/
    if(strcmp(toCount,"words") == 0){
        while(1){
            char temp[MAX_CHARS_PER_LINE+1];
            if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
                break;
            int i;
            int isEmpty = 1;//Assume line is blank by default
            if(isblank(temp[0]))//If first section of the line is blank decrement
                countable--;//countable as now there will be extra word counted
            for(i=0;i<strlen(temp);i++){
                if(isblank(temp[i]) && !isblank(temp[i+1]))
                    countable++;
                if(!isspace(temp[i]))//if char will be encountered switch the
                    isEmpty = 0;     //isEmpty bool to false
            }
            if(isEmpty == 1)//If line was empty don't increment
                continue;
            countable++;// words = spaces + 1, this is +1

        }toCount_validation = 1;}
    //For characters just count the characters*/
    else if(strcmp(toCount,"characters") == 0){
        while(1) {
            char temp[MAX_CHARS_PER_LINE+1];
            if (fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
                break;
            countable += (int) strlen(temp);
        }toCount_validation = 1;}
/*For letters use isalpha function to count letters either upper or lower case*/
    else if(strcmp(toCount,"letters") == 0){
        while(1) {
            char temp[MAX_CHARS_PER_LINE+1];
            if (fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
                break;
            int i;
            for(i=0;i< strlen(temp);i++){
                if(isalpha(temp[i]))
                    countable++;
            }}toCount_validation = 1;}
    /*For sentences increment countable for each . or ! or ? encountered*/
    else if(strcmp(toCount,"sentences") == 0){
        while(1) {
            char temp[MAX_CHARS_PER_LINE+1];
            if (fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
                break;
            int i;
            for(i=0;i< strlen(temp);i++){
                if(temp[i] == '.' || temp[i] == '?' || temp[i] == '!' )
                    countable++;
            }}toCount_validation = 1;}
    fclose(file);
    if(toCount_validation == 0){
        perror("The element to count was inputted incorrectly,please re-run the"
        "program with an appropriate 3rd command-line argument as specified in"
        "the documentation.");
        exit(EXIT_FAILURE);
    }
    printf("There are %d %s in %s\n",countable,toCount,name);
}
/*Same as encrypt but the formula now subtracts cipher text from key text*/
void decrypt(char* name,char* key){
    int k;
    for(k=0;k<strlen(key);k++){
        if(isalpha(key[k]) == 0){
            perror("Key contains an invalid character.Only alphanumeric letters"
            "are allowed");
            exit(EXIT_FAILURE); // key validation
        }

    }
    FILE *file;
    FILE *newFile;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }

    if ((newFile = fopen("temporaryName.txt", "w")) == NULL) {
        perror("Error modifying the file!");
        exit(EXIT_FAILURE);
    }

    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        int i,p;
        for(i=0;i<strlen(temp);i++){
            if (islower(temp[i])) {
                p = (((int) temp[i] - 97) - ((int) tolower(key[i % strlen(key)])
                 - 97));
                if(p<0) // In cases were original sum of indices went past 26 so
                    p += 26;//26 got taken away because of mod so add 26 back to
                p = p % 26 + 97;//get index of plain text char
            }
            else if (isupper(temp[i])) {
                p = (((int) temp[i] - 65) - ((int) toupper(key[i % strlen(key)])
                 - 65));
                if(p<0)
                    p += 26;
                p = p % 26 + 65;
            }
            else
                p = (int) temp[i];
            fputc(p, newFile);
        }
    }
    fclose(file);
    rename("temporaryName.txt",name);
    fclose(newFile);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Decrypted: %s\n",name);
    fclose(file);

}
/*Encrypts the contents of the text file using Vigenere cipher and the key string
Done by creating a copy of the file with a temporary name,then scanning the file
line by line,char by char adding its encrypted variant(or not if non letter) to
the copy. Rename the copy to the original name(overwriting the original in the
process)*/
void encrypt(char* name,char* key){
    int k;
    for(k=0;k<strlen(key);k++){
        if(isalpha(key[k]) == 0){
            perror("Key contains an invalid character.Only alphanumeric letters"
                   "are allowed");
            exit(EXIT_FAILURE);
        } // Validates the key by making sure its alphanumeric letters only

    }

    FILE *file;
    FILE *newFile;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }

    if ((newFile = fopen("temporaryName.txt", "w")) == NULL) {
        perror("Error modifying the file!");
        exit(EXIT_FAILURE);
    }

    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        int i,c;
/*Char by char go through the line encrypting the character (using the vigenere
 cypther formula) if it's a letter and just leaving it as is if
 non-letter.Case is maintained.*/
        for(i=0;i<strlen(temp);i++) {
            if (islower(temp[i]))
                //As chars in C are ASCII take away it's number to start the
                //alphabet at 0 and then add the ASCII number back
                                                            //For repeating key
                c = (((int) temp[i] - 97) + ((int) tolower(key[i % strlen(key)])
                - 97)) % 26 + 97;
            else if (isupper(temp[i]))
                c = (((int) temp[i] - 65) + ((int) toupper(key[i % strlen(key)])
                 - 65)) % 26 + 65;
            else
                c = (int) temp[i];
            fputc(c, newFile);
        }
    }
    fclose(file);
    rename("temporaryName.txt",name);
    fclose(newFile);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Encrypted: %s\n",name);
    fclose(file);

}
/*Scans line by line the file specified incrementing the variable with every new
 line.
* Prints out the variable*/
int lineNumber(char* name){
    FILE *file;
    int lines = 0;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }
    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        lines++;
    }

    fclose(file);
    return lines; // rather than printing the number return it as you will need
                  // it for other things as well
}
/*Takes line number from stdin and reads up to that line from our file.
 * Writes the line wanted to the standard output*/
void showLine(char* name){
    FILE *file;
    char wantedLine[10];
    int lines = 1;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error !");
        exit(EXIT_FAILURE);
    }

    printf("Input the line number of the line to show.\n");
    scanf("%s", wantedLine);
    num_validation(wantedLine,name);
    int wanted_line = atoi(wantedLine);

    char logLine[MAX_CHARS_PER_LINE+1]; //Save line for log
    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(lines == wanted_line)
            fputs(fgets(logLine, MAX_CHARS_PER_LINE, file), stdout);
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        lines++;
    }
    fclose(file);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Displayed the line:\n%sfrom %s\n",logLine,name);
    fclose(file);
}
/*Inserts the line of content at the specified line number, both taken from
standard input. Done by creating a copy of the file with a temporary name,then
copying the contents until the required line is encountered.
* Take the line from stdin and add the new line to the copy.
* Copy the rest and rename the copy to the original name(overwriting the
original in the process).*/
void insertLine(char* name){
    FILE *file;
    FILE *newFile;
    char wantedLine[10];
    int lines = 0;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }
    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        lines++;
    }
    rewind(file); // reset the file pointer

    if ((newFile = fopen("temporaryName.txt", "w")) == NULL) {
        perror("Error modifying the file!");
        exit(EXIT_FAILURE);
    }

    printf("Input the line number of the line to inject.\n");
    scanf("%s", wantedLine);
    num_validation(wantedLine,name);
    int wanted_line = atoi(wantedLine);
    getchar(); // to get rid of \n at the end of the buffer so that next fgets
               //won't cause unwanted behaviour.

    char logLine[MAX_CHARS_PER_LINE+1];//Save for log
    int i;
    for(i=1;i<=lines+1;i++) {
        char temp[MAX_CHARS_PER_LINE+1];
        if (i == wanted_line) {
            printf("Input the line of content you want to append.\n");
      // get the stdin line with fgets and insert into temporary copy with fputs
            fputs(fgets(logLine, MAX_CHARS_PER_LINE, stdin), newFile);
            continue;
        }
        fputs(fgets(temp, MAX_CHARS_PER_LINE, file), newFile);
    }
    fclose(file);
    rename("temporaryName.txt",name);
    fclose(newFile);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Inserted the line:\n%sto %s\n",logLine,name);
    fclose(file);
}
/*Deletes the line of content at the specified line number(taken from stdin).
 * Done by creating a copy of the file with a temporary name,then copying the
 contents until the required line is encountered.
 * Read the required line(for fgets to move on) but don't write it to the copy.
 * Copy the rest and rename the copy to the original name(overwriting the
 original in the process).*/
void deleteLine(char* name){
    FILE *file;
    FILE *newFile;
    char unwantedLine[10];
    int lines = 0;

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }
    // Count the lines in the file in a similar way to copy method where each
    // use of fgets points to a new line
    while(1){
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        lines++;
    }
    rewind(file); // reset the file pointer

    if ((newFile = fopen("temporaryName.txt", "w")) == NULL) {
        perror("Error modifying the file!");
        exit(EXIT_FAILURE);
    }

    printf("Input the line number of the line to delete.\n");
    scanf("%s", unwantedLine);
    num_validation(unwantedLine,name); // validate the input here so that
    int unwanted_line = atoi(unwantedLine);//it's a positive integer
                      //atoi is fine as we validated the input

    // Save the deleted line to then display in the change log.
    char logLine[MAX_CHARS_PER_LINE+1];

    int i;
    for(i=1;i<=lines;i++) {
        char temp[MAX_CHARS_PER_LINE+1];
        if (i == unwanted_line) {// use so that line pointer will move
            char *unneededStream = fgets(logLine, MAX_CHARS_PER_LINE, file);
            continue;            // but don't add it the temporary copy
        }
        fputs(fgets(temp, MAX_CHARS_PER_LINE, file), newFile);
    }
    fclose(file);
    rename("temporaryName.txt",name);
    fclose(newFile);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Deleted the line:\n%sfrom %s\n",logLine,name);
    fclose(file);
}
/*Appends a new line of text taken from the standard input to
 * the file specified*/
void appendLine(char* name){
    FILE *file;
    if ((file = fopen(name, "a")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }
    char line[MAX_CHARS_PER_LINE+1];
    printf("Input the line of content you want to append.\n");
    fputs(fgets(line,MAX_CHARS_PER_LINE,stdin),file);
    fclose(file);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Appended a line:\n%sto %s\n",line,name);
    fclose(file);
}
/*Line by line text is read from the file and is
 * written to standard output*/
void displayFile(char* name){
    FILE *file;
    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }

    while(1){ // line by line write to standard output
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break;
        fputs(temp,stdout);
    }
    fclose(file);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Displayed: %s\n",name);
    fclose(file);
}
/*Creates a new file with _copy added to the name of the original file.
 * Line by line text is read from original and is written to the copy*/
void copyFile(char* name){
    FILE *file;
    FILE *copy;
    char newName[256];

    if ((file = fopen(name, "r")) == NULL) {
        perror("Error opening the file!");
        exit(EXIT_FAILURE);
    }

    printf("Input the filename of the destination file of this copy.\n");
    scanf("%s",newName);
    filenameValidation(newName);

    if ((copy = fopen(newName, "w")) == NULL) {
        perror("Error modifying the file!");
        exit(EXIT_FAILURE); // Opens or creates the destination file of the copy
    }

    while(1){//Line by line copy from source file and paste into destination file
        char temp[MAX_CHARS_PER_LINE+1];
        if(fgets(temp, MAX_CHARS_PER_LINE, file) == NULL)
            break; //Break when EOF is found
        fputs(temp,copy);
    }
    if(lineNumber(newName) > 0)
        fputc('\n',copy);
    fclose(file);
    fclose(copy);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file,"Copied: %s as %s\n",name,newName);
    fclose(file);
}
/*Creates the file using the specified name*/
void createFile(char* name){
    filenameValidation(name); // Validate filename first

    FILE *file;
    if ((file = fopen(name, "w")) == NULL) {
        perror("Error creating the file!");
        exit(EXIT_FAILURE);
    } //Good way to open the file and check for errors simultaneously
    fclose(file);

    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE); // In case something goes wrong with the change log
    }
    fprintf(file,"Created: %s\n",name); // Append to Change log
    fclose(file);
}

int main(int argc, char** argv) {

    int first_arg_validation = 0;

/* First command-line argument takes in the name of the operation and second the
* name of the file on which the operation will be performed.
* For operations ENCRYPT, DECRYPT and COUNT 3 arguments are required. */
    if (strcmp(argv[1], "CREATE") == 0){
        createFile(argv[2]);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "COPY") == 0){
        copyFile(argv[2]);
        first_arg_validation = 1;
    }
    /*As deleting the file is already a function no point in making another one
     * like with other operations*/
    else if (strcmp(argv[1], "DELETE") == 0){
        FILE* file;
        remove(argv[2]);
      //Just like here at the end of every operation function its record is
      //appended to the ChangeLog.txt file(one is created in the same directory
      //if there wasn't one already)
        if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
            perror("Error adding the file to the change log!");
            exit(EXIT_FAILURE);
        }
        fprintf(file,"Deleted: %s\n",argv[2]);
        fclose(file);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "DISPLAY") == 0){
        displayFile(argv[2]);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "APPEND") == 0){
        appendLine(argv[2]);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "EXTERMINATE") == 0){
        deleteLine(argv[2]);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "INSERT") == 0){
        insertLine(argv[2]);
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "SHOW") == 0){
        showLine(argv[2]);
        first_arg_validation = 1;
    }
    //Uses the DISPLAY operation to show the change log file.
    else if (strcmp(argv[1], "LOG") == 0){ // no need for a specific file here
        displayFile("ChangeLog.txt");
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "LINES") == 0){
        printf("There are %d lines in %s\n",lineNumber(argv[2]),argv[2]);
        first_arg_validation = 1;
        // we need line number in input validation so ather than it being a
    } // void like others it returns line number and only then prints.
    else if (strcmp(argv[1], "ENCRYPT") == 0){
        encrypt(argv[2],argv[3]);//3rd argument is the key the user wants to use
        first_arg_validation = 1;
    }
    else if (strcmp(argv[1], "DECRYPT") == 0){
        decrypt(argv[2],argv[3]);//3rd argument is the key used when encrypting
        first_arg_validation = 1;//the file
    }
    else if (strcmp(argv[1], "COUNT") == 0){
        count(argv[2],argv[3]);//3rd argument is what we want to count that can be
        first_arg_validation = 1;//words,characters,letters and sentences.
    }
    FILE* file;
    if ((file = fopen("ChangeLog.txt", "a")) == NULL) {
        perror("Error adding the file to change log the file!");
        exit(EXIT_FAILURE);
    }
    // if file was deleted or isn't needed don't count its line number
    if(strcmp(argv[1], "DELETE") != 0 && strcmp(argv[1], "LOG") != 0)
        fprintf(file,"Line count now: %d\n", lineNumber(argv[2]));
    fclose(file); // add line count to change log as asked in the question

    if(first_arg_validation == 0) // Input validation for first argument.
        perror("Operation name wasn't recognised,please rerun the program with"
               "an exact operation name as specified in the documentation");

}
