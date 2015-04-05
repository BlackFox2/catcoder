/*
* Name: bowling.c
* Author: Markus Marihart
* Date: 17.03.2015
*
* Program for calculating the score in a given bowling game.
* As input it uses the number of how many houses/frames should be calculated and the
* results for every throw.
* The program covers in the special bonus points for strikes and spares.
* The output is the score for every round
*/
#include <stdio.h>

#define FRAMES 10		// Number of frames to calculate

int main(int argc, char* argv[]) {
    /* Input array with scores for every throw */
    int scores[] = { 7, 2, 1, 9, 6, 4, 5, 5, 10, 3, 7, 7, 3, 6, 4, 10, 2, 8, 6 };
    int points[FRAMES];		/* Array for score for every round */
    int points_sum = 0;		/* Sum of all previous frames/houses */
    int points_frame = 0; /* Points of actual frame/house */
    int throws = 2;				/* Remaining throws in this frame/house */
    int i = 0;
    int frame_act = 0;		/* Actual frame/house to calculate */
    int count = 0;				/* boolean if points should be counted now (strikes) */

    while (frame_act < FRAMES) {	/* do for the specified number of frames/houses */
        if (throws == 0 || count == 1) {	/* check if frame/house is finished or striek */
            points[frame_act] = points_sum + points_frame;
            points_sum += points_frame;
            points_frame = 0;
            count = 0;
            throws = 2;
            frame_act++;
        }
        if (scores[i] == 10) {	/* strike! */
            points_frame += 10 + scores[i + 1] + scores[i + 2];
            count = 1;	/* there's no second throw in this frame/house */
        }	/* spare! */
        else if (throws - 1 != 0 && scores[i] + scores[i + 1] == 10) {
            points_frame += scores[i] + scores[i + 1] + scores[i + 2];
            count = 1;
            i++;	/* we have to skip the next one because it's already included */
        }
        else {	/* normal points */
            points_frame += scores[i];
        }
        throws--;
        i++;
    }

    /* create output */
    for (int i = 0; i < FRAMES; i++) {
        if (i + 1 < FRAMES)
            printf("%d,", points[i]);
        else
            printf("%d", points[i]);
    }
    printf("\n");

    return 0;
}