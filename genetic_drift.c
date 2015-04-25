/*
 * Name: genetic_drift.c
 * Author: Markus Marihart
 * Date: 20.04.2015
 *
 * Program to calculate the genetically distance between two species.
 * Normally scientists use names for the genes but in this case the genes have numerical IDs
 * For further details see the level 1 description from the challenge.
 * In the last level we kind of implement an sorting algorithm for the given permutation. The IDs are sorted ascending.
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LVL 1   //enable different functionality according to the levels (max is 4)

/*
 * This function searches for oriented pairs in the permutation. An oriented pair is a pair of consecutive integers
 * (not necessarily following each other) where the difference of the absolute values is +/- 1. The pair always consists
 * of a negative and a positive number.
 * Notice: the given array must start with the real input, the first number to indicate how long the permutation is
 * has to be cut off.
 * @param:  int permLength:     how many gene ID there are in the array
 *          int * intArray:     array with permutation
 *          int * pairCount:    pointer to save the number of found oriented pairs which is needed later on
 *          int * orientPair:   array where the pairs are stored, x value always first
 * @return: int * orientPair:
 */
int * getOPairs(int permLength, int* intArray, int * pairCount, int* orientPair) {
    int pairIndex = 0;
    for(int i = 0; i < permLength; i++) {
        for(int j = i+1; j < permLength; j++) {
            if (((intArray[i] < 0 && intArray[j] >= 0) || (intArray[i] >= 0 && intArray[j] < 0))
                && (abs(intArray[i]) - abs(intArray[j]) == 1 || abs(intArray[i]) - abs(intArray[j]) == -1)) {
                (*pairCount)++;
                if(*pairCount > 1) {
                    orientPair = (int *) realloc(orientPair, sizeof(int) * (*pairCount*2));
                    if(orientPair == NULL) {
                        printf("Error reallocating memory!\n");
                        return (int *) -1;
                    }
                }
                orientPair[pairIndex++] = intArray[i];
                orientPair[pairIndex++] = intArray[j];
            }
        }

    }
    return orientPair;
}

/*
 * This function is used to sort the array with oriented pairs ascending (x-value, 1st value in array of every pair)
 * @param:  int * array:    pointer to the array with the oriented pairs
 *          int size:       length of the array
 * @return: 0 on success
 */
int sortOrientedPairs(int *array, int size) {
    int helper = 0;
    for(int i = 0; i < size; i += 2) {
        for(int x = i+2; x < size; x += 2) {
            if(array[i] > array[x]) {
                helper = array[i];
                array[i] = array[x];
                array[x] = helper;
                helper = array[i+1];
                array[i+1] = array[x+1];
                array[x+1] = helper;
            }
        }
    }
    return 0;
}

/*
 * This function is used to apply an inversion to the permutation. An inversion means that in genetics a DNA sequence
 * is cut out and inserted again but in reversed order.
 * How the inversion actually is performed depends in our case on the given numbers which.
 * If num1 + num2 = 1 we invert the permutation from index(num1) to index(num2)-1
 * If num1 + num2 = -1 we invert the permutation from index(num1)+1 to index(num2)
 * @params:  int * array:    pointer to the array with the permutation
 *           int num1:       first indicator of the DNA sequence to invert
 *           int num2:       second indicator of the DNA sequence to invert
 * @return: 0 on success
 */
int applyInversionsInt(int * array, int num1, int num2) {
    int index = 0;
    int indexes[4] = {num1, -1, num2, -1};          //here we save the index of the given numbers for the replacement
    int helper = 0;
    int distance = 0;                               //how many IDs are between the indexes of the two given numbers

    while(indexes[1] == -1 || indexes[3] == -1) {   // get the indexes
        if(array[index] == num1) {
            indexes[1] = index;
        } else if(array[index] == num2) {
            indexes[3] = index;
        }
        index++;
    }

    distance = abs(indexes[1] - (indexes[3]));
    if(indexes[0] + indexes[2] == 1) {
        //invert from i to j-1
        for(int x = indexes[1]; x < (ceil((double)distance/2)+indexes[1]); x++) {
            //only need to go half the way with x because we switch the numbers from both ends
            helper = array[x];
            array[x] = array[indexes[3]-1-(x-indexes[1])]*(-1);
            array[indexes[3]-1-(x-indexes[1])] = helper*(-1);
        }
    } else if(indexes[0] + indexes[2] == -1) {
        //invert from i+1 to j
        for(int x = indexes[1]+1; x < ceil((double)distance/2)+indexes[1]+1; x++) {
            //only need to go half the way with x because we switch the numbers from both ends
            helper = array[x];
            array[x] = array[indexes[3]-(x-(indexes[1]+1))]*(-1);
            array[indexes[3]-(x-(indexes[1]+1))] = helper*(-1);
        }
    }
    return 0;
}


