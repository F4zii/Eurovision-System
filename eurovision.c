
#include"eurovision.h"
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<assert.h>
#include"vote.h"
#include"state.h"
#include"judge.h"


struct eurovision_t
{
    Set votes;
    Set states;
    Set judges;
};

/**
 * Eurovision Creation and Destruction system
 * Manages the creation and destruction of Eurovision systems,
 * The addition of states and judges (and their creation and destruction),
 * and the recording of votes (but not processing/tallying those votes).
 * Low-level instructions based primarily on ADT functions for judge, state and vote.
 * */

//Probably works
Eurovision eurovisionCreate()
{
    Eurovision ev = (Eurovision)malloc(sizeof(*ev));
    if (ev==NULL)
    {
        return NULL;
    }
    ev->votes = setCreate((copySetElements) voteCopy, (freeSetElements) voteDestroy, (compareSetElements) voteCompare);
    if (ev->votes == NULL)
    {
        free(ev);
        return NULL;
    }
    ev->states = setCreate((copySetElements) stateCopy, (freeSetElements) stateDestroy,
                           (compareSetElements) stateCompare);
    if (ev->states == NULL)
    {
        setDestroy(ev->votes);
        free(ev);
        return NULL;
    }
    ev->judges = setCreate((copySetElements) judgeCopy, (freeSetElements) judgeDestroy,
                           (compareSetElements) judgeCompare);
    if (ev->judges == NULL)
    {
        setDestroy(ev->states);
        setDestroy(ev->votes);
        free(ev);
        return NULL;
    }
    return ev;
}

void eurovisionDestroy(Eurovision eurovision)
{
    if (eurovision==NULL)
        return;
    setDestroy(eurovision->votes);
    setDestroy(eurovision->states);
    setDestroy(eurovision->judges);
    free(eurovision);
}

/**A function which returns true if the string given to it is a valid name
 * for a state/judge and false otherwise*/
//Tested
static bool validName (const char* name)
{
    assert(name!=NULL);
    for(int i=0;name[i]!='\0';i++)
    {
        if ((name[i]>='a' && name[i]<='z')||name[i]==' ')
            continue;
        return false;
    }
    return true;
}

