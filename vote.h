//
// Created by ethan on 12/14/2019.
//

#ifndef EX1_VOTE_H
#define EX1_VOTE_H

#include<stdbool.h>
#define ALL -1

/**
 * VOTE MODULE TESTED SUCCESSFULLY
 * 26/12/19
 **/



typedef struct vote_t* Vote;

/**An enumerated type for recording if a vote comes from a state or a judge*/
typedef enum
{
    STATE,
    JUDGE
}VoterType;

typedef enum
{
    VOTE_SUCCESS,
    VOTE_NULL_ARG,
    VOTE_EMPTY
}VoteResult;

/** A function for creating a vote with a source (state/judge) and target and with 0 votes between them
 * Returns NULL if both ids are the same or if the memory allocation failed*/
Vote voteCreate (unsigned int srcId, unsigned int trgId, VoterType srct);

/**A function for removing one vote from a vote struct*/
VoteResult voteDecrement (Vote v);

/**A function which returns the srcId of the vote*/
unsigned int voteGetSource(Vote v);

/**A function which returns the numOfVotes of the vote*/
unsigned int voteGetNum(Vote v);

/**A function which returns the trgId of the vote*/
unsigned int voteGetTarget(Vote v);


/**A function for adding votes to a vote struct*/
VoteResult voteAppend (Vote v, unsigned int appendNum);

/**A function which returns 0 if v1=v2, and 1 otherwise*/
int voteCompare (Vote v1, Vote v2);

/**A function which checks if a vote's ids are equal to the parameters csrc (checked source Id)
 * and ctrg (checked target Id) and if the vote comes from the same VoterType.
 * If ALL is entered into @param csrc or ctrg, the function returns all votes
 * which fulfil the other criteria. For example, voteEqualsIds(v,ALL,trg,STATE) will return
 * true if v is a vote from a state to trg*/
bool compareVoteToIds (Vote v,int src_check,int trg_check, VoterType src_type);

/**A function for copying a vote*/
Vote voteCopy (Vote v);

/**A function for destroying a vote struct*/
void voteDestroy (Vote v);

/**Prints a vote*/
void votePrint (Vote v);



#endif //EX1_VOTE_H
