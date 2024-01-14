#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;
//my variables
//resized bSize
int cSize;
//bool for if finished
int finished = 0;
//how many loops gone through
int loops = 0;
//final order
int finalorder = 0;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
    //prints puzzle

    //this was for testing if resizing array worked
    int dSize = bSize;
    if(*(*(arr)) == '\0'){ dSize += 2; }
    //simple i j for loop going through size of puzzle
    for(int i = 0; i < dSize; i++){
        for(int j = 0; j < dSize; j++){
            //used for testing resized array
            if(*(*(arr+i)+j) != '\0'){
                //print value of arr at position i j
                printf("%c ", *(*(arr+i)+j));
            }
        }
        printf("\n");   
    }
    printf("\n");
}

void printResult(int** arr,int size) {
    //prints the result excluding the boundary bc of resizing
    for(int i = 1; i < cSize-1; i++){
        for(int j = 1; j < cSize-1; j++){
            printf("%d\t", *(*(arr+i)+j));
        }
        printf("\n");   
    }
}

//appends the steps into one int to be stored in array
//credit & inspiration from https://www.programminglogic.com/how-to-concatenate-two-integers-in-c/
int resultappend(int x, int y){
    //make 2 char strings
    char *result = (char*)malloc(20 * sizeof(char));
    char *newresult = (char*)malloc(20 * sizeof(char));
    //input int values into strings
    sprintf(result,"%d",x);
    sprintf(newresult,"%d",y);
    //combine strings
    strcat(result,newresult);
    //return as an int
    return atoi(result);
}

//finds length of word
int findLength(char* word){
    char x = *(word);
    int clen = 0;
    int k = 1;
    while(x != '\0'){
        clen++;
        x = *(word+k);
        k++;
    }
    return clen;
}

//recursive search
int sweepSearch(char** arr, int ox, int oy, int x, int y, char* word,int order, int** result,int len){
    //checks how many loops we've been through
    loops++;
    //checks if word has been found
    if(finished == 1){
        //testing
        //printf("RETURNING!\n");
        finished = 1;
    } else {
        //increases order (spot in word)
        order++;
        //saves old value in case of dead end
        int oldnum =  *(*(result+x)+y);
        //appends the current order to the correct spot in path
        *(*(result+x)+y) = resultappend(*(*(result+x)+y),order);

        //testing
        // printResult(result,cSize);
        // printf("\n");
        // printf("Order, Length: %d, %d\n",order, len);
        // printf("\n");

        //checks if the spot we are in word is equal to the size of the word
        if(order == len){
            //sets global finished variable to one
            finished = 1;
            //printf("Finished\n");
            //sets global variable finalorder to order, to be used in later comparisons
            finalorder = order;
        }
        //loops through the 8 spaces around the first letter
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                //checks that we aren't in the middle
                if(i != 0 || j != 0){
                    //checks if the spot in arr is equal to the spot in word (excluding border)
                    if(*(*(arr+x+i)+y+j) == *(word+order) && *(*(arr+x+i)+y+j) != '\0'){
                            //recurse
                            sweepSearch(arr,ox,oy,x+i,y+j,word, order, result,len);
                    }
                }
            }
        }
        //if we have stopped recursing and finish is not 1, backtrack
        if (finished == 0 || loops == (len && loops > order)){
            order = 0;
            //set current updated spot in path back to what it was before.
            *(*(result+x)+y) =  oldnum;
            //printf("Reset\n");
        }
    }
    return 0;
}

void searchPuzzle(char** arr, char* word) {
    //sets size of puzzle to +2
    cSize = bSize+2;
    //sets order to 0
    int order = 0;
    //finds word length
    int wordlen = findLength(word);

    //new 2d array identical to old one, but 1 bigger on each side, edges filled with '\0' as to not have issues checking out of bounds
    char **newarr = (char**)malloc(cSize * sizeof(char*));
    for(int i = 0; i < cSize; i++){
        *(newarr+i) = (char*)malloc(cSize * sizeof(char));
        for(int j = 0; j < cSize; j++){
            if(i == 0 || i == cSize-1 || j == 0 || j+1 == cSize){
                *(*(newarr+i)+j) = '\0';
            } else {
                *(*(newarr+i)+j) = *(*(arr+i-1)+j-1);
            }
            
        }
    }

    //result array, same size as new array, filled with zeros
    int **result = (int**)malloc(cSize * sizeof(int*));
    for(int i = 0; i < cSize; i++){
        *(result+i) = (int*)malloc(cSize * sizeof(int));
        for(int j = 0; j < cSize; j++){
            *(*(result+i)+j) = 0;
        }
    }

    //capitalize word
    for(int i = 0; i < wordlen; i++){
        if (*(word+i) >= 'a' && *(word+i) <= 'z'){
            *(word+i) += 'A' - 'a';
        }
    }

    //the main loop
    for(int i = 0; i < cSize; i++){
        for(int j = 0; j < cSize; j++){
            //check if current spot is equal to fist letter in word
            if(*(*(newarr+i)+j) == *(word)){
                //start recurse
                sweepSearch(newarr, i, j, i, j, word, order, result, wordlen);
                //after recurse is fully done, check if final order is equal to the word length
                //meaning the path has the full word
                if(finalorder == wordlen){
                    //print word found and exit loop
                    printf("Word Found!\n");
                    goto print;
                } else {
                    //reset order
                    order = 0;
                    //reset result array
                    for(int i = 0; i < cSize; i++){
                        for(int j = 0; j < cSize; j++){
                            *(*(result+i)+j) = 0;
                        }
                    }  
                } 
            } 
        }
    }
    //if path was found, print the table
    print: if(finalorder == wordlen){
        printf("Printing the seach path:\n");
        printResult(result,cSize);
    } else {
        //if not, print word not found
        printf("Word Not Found!");
    }
}