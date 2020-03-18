#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct will be take the name, extension, line and file number. */
struct file {
    char *name;
    char *extension;
    char *content;
    int fileNumber;
};

/* To learn how many lines there are in the file */
int toLearnNumberOfLine (const char *indexOfFile, long numberOfByte){
    int numberOfLine = 0;
    long i ;

    for (i=0 ; i<numberOfByte ; i++){
        if (indexOfFile[i] == '\n'){
            numberOfLine += 1 ; /* It increments the number when it sees the \n character. */
        }
    }
    return numberOfLine+1 ;
}

/* The function counts the any word that wanted in line */
int countingAword (char *line , char *word){

    int i,j;
    int count; /* for counting the word */
    int bool ; /* for controlling */
    int lengthLine  = (int)strlen(line);
    int lengthWord = (int)strlen(word);

    count = 0;

    for(i=0; i <= lengthLine-lengthWord; i++){
        bool = 1;
        for(j=0; j<lengthWord; j++){
            if(line[i + j] != word[j]){
                bool = 0;
                break;
            }
        }
        if(bool == 1){
            count++; /* It increment the number for each matching */
        }
    }

    return count; /* It returns the number as an integer */
}

/* The function the starting index of desired word */
int findingPosition(char *content, char *oldWord) {

    char *position; /* The pointer which will points the word */
    int pst; /* The number which will keeps the starting index of *position */

    position = strstr(content, oldWord);

    pst = (int) (position - content);

    return pst; /* Returns the index */
}

/* The function changes the content, it help the Replacing function */
void changingContent(char *content, char *oldWord, char *newWord , int numberOfOccurence){

    int sizeContent = (int) strlen(content);
    int sizeOldWord = (int) strlen(oldWord);
    int sizeNewWord = (int) strlen(newWord);

    int i,j,k,t ; /* Iterators */
    int position;
    int tempSize = sizeContent;

    if(sizeOldWord >= sizeNewWord){  /* If the size of old word is  equal or less than new word */

        for (i=0 ; i<numberOfOccurence ; i++) {

            position = findingPosition(content,oldWord);

            for (j = position, k = 0; k < sizeNewWord; j++, k++) {
                content[j] = newWord[k];
            }
            for (; j < sizeContent; j++) {
                content[j] = content[j + (sizeOldWord - sizeNewWord)]; /* It slides the content toward left */
            }

            content = (char *) realloc(content, sizeof(char) * (sizeContent + (sizeNewWord-sizeOldWord))); /* It decrease the memory allocation or takes stabil */
        }

    }
    else if(sizeOldWord < sizeNewWord){ /* If the size of word that will be write over is bigger than old word */

        tempSize = (tempSize + numberOfOccurence *(sizeNewWord-sizeOldWord));
        content = (char *) realloc(content,sizeof(char) * tempSize); /* It expands the memory allocation */

        for (i=0 ; i<numberOfOccurence ; i++) {
            position = findingPosition(content,oldWord);
            for (j=tempSize ; j > (position + sizeNewWord)-1 ;j--){
                content[j] = content[j-(sizeNewWord-sizeOldWord)]; /* It slides the content toward right */
            }
            for (t=position,k=0 ; k < sizeNewWord ; t++,k++){
                content[t] = newWord[k];
            }
        }
    }
}

/* The function takes the content part from the line and returns it as a pointer */
char *takingContent(char *line){
    char *content; /* To keep content part */
    char command[] ="-t";

    int i,j,k ;
    int start=0 ; /* to find starting point */
    int finish=0; /* to find finishing point */
    int length = (int) strlen(line);

    for (i=0,j=1;j<length-1;i++,j++){
        if (line[i]==command[0] && line[j]==command[1] && line[j+1] == ' '){ /*It does operations when it finds -t part */
            start = j+2; /* Start point is the character after the space character */
            for (k=j+2; k<length;k++) {
                if (line[k] == '\n'){
                    finish = k; /*If -t is after -n */
                    break;
                }
                else if (line[k+1] == '-' && line[k+2] == 'n'){
                    finish=k; /* If -t is before -n */
                    break;
                }
            }
        }
    }

    content = (char *) malloc(sizeof(char)*(finish-start)); /* Allocate the memory for content */

    for (i=0,j=start;j<finish;i++,j++){
        content[i] = line[j]; /* Filling the content part from the line */
    }

    return content; /* It returns the content part as a pointer */
}

