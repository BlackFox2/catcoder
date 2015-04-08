/*
 * Name:
 * Author: Markus Marihart
 * Date: 06.04.2015
 * Version:
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TRUE 1      // pseudo boolean

#define LEVEL 4     //enable different functionality/behaviour according to the actual level


/*
 * This function writes a given text into the global output array which is needed in levels higher than 2.
 * As the size of the output array isn't known in the beginning we have to increase the allocated memory every time
 * we add more characters to the output array.
 * We need to keep track of how much is already written into the output so we don't override the characters. Therefore
 * the global index variable is used.
 * @param:  char * output:      pointer to the global output array
 *          char nextText[]:    text which should be added to the output
 *          int * index:        pointer to the global index of the output array
 * @return: char * output:      new pointer to the output array
 */
char * createOutputArray(char * output, char nextText[], int * index) {
    int x = 0;
    char * temp;
    while(nextText[x] != '\0') {
        x++;
    }
    temp = (char *) malloc(sizeof(char) * (*index+x+1));
    if(temp == NULL) {
        printf("Error allocating memory!");
        return '\0';
    }
    memcpy(temp, output, (size_t) *index);
    free(output);
    output = temp;
    for(int i = 0; i < x; i++) {
        output[*index+i] = nextText[i];
    }
    output[*index+x] = ',';
    *index = *index+x+1;        //increase the index more so we we get to the next data after the , in our input
    return output;
}

/*
 * When working with numbers with several digits it might be needed to revert the order of the digits. This function
 * accomplishes exactly what we want.
 * @param:  char * numberArray: array with the digits to revert
 *          int count:          how many digits there are
 * @return: 0 if everything went alright
 */
int inverseNumbers(char * numberArray, int count) {
    char helper = 0;
    for(int x = 0; x < count/2; x++) {
        helper = numberArray[x];
        numberArray[x] = numberArray[count-1-x];
        numberArray[count-1-x] = helper;
    }
    return 0;
}

/*
 * To output integers we need to convert them into character strings. This function takes a pointer where to write the
 * characters and the number to convert. There's also the possibility to invert them because the order of the digits
 * is reverted.
 * @param:  char * array:   where to write the characters
 *          int number:     the number to convert
 *          int inverse:    whether or not to invert the characters
 * @return: 0 if everything went alright
 */
int intToChar(char * array, int number, int inverse) {
    int count = 0;
    for (; number != 0; number = number / 10) {
        array[count++] = (char) ((number % 10) + '0');
    }
    array[count] = '\0';
    if(inverse && number >= 10) {
        inverseNumbers(array, count);
    }
    return 0;
}

/*
 * This function extracts the value of the next bid from the given input. The value is bounded by the character ','.
 * The offset has to be at the beginning of the value.
 * @param:  char * input:   input array from where to extract the value
 *          int * offset:   where to start
 * @return: unsigned int bid:   the bid the last bidder made
 */
unsigned int getNextBid(char * input, int * offset) {
    int count = 0;
    unsigned int bid = 0;
    while(input[count+*offset] != ',' && input[count+*offset] != '\0') {
        count++;
    }
    for(int x = 0; x < count; x++) {
        bid += (input[*offset+x]-'0') * pow(10, count-x-1);
    }
    if(input[count+*offset] == '\0') {
        *offset = *offset + count;  //if it is the last bid we should not increase the offset by 1 more.
    } else {
        *offset = *offset + count + 1; //increase the offset by 1 more so we are over the next ','
    }
    return bid;
}

/*
 * This function extracts the name of the next bidder from the given input. The name is bounded by the character ','.
 * The offset has to be at the beginning of the name.
 * @param:  char * input:   input array from where to extract the name
 *          int * offset:   where to start
 *          char * bidder:  pointer where to write the name of the bidder
 * @return: 0 if everything went alright
 */
