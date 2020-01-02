//
// Created by ethan on 12/14/2019.
//

#ifndef EX1_JUDGE_H
#define EX1_JUDGE_H

#include<stdbool.h>

/**
 * JUDGE MODULE TESTED SUCCESSFULLY
 * 26/12/19
 **/

/**A datatype for creating a judge in the competition*/
typedef struct judge_t* Judge;

/**A function for creating a new judge*/
Judge judgeCreate (unsigned int judgeId, const char* name);

/**A function for copying a judge*/
Judge judgeCopy(Judge j);

/**A function that returns true if j1=j2, and false otherwise*/
int judgeCompare (Judge j1, Judge j2);

/**A function which returns j if j->judgeId = judgeId, and NULL otherwise*/
bool compareJudgeToId (Judge j, int judgeId);

/**A function that destroys a judge*/
void judgeDestroy(Judge j);

/**A function which returns a judges id*/
unsigned int judgeGetId(Judge j);

/**A function for printing a judge*/
void judgePrint (Judge j);


#endif //EX1_JUDGE_H