/* The function takes the extension part and returns it as a pointer */
char *takingExtension(const char *line, int middlePoint, int finishPoint){
    char *extension; /* To keep extension part */
    int i,j;
    int size = (finishPoint-middlePoint-1) ; /* Finds the size of extension part */
    extension = (char *) malloc(sizeof(char)* size); /* Allocating for necessary for extension */

    for (j=0,i=middlePoint+1; j< size ,i<finishPoint;i++,j++){
        extension[j] = line[i]; /* It fills the extension from the line */
    }

    return extension; /* Returns the extension part as a pointer */
}

/* The function specify the parts of line : name, content,extension. It returns a pointer array that contains these. */
char **takingPartsOfLine(char *line){
    char *name;
    char *extension;
    char *content;
    char **partsOfLine = (char **) calloc(3, sizeof(char *)); /* 3 for name, extension and content */
    char *command ="-n";

    int length = (int) strlen(line);
    int start = 0; /* to find starting point for name */
    int finish=0; /* to find finishing point for extension */
    int middle=0; /* to find finishing point for name and starting for extension */
    int i,j,k; /* Iterators for finding start,middle,finish */

    for (i=0,j=1 ;j<length-1; i++,j++){
        if (line[i]==command[0] && line[j]==command[1] && line[j+1] == ' '){ /*When it pairs with -n, it does the operations. */
            start=j+2; /* Start equals the character after space. */
            for(k=j+2; k<length ;k++){  /* k starts the point Start */
                if (line[k] == '.'){
                    middle=k; /* It finds the dot, so it is important for Start and Finish points. */
                }
                if(line[k] == ' ' ){
                    finish = k; /* It finds the Finish point */
                    break;
                }
                else if (line[k] == '\n'){
                    finish = k; /* It finds the Finish point */
                    break;
                }
            }
            break;
        }
    }

    name=(char *)malloc(sizeof(char) * (middle-start+1)); /* It allocates the memory for size of name */

    for (i=0, j=start ;j<middle ;i++,j++){ /*Filling the name from line */
        name[i] = line[j];
    }

    extension = takingExtension(line,middle,finish); /* Taking extension part */
    content = takingContent(line); /*Taking content part */

    partsOfLine[0] = (char *) malloc(sizeof(char)*((int)strlen(name))); /* Allocating for necessary memory for name */
    partsOfLine[1] = (char *) malloc(sizeof(char)*((int)strlen(extension))); /* Allocating for necessary memory for extension */
    partsOfLine[2] = (char *) malloc(sizeof(char)*((int)strlen(content))); /* Allocating for necessary memory for content */

    strcpy(partsOfLine[0],name); /* Puts the name to struct */
    strcpy(partsOfLine[1],extension); /* Puts the extension to struct */
    strcpy(partsOfLine[2],content); /* Puts the content to struct */

    free(extension); /* Deallocate for takingExtension function */
    free(content); /* Deallocate for takingContent function */
    free(name); /* Deallocate for name allocation */

    return partsOfLine; /* Returning the array of pointers that contains name, extension and content */

}