//Tested
EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
                                    const char *stateName,
                                    const char *songName)
{
    if (stateName==NULL || songName == NULL || eurovision==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateId<0)
        return EUROVISION_INVALID_ID;
    if (!validName(stateName) || !validName(songName))
    return EUROVISION_INVALID_NAME;
    State s = stateCreate(stateId,stateName,songName);
    if (s==NULL)
    {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    if (setAdd(eurovision->states,s)==SET_ITEM_ALREADY_EXISTS)
    {
        stateDestroy(s);
        return EUROVISION_STATE_ALREADY_EXIST;
    }
    stateDestroy(s);
    return EUROVISION_SUCCESS;
}

/**Deletes a subset of votes depending on the srcIds, trgIds, and VoterTypes
 * srcId, trgId can be ALL, whose behavior is described in vote.h*/

static void clearSubsetOfVotes(Eurovision ev,int srcId, int trgId, VoterType srctype)
{
    assert(ev!=NULL && (srcId>=0 || srcId==ALL) && (trgId>=0 || trgId==ALL));
    SET_FOREACH(Vote,i,ev->votes)
    {
        if (compareVoteToIds(i,srcId,trgId,srctype))
        {
            setRemove(ev->votes,i);
        }
    }
}

EurovisionResult eurovisionRemoveJudge (Eurovision ev, int judgeId)
{
    if (ev==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (judgeId<0)
        return EUROVISION_INVALID_ID;
    SET_FOREACH(Judge,iter,ev->judges)
    {
        if (compareJudgeToId(iter,judgeId))
        {
            clearSubsetOfVotes(ev,judgeId,ALL,JUDGE);
            setRemove(ev->judges,iter);
            return EUROVISION_SUCCESS;
        }
    }
    return EUROVISION_JUDGE_NOT_EXIST;
}

/**This function returns a pointer to a state if the state is in the eurovision system.
 * If no state is found @param is NULL*/

static State findStateById (Eurovision ev, unsigned int stateId)
{
    assert(ev != NULL);
    SET_FOREACH(State, iter, ev->states)
    {
        if (stateCompareToId(iter, stateId) == true)
            return iter;
    }
    return NULL;
}

/**This function returns a pointer to a vote by its ids from the
 * eurovision system ev. If no votes are found @return is null*/

static Vote findVoteByIds (Eurovision ev,unsigned int srcId, unsigned int trgId, VoterType srctype)
{
    SET_FOREACH(Vote,i,ev->votes)
    {
        if (compareVoteToIds(i,srcId,trgId,srctype))
            return i;
    }
    return NULL;
}



EurovisionResult eurovisionRemoveState (Eurovision eurovision, int stateId)
{
    if(eurovision==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateId<0)
        return EUROVISION_INVALID_ID;
    State s = findStateById(eurovision,stateId);
    if (s==NULL)
        return EUROVISION_STATE_NOT_EXIST;
    setRemove(eurovision->states,s);
    Set copyOfVotes = setCopy(eurovision->votes);
    clearSubsetOfVotes(eurovision,stateId,ALL,STATE); //Deletes votes from state
    clearSubsetOfVotes(eurovision,ALL,stateId,STATE); // Deletes votes to state from other states
    SET_FOREACH(Vote,iter,copyOfVotes) //Deletes judges which voted from the state
    {
        if (compareVoteToIds(iter,ALL,stateId,JUDGE))
        {
            eurovisionRemoveJudge(eurovision,voteGetSource(iter));
        }
    }
    setDestroy(copyOfVotes);
    return EUROVISION_SUCCESS;
}

/**This function receives an integer stateRank (the states place - 1)
 * and returns the score given to it, in points, using the const array that
 * correlates rank to score*/

const int scores[10] = {12,10,8,7,6,5,4,3,2,1};


static int getScore(int stateRank)
{
    if (stateRank<0)
        return 0;
    if (stateRank<10)
        return scores[stateRank];
    return 0;
}

EurovisionResult eurovisionAddJudge (Eurovision eurovision, int judgeId,
        const char* judgeName, int* judgeResults)
{
    if (judgeName==NULL || judgeResults==NULL || eurovision==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (judgeId<0)
        return EUROVISION_INVALID_ID;
    if (!validName(judgeName))
        return EUROVISION_INVALID_NAME;
    Judge new_judge = judgeCreate(judgeId,judgeName);
    if (new_judge==NULL)
    {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    if(setIsIn(eurovision->judges,new_judge))
    {
        judgeDestroy(new_judge);
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    setAdd(eurovision->judges,new_judge);
    for (int i=0;i<10;i++)
    {
        if (judgeResults[i]<0)
        {
            setRemove(eurovision->judges,new_judge);
            judgeDestroy(new_judge);
            return EUROVISION_INVALID_ID;
        }
        if (findStateById(eurovision, judgeResults[i])==NULL)
        {
            setRemove(eurovision->judges,new_judge);
            judgeDestroy(new_judge);
            return EUROVISION_STATE_NOT_EXIST;
        }
        Vote v = voteCreate(judgeId,judgeResults[i],JUDGE);
        voteAppend(v,getScore(i));
        setAdd(eurovision->votes,v);
        voteDestroy(v);
    }
    judgeDestroy(new_judge);
    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker)
{
    if (eurovision==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateGiver<0 || stateTaker<0)
        return EUROVISION_INVALID_ID;
    if (findStateById(eurovision,stateGiver)==NULL || findStateById(eurovision,stateTaker)==NULL)
        return EUROVISION_STATE_NOT_EXIST;
    if (stateGiver == stateTaker)
        return EUROVISION_SAME_STATE;
    Vote v = findVoteByIds(eurovision,stateGiver,stateTaker,STATE); //Attempts to find vote in votes set
    if (v!=NULL) //Checks if a vote type between the state exists
    {
        voteAppend(v,1);
        return EUROVISION_SUCCESS;
    }
    v = voteCreate(stateGiver,stateTaker,STATE); //Creates a vote between the two states
    voteAppend(v,1);
    setAdd(eurovision->votes,v);
    voteDestroy(v);
    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveVote (Eurovision eurovision, int stateGiver, int stateTaker)
{
    if (eurovision==NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateGiver<0 || stateTaker<0)
        return EUROVISION_INVALID_ID;
    if (stateGiver==stateTaker)
        return EUROVISION_SAME_STATE;
    Vote vote = findVoteByIds(eurovision,stateGiver,stateTaker,STATE);
    if (vote==NULL)
        return EUROVISION_STATE_NOT_EXIST;
    voteDecrement(vote);
    return EUROVISION_SUCCESS;
}



/**
 * Eurovision vote management & scoring system
 * Manages the tallying and counting of votes from states/judges
 * and the recording of total scores into the participating states
 * Intermediate-level instructions for contest-running system
 * */




/**
 * This function returns 1 if the vote v2 has a higher number of votes than v1,
 * or if the votes are equal if the v2's source has a lexicographically smaller name than v1's
 * and 0 otherwise
 **/
static int isVoteHigherThan(Eurovision ev, Vote v1, Vote v2)
{
    if (voteGetNum(v2)>voteGetNum(v1))
        return 1;
    if (voteGetNum(v2)==voteGetNum(v1))
    {
        State s1 = findStateById(ev,voteGetSource(v1));
        State s2 = findStateById(ev,voteGetSource(v2));
        if (strcmp( stateGetName(s1) , stateGetName(s2))<0)
            return 1;
    }
    return 0;
}

/**
 * A function which receives two states, srcId (the state giving votes) and trgId (the state whose
 * score from srcId will be returned) and a eurovision system as parameters, and returns the score
 * given to trgId from the audience in srcId, in points.
 **/
//Tested
static unsigned int getScoreFromState(Eurovision ev, unsigned int srcId,
        unsigned int trgId)
{
    Vote v = findVoteByIds(ev,srcId,trgId,STATE);
    int cnt=0;
    SET_FOREACH(Vote,iter,ev->votes)
    {
        if (voteCompare(v,iter)==0)
            continue;
        if (compareVoteToIds(iter,srcId,ALL,STATE))
            cnt+=isVoteHigherThan(ev,v,iter);
    }
    return getScore(cnt);
}

/**
 * This function calculates the score a state receives from other states, by going over
 * each state in the eurovision and finding out how much the given state received
 * from each one of them, using the getScoreFromState function
 * NOTICE: This function calculates the whole number of points given to the state,
 * other calculations of averages and percentages are done at a higher level.
 **/
static void calculateTotalScoreFromStates(Eurovision ev, unsigned int stateId)
{
    unsigned int score = 0;
    Set votesCopy = setCopy(ev->votes);
    SET_FOREACH(Vote,iter,votesCopy)
    {
        if (compareVoteToIds(iter,ALL,stateId,STATE))
        {
            score+=getScoreFromState(ev,voteGetSource(iter),stateId);
        }
    }
    State s = findStateById(ev,stateId);
    if (s==NULL)
        return;
    setStatesScore(s,score);
    setDestroy(votesCopy);
}

/**A function which calculates a states total score from the judges and
 * places it into the judgesScore field of a state.
 * NOTICE: This score is a whole number, for this function. The conversion
 * to averages and percentages are done at a higher level
 *
 * This function was written slightly differently from the calculateTotalScoreFromStates
 * function, because the judges votes are automatically stored with their given numerical
 * scores, to reduce complexity and inefficiency*/
static void calculateTotalScoreFromJudges(Eurovision ev, unsigned int stateId)
{
    unsigned int score = 0;
    SET_FOREACH(Judge,j,ev->judges)
    {
        Vote v = findVoteByIds(ev,judgeGetId(j),stateId,JUDGE);
        if (v!=NULL)
        {
            score+=voteGetNum(v);
        }
    }
    State s = findStateById(ev,stateId);
    if(s==NULL)
        return;
    setJudgesScore(s,score);
}

/**A function which calculates the scores of each state from the other states and the judges
 * (The first set_foreach loop) and then sets the score to averages and depending on the audience percent
 * (The second set_foreach loop)*/

static void calculateAllScores (Eurovision ev, int audiencePercent)
{
    assert(ev!=NULL && audiencePercent>=0 && audiencePercent<=100);
    Set statesCopy = setCopy(ev->states);
    SET_FOREACH(State,state,statesCopy)
    {
        calculateTotalScoreFromStates(ev,stateGetId(state));
        calculateTotalScoreFromJudges(ev,stateGetId(state));
    }
    SET_FOREACH(State,s,ev->states)
    {
        setScoreToAverages(s,setGetSize(ev->states),setGetSize(ev->judges));
        setTotalScore(s,audiencePercent);
        //statePrint(s);
    }
    setDestroy(statesCopy);
}

/**
 * Eurovision contest-running system
 * High-level interface for running different eurovision contests
 * on already-defined eurovisions
 *
 * */

List eurovisionRunContest(Eurovision eurovision, int audiencePercent)
{
    if (eurovision==NULL || audiencePercent<0 || audiencePercent>100)
        return NULL;
    calculateAllScores(eurovision,audiencePercent);
    List contestList = listCreate((CopyListElement) stateCopy, (FreeListElement) stateDestroy);
    SET_FOREACH(State,s,eurovision->states)
    {
        //statePrint(s);
        listInsertFirst(contestList,s);
    }
    listSort(contestList, (CompareListElements) stateScoresCompare);
    List contestNames = listCreate((CopyListElement) namecpy,
            (FreeListElement) namefree);
    LIST_FOREACH(State,iter,contestList)
    {
        //printf("%s\n",stateGetName(iter));
        listInsertLast(contestNames,stateGetName(iter));
    }
    listDestroy(contestList);
    return contestNames;
}

/*
 * Runs a eurovision contest with 100% audience percent rates (not efficient but reduces
 * code duplication)
 */

List eurovisionRunAudienceFavorite(Eurovision eurovision)
{
    return eurovisionRunContest(eurovision,100);
}

/*
 * A function which attempts to find a friendly state to the state
 * whose Id is the parameter. If a state is found, a pointer to it is returned
 * if not, NULL is returned.
 */
/* Alternative code for the inside of the set_foreach loop, probably causes mem leak
 * Vote vc = voteCopy(v); //eliminates memory leak, enables votesCopy to be freed
                setDestroy(votesCopy);
                return findStateById(ev,voteGetTarget(vc));
*/


static State findFriendlyState(Eurovision ev, unsigned int stateId)
{
    Set votesCopy = setCopy(ev->votes);
    SET_FOREACH(Vote,v,votesCopy)
    {
        if(compareVoteToIds(v,stateId,ALL,STATE))
        {
            int srcToTrgScore = getScoreFromState(ev,stateId,voteGetTarget(v));
            int trgToSrcScore = getScoreFromState(ev,voteGetTarget(v),stateId);
            if (srcToTrgScore == 12 && trgToSrcScore == 12)
            {
                unsigned int trg = voteGetTarget(v); //eliminates memory leak, enables votesCopy to be freed
                setDestroy(votesCopy);
                return findStateById(ev,trg);
            }
        }
    }
    setDestroy(votesCopy);
    return NULL;
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision)
{
    if (eurovision==NULL)
        return NULL;
    List friendlyStates = listCreate((CopyListElement)namecpy,
                                     (FreeListElement)namefree);
    SET_FOREACH(State,s,eurovision->states)
    {
        State fr = findFriendlyState(eurovision, stateGetId(s));
        if (fr!=NULL)
        {
            char* string = getFriendlyStatesString(s,fr);
            listInsertFirst(friendlyStates,string);
            free(string);
        }
    }
    listSort(friendlyStates,(CompareListElements) strcmp);
    return friendlyStates;
}

















