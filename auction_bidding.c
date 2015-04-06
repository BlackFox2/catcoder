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


void createOutputArray(char output[], int size, char nextText[], int * index) {
    int x = 0;
    while(nextText[x] != '\0') {
        output[*index+x] = nextText[x];
        x++;
    }
    output[*index+x] = ',';
    *index = *index+x+1;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        return 0;
    }
    int input[] = {1,'A',5,'B',10,'A',8,'A',17,'B',17};
    int length = 0;
    int bid_count = 0;
    int bidder_count = 0;
    unsigned int price = 0;
    char highest_bidder[30];
    char act_bidder[30];
    unsigned int bidder_max = 0;

    int bids[10];
    char bidders[5];
    int i = 0;
    int sum = 0;

    int temp = 0;
    char helperArray[200];
    char output[300];
    int oIndex = 0;
    int write = 0;
    int instantBuy = 0;
    int auctionRunning = 1;
    int y = 0;

    //get start bid
    while(argv[1][length] >= '0' && argv[1][length] <= '9') {
        length++;
    }

    for(int x = 0; x < length; x++) {
        price += (argv[1][x]-'0') * pow(10, length-x-1);
    }


    createOutputArray(output, 100, "-\0", &oIndex);
    temp = 0;
    for(int num = price; num != 0; num=num/10) {
        helperArray[temp++] = (num%10)+'0';
    }
    helperArray[temp] = '\0';
    for(int x = 0; x < temp/2; x++) {
        int helper = helperArray[x];
        helperArray[x] = helperArray[temp-1-x];
        helperArray[temp-1-x] = helper;
    }
    createOutputArray(output, 100, helperArray, &oIndex);
    length++;
    y = length;
    //get instant buy price
    while(argv[1][length] >= '0' && argv[1][length] <= '9') {
        length++;
    }
    for(; y < length; y++) {
        instantBuy += (argv[1][y]-'0') * pow(10, length-y-1);
    }

    while(argv[1][length] != '\0' && auctionRunning) {


        if(argv[1][length] >= 'A' && argv[1][length] <= 'Z' || argv[1][length] >= 'a' && argv[1][length] <= 'z') {
            i = length;
            while(argv[1][i] != ',') {
                act_bidder[i-length] = (char) argv[1][i];
                i++;
            }
            act_bidder[i-length] = '\0';
            length = ++i;
            while(argv[1][i] >= '0' && argv[1][i] <= '9') {
                i++;
            }
            for(int x = length; x < i; x++) {
                sum += (argv[1][x]-'0') * pow(10, i-x-1);
            }
            if(sum > bidder_max) {
                write = 1;
                if(memcmp(highest_bidder, act_bidder, 20) == 0) {
                    write = 0;
                } else if(bidder_max != 0) {
                    price = bidder_max + 1;
                }
                memcpy(highest_bidder, act_bidder, 20);

                bidder_max = sum;

            } else if(sum < bidder_max) {
                price = sum+1;
                write = 1;
            } else {
                price = bidder_max;
                write=1;

            }
            if(price >= instantBuy && instantBuy != 0) {
                price = instantBuy;
                auctionRunning = 0;
            }
            if(write != 0) {
                createOutputArray(output, 100, highest_bidder, &oIndex);

                temp = 0;
                for (int num = price; num != 0; num = num / 10) {
                    helperArray[temp++] = (num % 10) + '0';
                }
                helperArray[temp] = '\0';
                for(int x = 0; x < temp/2; x++) {
                    int helper = helperArray[x];
                    helperArray[x] = helperArray[temp-1-x];
                    helperArray[temp-1-x] = helper;
                }
                createOutputArray(output, 100, helperArray, &oIndex);
            }

            sum = 0;
        }

        length++;
    }
    output[oIndex-1] = '\0';
    /*
    for(int i = 1; i < length; i++) {
        if(input[i] >= 'A' && input[i] <= 'Z') {

            if(input[i+1] > bidder_max) {
                price = bidder_max + 1;
                highest_bidder = (char) input[i];
                bidder_max = input[i+1];
            } else if(input[i+1] == bidder_max) {
                price = bidder_max;
            } else {
                price = input[i+1] + 1;
            }
            i++;
        }
    }
*/
   // printf("%s,%d\n", highest_bidder, price);
    printf("%s\n", output);
    return 0;
}