/*The function creates the file, and fills the related places in struct. And returns the total number of files */
int creatingFiles(char *line, int fileNumber, struct file *all,FILE *out){

    char *command = "create" ; /* The command is constant */
    char **thePartsOfLine; /* It will take the parts of the line : name,extension,content,fileNumber */
    int lengthOfCommand = (int) strlen(command);
    int i , bool=0;

    for (i=0 ; i<lengthOfCommand ; i++){ /* Controlling the command is true or not. */
        if (line[i] == command[i]){
            bool++ ;
        }
    }

    if (bool == lengthOfCommand){ /* If command is correct, then do the operations. */
        fprintf(out,"%s",line);
        fileNumber++ ; /* Increments the number, because a file was created. */
        //all = (struct file *) realloc(all,fileNumber * sizeof(struct file)); /*It reallocate the struct pointer according to the number of file. */
        thePartsOfLine = takingPartsOfLine(line);
        all[fileNumber-1].name = thePartsOfLine[0];
        all[fileNumber-1].extension = thePartsOfLine[1];
        all[fileNumber-1].content = thePartsOfLine[2];
        all[fileNumber-1].fileNumber = fileNumber ;

        free(thePartsOfLine);
        return fileNumber;

    }
    else {
        printf("Error! Invalid command!\n"); /* If command is incorect, it will give a error message. */
        fprintf(out,"Error! Invalid command!\n");
        return fileNumber;
    }
}

/*The function deletes the file and also deletes the related places in struct. And returns the total number of files */
int deletingFiles(char *line, int fileNumber,struct file *all,FILE *out){

    char *command = "delete"; /* The command is constant */
    int length = (int) strlen(command);
    int i , bool=0;
    char *name, *extension, **temp;

    for (i=0 ; i<length ; i++){
        if (line[i] == command[i]){
            bool++ ;
        }
    }
    if (bool == length){ /* If the command is correct, do the operations */
        fprintf(out,"%s",line);
        temp = takingPartsOfLine(line); /* taking parts of line */
        name = temp[0]; /* taking name */
        extension = temp[1]; /* taking extension */
        free(temp); /* Deallocate for takingPartsOfLine function */
        int k;
        for (k=0 ; k < fileNumber ; k++){
            if(!(strcmp(name,all[k].name)) && !(strcmp(extension,all[k].extension))){ /* If name and extension are correct, continue */

                fileNumber -- ;
                int p;
                for (p=k ; p<=fileNumber ; p++){
                    all[p] = all[p+1]; /* It transfers the datas previous struct */
                }
                all = (struct file *) realloc(all,fileNumber * sizeof(struct file)); /* Reallocating for decreasing file number */
                return fileNumber;
            }
        }
    }
    else { /* It will error message if command is not correct */
        printf("Error! Invalid command!\n");
        fprintf(out,"Error! Invalid command!\n");
        return fileNumber;
    }

}

/* The function removes the spesific part of the content */
void removingSthInFiles(char *line,struct file *all,int numberOfFile,FILE *out){

    char *command = "remove"; /* The command is constant */
    char **temp; /* a pointer array for taking parts of line */
    char *name ; /* taking for name */
    char *extension; /* taking for extension */

    int length = (int) strlen(command);
    int lengthOfLine = (int) strlen(line);
    int i,j,bool=0;
    int indexStart = 0; /* to specify the starting index for deleting */
    int numberToDelete = 0; /* to specify how many character will be deleted */

    for (i=0 ; i<length ; i++){
        if (line[i] == command[i]){
            bool++ ;
        }
    }
    if (bool == length){
        fprintf(out,"%s",line);
        temp = takingPartsOfLine(line);
        name = temp[0]; /*taking name */
        extension = temp[1]; /* taking extension */
        free(temp); /* Deallocate for takingPartsOfLine function */

        for (i=0,j=1 ;j<lengthOfLine; i++,j++){
            if (line[i]== '-' && line[j]== 's' && line[j+1] == ' '){ /* Finding the -s command */
                if (line[j+3] == ' '){
                    indexStart = (line[j+2] - '0'); /*Finding the start index */
                }
                else{
                    indexStart = (line[j+2] -'0')*10 + (line[j+3]-'0'); /*Finding the start index */
                }


            }
            if (line[i]== '-' && line[j]== 'l' && line[j+1] == ' '){ /* Finding -l command */
                if (line[j+3] == ' ' || line[j+3] == '\n'){
                    numberToDelete = (line[j+2] - '0'); /* Finding the number that will be deleted */
                }
                else{
                    numberToDelete = (line[j+2] -'0')*10 + (line[j+3]-'0'); /* Finding the number that will be deleted */
                }

            }
        }
        int k;
        for(k=0 ; k<numberOfFile ;k++){
            if(!(strcmp(name,all[k].name)) && !(strcmp(extension,all[k].extension))) {
                if ((indexStart+numberToDelete) <= (int) strlen(all[k].content )){ /* Controlling for if overflowing the line or not */
                    int n ;
                    for (n=indexStart+numberToDelete ; n<lengthOfLine ; n++,indexStart++){
                        (all[k].content)[indexStart] = (all[k].content)[n];
                    }
                    free(name);
                    free(extension);
                    break;
                }
                else { /* If the size is bigger tha length of line, it will give a error message */
                    printf("Error! The size of the number that will be deleted is out of line.");
                    break;
                }
            }
        }
    }
    else {
        fprintf(out,"Error! Invalid command!\n");
        printf("Error! Invalid command!\n");
    }
}

