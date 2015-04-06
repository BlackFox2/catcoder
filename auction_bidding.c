//
// Created by blackfox on 05.04.15.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MIN_BID 1
#define BID_STEP 1
#define BIDDERS 2
#define TRUE 1

#define LEVEL 4

char * createOutputArray(char * output, char nextText[], int * index) {
    int x = 0;
    char * temp;
    while(nextText[x] != '\0') {

        x++;
    }
    temp = (char *) malloc(sizeof(char) * (*index+x+1));
    memcpy(temp, output, *index);
    free(output);
    output = temp;
    for(int i = 0; i < x; i++) {
        output[*index+i] = nextText[i];
    }
    output[*index+x] = ',';
    *index = *index+x+1;
    return output;
}


int inverseNumbers(char * numberArray, int count) {
    int helper = 0;
    for(int x = 0; x < count/2; x++) {
        helper = numberArray[x];
        numberArray[x] = numberArray[count-1-x];
        numberArray[count-1-x] = helper;
    }
}


int intToChar(char * array, int number, int inverse) {
    int count = 0;
    for (; number != 0; number = number / 10) {
        array[count++] = (number % 10) + '0';
    }
    array[count] = '\0';
    if(inverse && number >= 10) {
        inverseNumbers(array, count);
    }
}


int getNextBid(char * input, int * offset) {
    int count = 0;
    int bid = 0;
    while(input[count+*offset] != ',' && input[count+*offset] != '\0') {
        count++;
    }
    for(int x = 0; x < count; x++) {
        bid += (input[*offset+x]-'0') * pow(10, count-x-1);
    }
    if(input[count+*offset] == '\0') {
        *offset = *offset + count;
    } else {
        *offset = *offset + count + 1;
    }
    return bid;
}

int getNextBidder(char * input, int *offset, char * act_bidder) {
    int count = 0;
    while(input[count+*offset] != ',' && input[count+*offset] != '\0') {
        count++;
    }

    act_bidder = (char*) realloc(act_bidder, sizeof(char)*count);
    if(act_bidder == NULL) {
        printf("Error reallocating memory!\n");
    }
    for(int x = 0; x < count; x++) {
        act_bidder[x] = input[*offset+x];
    }
    act_bidder[count] = '\0';
    *offset = *offset + count + 1;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        return 0;
    }
    unsigned int price = 0;
    char * highest_bidder;
    char * act_bidder;
    unsigned int bidder_max = 0;

    int i = 0;
    int sum = 0;

    int offset = 0;
    int old_offset = 0;
    int getBidder = 0;
    int size;
    int write = 0;

    #if LEVEL>=3
        char * helperArray;
        char * output;
        int oIndex = 0;
        output = (char *) malloc(sizeof(char)*1);
        int addSize = 0;
    #endif
    #if LEVEL >= 4
        int instantBuy = 0;
        int auctionRunning = 1;
    #endif


    act_bidder = (char*) malloc(sizeof(char)*1);
    highest_bidder = (char*) malloc(sizeof(char)*1);
    if(act_bidder == NULL || highest_bidder == NULL) {
        printf("Error allocating memory!");
        return 0;
    }

    //get start bid
    old_offset = offset;
    price = getNextBid(argv[1], &offset);

    #if LEVEL >= 3
        size = offset - old_offset - 1;
        helperArray = (char *) malloc(sizeof(char) * size);
        output = createOutputArray(output, "-", &oIndex);
        intToChar(helperArray, price, TRUE);
        output = createOutputArray(output, helperArray, &oIndex);
    #endif

    //get instant buy price
    #if LEVEL >= 4
        instantBuy = getNextBid(argv[1], &offset);
    #endif

    getBidder = 1;
    while(argv[1][offset] != '\0') {
        #if LEVEL >= 4
            if(auctionRunning == 0) {
                break;
            }
        #endif
        if(getBidder) {
            old_offset = offset;
            getNextBidder(argv[1], &offset, act_bidder);
            size = offset-old_offset-1;
            getBidder = 0;
            write = 0;
        } else {
            sum = getNextBid(argv[1], &offset);
            getBidder = 1;

            if(sum > bidder_max) {
                write = 1;
                if(memcmp(highest_bidder, act_bidder, size) == 0) {
                    write = 0;
                } else if(bidder_max != 0) {
                    price = bidder_max + 1;
                }
                highest_bidder = (char*) realloc(highest_bidder, sizeof(char)*size);
                memcpy(highest_bidder, act_bidder, 20);

                bidder_max = sum;

            } else if(sum < bidder_max) {
                price = sum+1;
                write = 1;
            } else {
                price = bidder_max;
                write=1;
            }

            #if LEVEL >= 4
            if(price >= instantBuy && instantBuy != 0) {
                price = instantBuy;
                auctionRunning = 0;
            }
            #endif

            sum = 0;
        }

        #if LEVEL >= 3
        if(write != 0) {
            output = createOutputArray(output, highest_bidder, &oIndex);
            helperArray = (char*) realloc(helperArray, sizeof(char)*size);
            intToChar(helperArray, price, TRUE);
            output = createOutputArray(output, helperArray, &oIndex);
        }
        #endif
    }
    #if LEVEL >= 3
        output[oIndex-1] = '\0';
        printf("%s\n", output);
    #else
        printf("%s,%d\n", highest_bidder, price);
    #endif

    return 0;
}
