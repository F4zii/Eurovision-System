//
// Created by ethan on 12/14/2019.
//
#include"vote.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include<assert.h>

struct vote_t
{
    unsigned int sourceId;
    VoterType sourceType;
    unsigned int targetId;
    unsigned int numOfVotes;
};

//Tested
Vote voteCreate(unsigned int srcId, unsigned int trgId, VoterType srct)
{
    if(srcId==trgId && srct==STATE)
        return NULL;
    Vote v = (Vote)malloc(sizeof(*v));
    if (v==NULL)
        return NULL;
    v->sourceId = srcId;
    v->sourceType = srct;
    v->targetId = trgId;
    v->numOfVotes = 0;
    return v;
}

//Tested
VoteResult voteAppend (Vote v, unsigned int appendNum)
{
    if (v==NULL)
        return VOTE_NULL_ARG;
    if (v->numOfVotes+appendNum>UINT_MAX)
    {
        v->numOfVotes = UINT_MAX;
        return VOTE_SUCCESS;
    }
    v->numOfVotes+=appendNum;
    return VOTE_SUCCESS;

}

//Tested
unsigned int voteGetSource(Vote v)
{
    return v->sourceId;
}

//Tested
unsigned int voteGetTarget(Vote v)
{
    return v->targetId;
}

//Tested
unsigned int voteGetNum(Vote v)
{
    return v->numOfVotes;
}

//Tested
VoteResult voteDecrement (Vote v)
{
    if (v==NULL)
        return VOTE_NULL_ARG;
    if (v->numOfVotes==0)
        return VOTE_EMPTY;
    v->numOfVotes--;
    return VOTE_SUCCESS;
}

//Tested
bool compareVoteToIds (Vote v,int src_check,int trg_check, VoterType src_type)
{
    bool check_source = ((v->sourceId==src_check) || (src_check==ALL));
    bool check_target = ((v->targetId==trg_check) || (trg_check==ALL));
    bool check_type = (v->sourceType == src_type);
    return (check_source && check_target && check_type);
}

//Tested
int voteCompare (Vote v1, Vote v2)
{
    if(compareVoteToIds(v1,v2->sourceId,v2->targetId,v2->sourceType))
        return 0;
    return 1;
}

//Tested
Vote voteCopy (Vote v)
{
    Vote cpy = voteCreate(v->sourceId,v->targetId,v->sourceType);
    if (cpy==NULL)
        return NULL;
    cpy->numOfVotes = v->numOfVotes;
    return cpy;
}

//Tested
void voteDestroy(Vote v)
{
    free(v);
}

//Tested
void votePrint(Vote v)
{
    assert(v!=NULL);
    printf("Vote Source: %d Vote Target: %d Number of Votes: %d\n",v->sourceId,v->targetId,v->numOfVotes);
}