/* The function appending new things to the content related file. */
void appendingSthToFiles(char *line,struct file *all,int numberOfFile,FILE *out){

    char *command = "append"; /* The command is constant */
    char **temp; /* a pointer array for taking parts of line */
    char *name, *extension, *content;
    int length = (int) strlen(command);
    int i , bool=0;

    for (i=0 ; i<length ; i++){
        if (line[i] == command[i]){
            bool++ ;
        }
    }
    if (bool == length){
        fprintf(out,"%s",line);
        temp = takingPartsOfLine(line);
        name = temp[0];
        extension = temp[1];
        content = temp[2];
        free(temp);
        int controlForExisting = 0; /* Control for the file */

        int k;
        for (k=0 ; k<numberOfFile ;k++){
            if(!(strcmp(name,all[k].name)) && !(strcmp(extension,all[k].extension))){
                int newContent = (int) strlen(content); /* Length of te content that will be added. */
                int sizeLine = (int)strlen(all[k].content);
                all[k].content = (char *)realloc(all[k].content, sizeof(char)*(newContent+sizeLine)); /* Allocate for the memory for increasing size of the content */
                int x,y ;
                for (x=sizeLine,y=0; x<(sizeLine+newContent) ;x++,y++){
                    all[k].content[x] = content[y]; /* Adding the new content to end of the old content */
                }
                controlForExisting = 1; /* If there is a file like this, it will true */
                free(name);
                free(extension);
                free(content);
                break;
            }
        }
        if (controlForExisting != 1 ){ /* If there is no file like this, it will be give a error message */
            printf("Error! There is no file to delete like this name and extension.") ;
        }
    }
    else {
        fprintf(out,"Error! Invalid command!\n");
        printf("Error! Invalid command!\n");
    }
}