int getNextBidder(char * input, int *offset, char * bidder) {
    int count = 0;
    while(input[count+*offset] != ',' && input[count+*offset] != '\0') {
        count++;
    }

    bidder = (char*) realloc(bidder, sizeof(char)*count);
    if(bidder == NULL) {
        printf("Error reallocating memory!\n");
        return -1;
    }
    for(int x = 0; x < count; x++) {
        bidder[x] = input[*offset+x];
    }
    bidder[count] = '\0';
    *offset = *offset + count + 1;
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {      /* usage check */
        return 0;
    }
    unsigned int price = 0; /* the actual price you have to pay */
    char * highest_bidder;  /* name of the highest bidder at the moment */
    char * act_bidder;      /* name of the actual bidder we are computing */
    unsigned int bidder_max = 0;    /* value the highest bidder is ready to pay */

    unsigned int bid = 0;   /* bid of the bidder we are computing */

    int offset = 0;         /* offset to work in the input string */
    int old_offset = 0;     /* old_offset to calculate length of value or name */
    int getBidder = 0;      /* alternating to extract bidder and bid from the input string */
    int size;               /* size value for allocating memory*/
    int write = 0;          /* only write if when computation of actual bidder is finished */

    #if LEVEL>=3            /* enable functionality for level 3 */
        char * helperArray; /* array for converting integers to char */
        char * output;      /* global output array */
        int oIndex = 0;     /* writing index of global output array */
        output = (char *) malloc(sizeof(char)*1);
        if(output == NULL) {
            printf("Error allocating memory!");
            return -1;
        }
    #endif
    #if LEVEL >= 4          /* enable functionality for level 4 */
        unsigned int instantBuy = 0;    /* value to achieve an instant buy */
        int auctionRunning = 1;         /* we need to have the possibility to abort the auction when someone buys instant */
    #endif

    /* initialize pointers so realloc won't fail */
    act_bidder = (char*) malloc(sizeof(char)*1);
    highest_bidder = (char*) malloc(sizeof(char)*1);
    if(act_bidder == NULL || highest_bidder == NULL) {
        printf("Error allocating memory!");
        return 0;
    }

    /* get start bid (minimal price) */
    old_offset = offset;
    price = getNextBid(argv[1], &offset);

    #if LEVEL >= 3
        size = offset - old_offset - 1;
        helperArray = (char *) malloc(sizeof(char) * size); /* adjust size of array to needed size */
        if(helperArray == NULL) {
            printf("Error allocating memory!");
            return -1;
        }
        output = createOutputArray(output, "-", &oIndex);   /* first "bidder" is '-' */
        intToChar(helperArray, price, TRUE);
        output = createOutputArray(output, helperArray, &oIndex);
    #endif

    /* get instant buy price */
    #if LEVEL >= 4
        instantBuy = getNextBid(argv[1], &offset);
    #endif

    getBidder = 1;
    while(argv[1][offset] != '\0') {    /* do as long as there are more bids */
        #if LEVEL >= 4
            if(auctionRunning == 0) {   /* abort if someone hit the instant buy price */
                break;
            }
        #endif
        if(getBidder) {     /* extract the next bidder */
            old_offset = offset;
            getNextBidder(argv[1], &offset, act_bidder);
            size = offset-old_offset-1;
            getBidder = 0;
            write = 0;
        } else {            /* extract the next bid */
            bid = getNextBid(argv[1], &offset);
            getBidder = 1;

            if(bid > bidder_max) {  /* new highest bid, actual bidder is highest */
                write = 1;
                if(memcmp(highest_bidder, act_bidder, (size_t) size) == 0) {
                    /* no need to write something when highest bidder only adjusted his bid */
                    write = 0;
                } else if(bidder_max != 0) {
                    /* price is now 1 higher than the maximum bid from the old highest bidder */
                    price = bidder_max + 1;
                }
                highest_bidder = (char*) realloc(highest_bidder, sizeof(char)*size);
                if(highest_bidder == NULL) {
                    printf("Error reallocating memory!");
                    return -1;
                }
                memcpy(highest_bidder, act_bidder, (size_t) size);

                bidder_max = bid;

            } else if(bid < bidder_max) {   /* actual bid is smaller than highest pid */
                price = bid+1;  /* price is now 1 higher than the actual bid */
                write = 1;
            } else {            /* bid is exactly the same as actual highest bid */
                price = bidder_max; /* the price increases but the highest bidder doesn't change (first come) */
                write=1;
            }

            #if LEVEL >= 4
            if(price >= instantBuy && instantBuy != 0) {
                price = instantBuy;
                auctionRunning = 0;
            }
            #endif

        }

        #if LEVEL >= 3
        if(write != 0) {    /* write bidder and bid to the output array */
            output = createOutputArray(output, highest_bidder, &oIndex);
            helperArray = (char*) realloc(helperArray, sizeof(char)*size);
            if(helperArray == NULL) {
                printf("Error reallocating memory!");
                return -1;
            }
            intToChar(helperArray, price, TRUE);
            output = createOutputArray(output, helperArray, &oIndex);
        }
        #endif
    }
    #if LEVEL >= 3      /* print information required according to level */
        output[oIndex-1] = '\0';
        printf("%s\n", output);
    #else
        printf("%s,%d\n", highest_bidder, price);
    #endif

    free(highest_bidder);
    free(act_bidder);
    free(helperArray);
    free(output);

    return 0;
}
