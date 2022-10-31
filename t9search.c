//
//  t9search.c
//  t9search
//
//  Created by Filip Matus on 16.10.2022.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>


void filterContacts(int contactsLength, char contacts[][2][100], char filter[], char printContacts[][2][100]);
bool filterNumber(char number[], char filter[]);
bool filterName(char name[], char filter[]);
void printAllContacts(char contacts[][2][100], int length);

int main(int argc, char * argv[]) {
    
    if(argc > 2){
        printf("ERROR too many arguments!!!\n");
        return 0;
    }
    
    int c;
    int i = 0; // index for array where to write letter to contacts[][][] (not formatted)
    int y = 0; // index for array where to write letter to fContacts[][][] (formmatted)
    int loadName = 0; // if this is 0 then loading text is name. if it is 1 loading text is number
    int contactsCount = 0;
    
    char fContacts[100][2][100];//fcontacts = formatted contacst
    char contacts[100][2][100];// not formatted contacts
    
    while((c=getchar())!=EOF){ //reading from input (file)
        
        if(c == '\n'){//end of line = name or number is loaded
            if(loadName == 0){
                loadName = 1; // name is loaded, change loading for number
            }else{
                loadName = 0; // number is loaded, change loading for name
                contactsCount ++;
            }
            i = 0;//index for not formatted array
            y = 0;//index for formatted array
            
        }else{
            
            if(c != 32){
                fContacts[contactsCount][loadName][y] = tolower(c);// add c(name) to formatContacts[]
                y++;
            }
            contacts[contactsCount][loadName][i] = c; //add c (name) to contacts (not formatted)
            i++;
        }
        if(i>100){//check line is not longer then 100
            printf("ERROR, wrong input data format!!!\n");
            return 0;
        }
    }
    
    if(contactsCount == 0){ //if file is empty
        printf("ERROR no contact in the file!!!\n");
        return 0;
    }
    
    if(argc == 1){//filter was not inserted
        printAllContacts(contacts, contactsCount);
        return 1;
        
    }else{
        int filterLength = (int)strlen(argv[1]);
        char filter[filterLength]; //create array for filter
        strcpy(filter, argv[1]); //set filter to array
        for (int i = 0; i < filterLength; i++) {
            if(filter[i] < 47 || filter[i] > 57){ //check filter is number
                printf("Error, wrong filter!!!\n");
                return 0;
            }
        }
        filterContacts(contactsCount, fContacts ,filter, contacts);//filter function
    }
    return 1;
}

void filterContacts(int contactsLength, char contacts[][2][100], char filter[], char printContacts[][2][100]){
    int resultsCount = 0;
    
    for (int i = 0; i < contactsLength; i++) {
        if(filterNumber(contacts[i][1], filter) || filterName(contacts[i][0], filter)){ //if contacts are filtred out by name or number then print it
            printf("%s, %s\n", printContacts[i][0], printContacts[i][1]);
            resultsCount++;
        }
    }
    if(resultsCount == 0){ //no contact was filtred out
        printf("NOT FOUND\n");
    }
}

bool filterNumber(char number[], char filter[]){
    bool match = false;
    int fIndex = 0; //filter index
    bool previousMatch = false;
    
    for (int i = 0; i < (int)strlen(number); i++) {
        if(filter[fIndex] == number[i] || (i == 0 && filter[0] == '0' && number[i] == '+')){ //first char of the contact can be + (+420...)
            fIndex ++;
            previousMatch = true;
        }else{
            fIndex = 0;//reset filter index
            if(previousMatch){
                i--;
                previousMatch = false;
            }
        }
        if(fIndex == (int)strlen(filter)){
            match = true;
            break;
        }
    }
    return match;
}

bool filterName(char name[], char filter[]){
    bool match = false;
    int index = 0;
    char options[4];
    bool previousMatch = false;

    for (int i = 0; i < (int)strlen(name); i++) { //for every letter in name
        char help = filter[index];
        int number = atoi(&help); //number to integer
        
        if((int)filter[index] < 55){//for number 1-6
            options[0] = 91 + number * 3;
            options[1] = 92 + number * 3;
            options[2] = 93 + number * 3;
            options[3] = 0;
        }else if((int)filter[index] == 55){ //for number 7
            options[0] = 112;
            options[1] = 113;
            options[2] = 114;
            options[3] = 115;
        }else if((int)filter[index] == 56){//for number 8
            options[0] = 116;
            options[1] = 117;
            options[2] = 118;
            options[3] = 0;
        }
        else if((int)filter[index] == 57){//for number 9
            options[0] = 119;
            options[1] = 120;
            options[2] = 121;
            options[3] = 122;
        }
        if(name[i] == options[0] || name[i] == options[1] || name[i] == options[2] || name[i] == options[3]){
            index++;
            previousMatch = true;
        }else{
            index = 0;
            if(previousMatch){
                i --;
                previousMatch = false;
            }
        }
        if(index == (int)strlen(filter)){ //if index == length of the filter, then it is MATCH
            match = true;
            break;
        }
    }
    return match;
}

void printAllContacts(char contacts[][2][100], int length){
    printf("ALL CONTACTS: \n");
    for (int i = 0; i < length; i++) {
        printf("%s, %s\n", contacts[i][0], contacts[i][1]);
    }
}