/* It replace a word to another word in the content related file. */
void replacingSthInFiles(char *line,struct file *all,int numberOfFile,FILE *out){

    char *command = "replace"; /* The command is constant */
    char **temp;
    char *name, *extension;
    int length = (int) strlen(command);
    int lengthOfLine = (int) strlen(line);
    int i , bool=0;

    for (i=0 ; i<length ; i++){ /* Checking for if the command is correct or not */
        if (line[i] == command[i]){
            bool++ ;
        }
    }
    if (bool == length){
        fprintf(out,"%s",line);
        temp = takingPartsOfLine(line);
        name = temp[0]; extension = temp[1];
        free(temp);

        int k,j,m;
        int newStart=0,newFinish=0,oldStart=0,oldFinish=0; /* The variables for determine the points for deleted and writed words */
        for(k=0,j=1,m=2; m<lengthOfLine ; k++,j++,m++){
            if (line[k] == '-' && line[j] == 'o' && line[m] == 'w' && line[m+1] == ' '){ /* Finds the command -ow */
                oldStart = m+2; /* Finds the start point for deleted word */
                int t;
                for (t=oldStart ; t<lengthOfLine ;t++){
                    if (line[t] == ' ' || line[t] == '\n'){ /* Finds the finish point for deleted word */
                        oldFinish = t;
                        break;
                    }
                }
            }
            if (line[k] == '-' && line[j] == 'n' && line[m] == 'w' && line[m+1] == ' '){ /* Finds the command -nw */
                newStart = m+2; /* Finds the start point for writed word */
                int t;
                for (t=newStart ; t<=lengthOfLine ;t++){
                    if (line[t] == ' ' || line[t] == '\n' || line[t] == '\0'){
                        newFinish = t; /* Finds the finish point for writed word */
                        break;
                    }
                }
            }
        }

        char *oldWord = (char *) malloc(sizeof(char)*(oldFinish-oldStart)); /* Allocate for keep for old word in temporary */
        char *newWord = (char *) calloc((newFinish-newStart),sizeof(char)); /* Allocate for keep for new word in temporary */

        int x,y,z;
        for (x=oldStart,z=0; x<oldFinish ; x++,z++){
            oldWord[z] = line[x]; /* Taking the old word in a pointer */

        }
        for (z=0 ,y=newStart ; z < (newFinish-newStart) ; z++ ,y++){
            newWord[z] = line[y];  /* Taking the new word in a pointer */
        }

        int controlForExisting = 0; /* Control for the file */

        for (k=0 ; k<numberOfFile ;k++){
            if (!(strcmp(name,all[k].name)) && !(strcmp(extension,all[k].extension))){
                    int number;
                    number = countingAword(all[k].content,oldWord); /* To determine the number that how many old word are there in the content */
                    changingContent(all[k].content,oldWord,newWord,number); /* Look at the function */
                    controlForExisting = 1; /* If there is a file like this, it will true */
                    break;

            }
        }
        if (controlForExisting != 1 ){ /* If there is no file like this, it will be give a error message */
            printf("Error! There is no file to delete like this name and extension.") ;
        }
        free(oldWord);
        free(newWord);
    }
    else {
        fprintf(out,"Error! Invalid command!\n");
        printf("Error! Invalid command!\n");
    }
}