/*
 * Function to count how many digits there are until the next given separator char.
 * The function also handles negative numbers and returns a negative count to indicate it.
 * @params:    char * array:    input array from which to count the next count of digits
 *             char separator:  the separator between the numbers
 *             int offset:      where to start from in the array
 * @return:    int count:       count of digits; negative when the number itself is negative
 */
int countDigitsToSep(char * array, char separator, int offset) {
    int count = 0;
    int i = offset -1;
    int sign = 1;
    do {
        i++;
        if (array[i] >= '0' && array[i] <= '9') {
            count++;
        } else if (array[i] == '-') {
            sign = -1;
        }
    }while(array[i] != separator && array[i] != '\0');

    return count*sign;

}

/*
 * This function convert an char array which includes the permutation and a specific separator into an integer array.
 * @params:    char * input:    the input array from which to convert the numbers
 *             char separator:  the separator between the numbers
 *             int * arrayIndex:    count of how many numbers have been converted
 * @return:    int * array:     pointer to the integer array with the numbers
 */
int * charAtoIntA(char * input, char separator, int * arrayIndex) {
    int i = -1;
    int digits = 0;
    int sign;
    int * array = malloc(sizeof(int));
    if(array == NULL) {
        printf("Error allocating memory!\n");
        return (int *) -1;
    }
    *arrayIndex = 0;
    do {
        i++;
        array = realloc(array, sizeof(int) * (*arrayIndex + 1));    //allocate more memory for a new number
        if(array == NULL) {
            printf("Error reallocating memory!\n");
            return (int *) -1;
        }
        digits = countDigitsToSep(input, separator, i);             //count how many digits are the next number
        if (digits < 0) {                                           // it's a negative number
            sign = -1;
            digits *= -1;
            i++;
        } else {
            sign = 1;
        }
        i += digits;
        array[*arrayIndex] = 0;
        for (int x = i - digits; x < i; x++) {                      //convert all the digits into the number
            array[*arrayIndex] += pow(10, digits - (x - (i - digits)) - 1) * (input[x] - '0');
        }
        array[*arrayIndex] *= sign;
        (*arrayIndex)++;
    }while(input[i] != '\0');

    return array;
}

int main(int argc, char* argv[]) {
    int permLength = 0;                 //lenght of the permutation
    int * intArray;                     //pointer to the integer array with the numbers
    int * orientPair;                   //pointer to the memory for the oriented pairs
    int pairCount = 0;
    int size = 0;                       //how many integers have been converted from the input string

    if(argc < 2) {
        printf("Wrong input\n");
        return -1;
    }

    intArray = charAtoIntA(argv[1], ' ', &size);
    permLength = intArray[0];

    orientPair = (int*) malloc(sizeof(int) * 2);
    if(orientPair == NULL) {
        printf("Error allocating memory!\n");
        return -1;
    }

    #if LVL == 1
        getOPairs(permLength, intArray+1, &pairCount, orientPair);
        sortOrientedPairs(orientPair, pairCount*2);
        printf("%d ", pairCount);
        for(int i = 0; i < pairCount*2; i++) {
            printf("%d ", orientPair[i]);
        }
    #elif LVL == 4
        getOPairs(permLength, intArray+1, &pairCount, orientPair);
        sortOrientedPairs(orientPair, pairCount * 2);
        int num2;
        int num1;
        int steps = 0;
        while(pairCount > 0) {
            num2 = orientPair[pairCount*2-1];
            num1 = orientPair[pairCount*2-2];
            applyInversionsInt(intArray, num1, num2);
            pairCount = 0;
            orientPair = getOPairs(permLength, intArray+1, &pairCount, orientPair);
            sortOrientedPairs(orientPair, pairCount * 2);
            steps++;
        }
        printf("%d\n", steps);
    #elif LVL >= 2
        applyInversionsInt(intArray, intArray[size-4], intArray[size-2]);
        #if LVL == 2
        for(int i  = 1; i <= permLength; i++) {
            printf("%d ", intArray[i]);
        }
        #elif LVL == 3
            pairCount = 0;
            getOPairs(permLength, intArray+1, &pairCount, orientPair);
            sortOrientedPairs(orientPair, pairCount*2);
            printf("%d", pairCount);
        #endif

    #endif

    free(intArray);
    free(orientPair);

    return 0;
    
}
