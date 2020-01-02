//
// Created by iddos on 12/13/2019.
//

#ifndef EX1_STATE_H
#define EX1_STATE_H
#include<stdbool.h>
/**
 * STATE MODULE SUCCESSFULLY TESTED
 * 26/12/19
 **/

typedef struct state_t* State;

/**A function for creating a new state*/
State stateCreate(unsigned int stateId, const char *stateName, const char *songName);

/**A function for creating a copy of an existing state*/
State stateCopy(State s);

/**A function which returns 1 if s2 has a higher score than s1, 0 if they
 * are equal, and -1 if s2 has a lower score than s1 */
int stateScoresCompare(State s1, State s2);


/**A function which calculates a state's total score, depending on @param audiencePercent,
 * and places it into the statesScore field (so that it can be read by the
 * stateCompare function and listSorted in the eurovision contest running functions)*/
void setTotalScore (State s, int audiencePercent);

/**A function which returns a states's id*/
unsigned int stateGetId (State s);

/**A function which returns a state's name*/
char* stateGetName(State s);

/**A function which sets the statesScore value (the audience points) to @param score*/
void setStatesScore (State s, double score);

/**A function which sets the judgesScore value (the judges points) to @param score*/
void setJudgesScore(State s, double score);

/**A function which sets a state's score to be the average score, depending on the number of states and judges*/
void setScoreToAverages (State s, int numOfStates, int numOfJudges);

/**A function which returns true if s->stateId = id, and false otherwise*/
bool stateCompareToId (State s, unsigned int id);

/**A function which returns 0 if the stateIds are equal, and 1 otherwise*/
int stateCompare (State s1, State s2);

/**A function which returns the state if its id is equal to @param stateId, and NULL otherwise
State returnStateById (State s, unsigned int stateId);*/

/**A function which destroys a state*/
void stateDestroy(State state);

/**A function which creates a string of the form state1 - state2 where state1 and state2 are
 * friendly states*/
char* getFriendlyStatesString(State s1, State s2);

/**A function which frees a string of friendly states pair*/
void friendlyStatesPairDestroy(char* str);

/**A function which copies a string of friendly states pair*/
char* friendlyStatesPairCopy(char* str);

/**A function which prints a state*/
void statePrint(State s);

void namefree(char* str);

char* namecpy(char* str);



#endif //EX1_STATE_H