/* It prints the outputs according to the print commands */
void printingTheContent(char *line,struct file *all,int numberOfFile,FILE *out){

    char *command = "print";
    int length = (int) strlen(command);
    int lengthOfLine = (int) strlen(line);
    int i ,j, bool=0;


    for (i=0 ; i<length ; i++){
        if (line[i] == command[i]){
            bool++ ;
        }
    }
    if (bool == length){
        for (i=0,j=1; j<lengthOfLine ; i++,j++){
            if (line[i]== '-' && line[j]== 'n' && line[j+1] == ' '){ /* Finding command -n */
                fprintf(out,"%s",line);
                char *name ,*extension, **temp;
                temp = takingPartsOfLine(line);
                name = temp[0] ; extension = temp[1]; /* It takes name and content */
                free(temp);
                printf("%s",line);
                int x,y ;
                for (x=0, y=1 ; y<lengthOfLine ;x++,y++){
                    if (line[x]== '-' && line[y]== 't'){ /* It finds the "-n name.extension -t" command */
                        int print;
                        for (print=0;print<numberOfFile;print++){
                            if(!(strcmp(name,all[print].name)) && !(strcmp(extension,all[print].extension))){
                                printf("Text: %s\n",all[print].content);
                                fprintf(out,"Text: %s\n",all[print].content);
                                break;
                            }
                        }
                        break;
                    }
                    else if (line[x]== '-' && line[y]== 'c' && line[y+1] == 'w' && line[y+2] == ' '){ /* It finds the "-n name.extension -cw" command */
                        char *cont;
                        int size=0; /* To find size of the word what will be counted */
                        int match;
                        for (match = y+3; ;match++){
                            if(line[match] == ' ' || line[match] == '\0' || line[match] == '\n'){
                                break;
                            }
                            size++; /* It measures the the size of word that will be counted */
                        }
                        
                        int cw ;
                        cont = (char *) calloc(size, sizeof(char)); /* Allocating for the word */
                        for (cw=0 ,match =y+3; cw<size ; cw++,match++){
                            cont[cw] = line[match]; /* It takes the word that will be counted */
                        }
                        
                        int print = 0;
                        for (print=0;print<numberOfFile;print++){
                            if(!(strcmp(name,all[print].name)) && !(strcmp(extension,all[print].extension))){
                                int number = countingAword(all[print].content,cont); /* To determine how many word are there in the content */
                                printf("Text: %s\nNumber Of Occurrence of %s : %d\n",all[print].content,cont,number);
                                fprintf(out,"Text: %s\nNumber Of Occurrence of %s : %d\n",all[print].content,cont,number);
                                break;
                            }                            
                        }
                        free(cont);
                        break;
                    }
                    else if (line[x]== '-' && line[y]== 'c' && line[y+1] == 's'){
                        int print;
                        for (print=0;print<numberOfFile;print++){
                            if(!(strcmp(name,all[print].name)) && !(strcmp(extension,all[print].extension))){

                                int iter,cs=0; /* cs is for count sentence */
                                for (iter=0 ; iter < strlen(all[print].content) ; iter++){
                                    if((all[print].content)[iter] == '.' || (all[print].content)[iter] == '?' || (all[print].content)[iter] == '!'){
                                        cs +=1; /* It counts every sentence */
                                    }
                                }
                                printf("Text: %s\nNumber Of Sentences : %d\n",all[print].content,cs);
                                fprintf(out,"Text: %s\nNumber Of Sentences : %d\n",all[print].content,cs);
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
            else if (line[i]== '-' && line[j]== 'a'){ /* It finds the "-a" command */
                fprintf(out,"%s",line);
                printf("%s",line);
                int print;
                for (print =0 ; print<numberOfFile ; print++){
                    printf("Filename %d: %s.%s\n",all[print].fileNumber,all[print].name,all[print].extension);
                    fprintf(out,"Filename %d: %s.%s\n",all[print].fileNumber,all[print].name,all[print].extension);
                }
                break;
            }
            else if (line[i]== '-' && line[j]== 'e' && line[j+1] == ' '){ /* It finds the -e command */
                fprintf(out,"%s",line);
                printf("%s",line);
                char *ext = NULL; /* To keep extension part that was given in command */
                int size=0 ; /* To find size of the extension */
                int temp ;
                for (temp = j+2; ;temp++){
                    if(line[temp] == ' ' || line[temp] == '\0' || line[temp] == '\n'){
                        break;
                    }
                    size++;
                }
                int iter;
                ext = (char *) calloc(size,sizeof(char)); /* Allocating for memory for temporary extension part */
                for(iter=0,temp=j+2;iter<size;iter++,temp++){
                    ext[iter] = line[temp]; /* To find extension's name */
                }
                for (iter=0 ; iter< numberOfFile ; iter++){
                    if (!(strcmp(ext,all[iter].extension))){
                        printf("Filename %d: %s\nText: %s\n",iter+1,all[iter].name,all[iter].content);
                        fprintf(out,"Filename %d: %s\nText: %s\n",iter+1,all[iter].name,all[iter].content);
                    }
                }
                free(ext);
                break;
            }
            else if(line[i]== '-' && line[j]== 'c' ){
                fprintf(out,"%s\n",line);
                printf("%s\n",line);
                int print;
                for (print =0 ; print<numberOfFile ; print++){
                    printf("Num: %d\nName: %s\nText: %s\n",all[print].fileNumber,all[print].name,all[print].content);
                    fprintf(out,"Num: %d\nName: %s\nText: %s\n",all[print].fileNumber,all[print].name,all[print].content);
                }
                break;
            }
        }
    }
    else {
        printf("Error! Invalid command!\n");
    }

}

/* Allocating the every pointer in the linesOfFile up to size of line. It takes 4 parameters */
/* Parameters : indexOfFile (pointer to pointer) , indexOfFile , Number of Bytes , Numbers of Line */
void allocatingForLines (char **linesOfFile, const char *indexOfFile, long numberOfByte, int numberOfLine) {

    int i, j = 0 ;
    int sizeOfLine; /* A variable that learns the size of line. */

    for (i = 0; i < numberOfLine; i++) { /* Iterator goes to the end of file. */
        sizeOfLine = 0; /* The value is zero each time, then it will increment. */
        for ( ; j < numberOfByte; j++) {
            if (indexOfFile[j] == '\n') {
                j++;
                break;
            } else {
                sizeOfLine += 1; /* It increments  until sees the \n character. */
            }
        }
        linesOfFile[i] = (char *) malloc(sizeOfLine*sizeof(char) ); /* Allocating every pointer according to the size of related line. */
    }

    int m, k, n = 0; /* Iterators for filling the every pointer's gap with a character from the lines. */

    for (m = 0; m < numberOfLine; m++) {
        for (k = 0; n < numberOfByte; n++, k++) {
            linesOfFile[m][k] = indexOfFile[n];
            if (indexOfFile[n] == '\n') {
                n++;
                break; /* When it sees the \n character, it passes the next pointer to fill it. */
            }
        }
    }
}

/* It seperates every command. And take 3 parameter  */
void specifyingCommands(char **linesOfFiles, int numOfLine, struct file *all,FILE *output){
    int i , numberOfFiles;
    numberOfFiles = 0; /* To learn how many file will be created or deleted */

    for (i=0 ; i<numOfLine ; i++){
        switch (linesOfFiles[i][0]){  /* It controls the first character in commands. Then it seperates the commands. */
            case 'c' :
                numberOfFiles = creatingFiles(linesOfFiles[i],numberOfFiles,all,output) ;
                break;
            case 'a' :
                appendingSthToFiles(linesOfFiles[i],all,numberOfFiles,output) ;
                break;
            case 'd' :
                numberOfFiles = deletingFiles(linesOfFiles[i],numberOfFiles,all,output);
                break;
            case 'p' :
                printingTheContent(linesOfFiles[i],all,numberOfFiles,output) ;
                break ;
            case 'r' :
                if (linesOfFiles[i][2] == 'p') {
                    replacingSthInFiles(linesOfFiles[i],all,numberOfFiles,output);
                    break;
                }
                else if (linesOfFiles[i][2] == 'm') {
                    removingSthInFiles(linesOfFiles[i],all,numberOfFiles,output);
                    break;
                }

            default:
                printf("Error! Invalid command!\n") ; /*If the command is invalid, it will give the Error Message. */
                break ;
        }
    }
}

int main(int argc, char *argv[]) {

    FILE *fileInput,*fileOutput;
    fileInput = fopen(argv[1],"r");
    fileOutput = fopen("output.txt","w");

    char *indexOfFile ; /* A pointer to take everything in fileInput */
    long numberOfBytes ; /* The number will take number of the bytes all input file for allocating. */
    int numbersOfLines ; /* The number of lines of Input.txt */

    fseek(fileInput, 0L, SEEK_END);
    numberOfBytes = ftell(fileInput); /* To learn how many bytes there are in Input.txt */

    fseek(fileInput, 0L, SEEK_SET); /* It sets the file pointer to starting point in the file. */
    indexOfFile = (char *) calloc(numberOfBytes, sizeof(char)); /* Allocate the memory */

    fread(indexOfFile, sizeof(char), (size_t) numberOfBytes, fileInput); /* To read all data in the pointer that name is indexFile*/

    fclose(fileInput); /* To close Input.txt */

    numbersOfLines = toLearnNumberOfLine(indexOfFile,numberOfBytes);

    char **linesOfFile ; /* A pointer to pointer that takes every line in a pointer. */
    linesOfFile = (char**)calloc(numbersOfLines,sizeof(char *)); /* Allocating pointers up to number of lines */

    allocatingForLines(linesOfFile,indexOfFile,numberOfBytes,numbersOfLines);

    struct file *files = (struct file * ) calloc( numbersOfLines  , sizeof(struct file)); /* Firstly allocate with 1, because we don't know how many files there are. */

    specifyingCommands(linesOfFile,numbersOfLines,files,fileOutput);


    free(indexOfFile); /* Deallocating the memory */
    free(linesOfFile); /* Deallocating the memory */
    free(files); /* Deallocating the memory */
    fclose(fileOutput);

    return 0;
}