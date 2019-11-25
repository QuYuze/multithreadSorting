/*
 * Multithread Sorting
 *
 * -The programs takes in a file of integers seperated by commas then
 * -parse the stream into a list
 * -split the list into two sublist, and sort them respectively
 * -create threads to launch sorting
 * -using bubble sort to sort the sublists, may change to a sorting algorithm with more favorable
 *  running time in the future
 * -merge the sublist and store the result in a new list
 * -save and output the sorted list to another file stream
 *
 * This is entirely my own work.
 *
 * Last modified:
 * Octerober.24.2019
 *
 * Author:
 * Qu Yuze  B00713367
 *
 * Dalhousie Univeristy
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define DELIMETER ","

int ary[600];
int mergedList[600];
int low, middle, high;

typedef struct{
    int starting_index;
    int ending_index;
}parameters;

void * sorter(void * params);
void * merger(void * params);

int main() {
    //read from the file, then tokenlize the stream
    FILE * inputFile;
    inputFile = fopen("IntegerList.txt", "r");
    char * token;
    int i = 0;
    if(inputFile != NULL){
        char line[600];
        while(fgets(line, sizeof(line), inputFile) != NULL){
            token = strtok(line, DELIMETER);
            while(token != NULL){
                ary[i] = atoi(token);
                i++;
                token = strtok(NULL, DELIMETER);
            }
        }
    }
    fclose(inputFile);

    /*first sublist: from low to middle
     * second sublist: from middle+1 to high
     */
    low = 0;
    high = i - 1;
    middle = high/2;

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_attr_t attr1;
    pthread_attr_t attr2;
    pthread_attr_t attr3;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    //create thread and launches the sorting
    parameters * data1 = (parameters *) malloc(sizeof(parameters));
    data1 ->starting_index = low;
    data1 ->ending_index = middle;
    int error1;
    error1 = pthread_create(&tid1, &attr1, sorter, (void*)data1);
    if(error1 != 0){
        printf("cant create thread: %s\n", strerror(error1));
    }

    parameters * data2 = (parameters *) malloc(sizeof(parameters));
    data2 ->starting_index = middle + 1;
    data2 ->ending_index = high;
    int error2;
    error2 = pthread_create(&tid2, &attr2, sorter, (void*)data2);
    if(error2 != 0){
        printf("cant create thread: %s\n", strerror(error2));
    }

    parameters * data3 = (parameters *) malloc(sizeof(parameters));
    data3 ->starting_index = middle + 1;
    data3 ->ending_index = high;
    int error3;
    error3 = pthread_create(&tid3, &attr3, merger, (void*)data3);
    if(error3 != 0){
        printf("cant create thread: %s\n", strerror(error3));
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    //output to the output file stream
    FILE * outputFile = fopen("SortedIntegerList.txt", "w");
    int j;
    for (j = 0; j < i-1; j++) {
        fprintf(outputFile, "%d,", mergedList[j]);
    }
    fprintf(outputFile, "%d", mergedList[j]);

    fclose(outputFile);
    free(data1);
    free(data2);
    free(data3);

    return 0;
}

/*using bubble sort to sort array
 * takes in the paramter struct as input
 * sort the potion of the array respect to the index pointers
 */
void * sorter(void * params){
    parameters * p = (parameters*)params;
    int i, j, temp;
    int l = p ->starting_index;
    for(i = p ->starting_index; i < p ->ending_index; i++){
        for(j = p ->starting_index; j < p ->ending_index-i+l; j++){
            if(ary[j] > ary[j+1]){
                temp = ary[j];
                ary[j] = ary[j+1];
                ary[j+1] = temp;
            }
        }
    }

    pthread_exit(0);
}

/*merge the sublist
 * and load the output to the second integer list
 */
void * merger(void * params){
    parameters * p = (parameters*)params;
    int i = 0;
    int j = p ->starting_index;
    int k = 0;

    while(k < p ->ending_index+1){
        if(ary[i] < ary[j]){
            if(i < p ->starting_index+1){
                mergedList[k] = ary[i];
                i++;
            }else{
                mergedList[k] = ary[j];
                j++;
            }
            k++;
        }else{
            if(j < p ->ending_index +1){
                mergedList[k] = ary[j];
                j++;
            }else{
                mergedList[k] = ary[i];
                i++;
            }
            k++;
        }
    }
    pthread_exit(0);
}