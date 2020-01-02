//
// Created by iddos on 12/13/2019.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "state.h"



struct state_t
{
    unsigned int stateId;
    double statesScore;
    double judgesScore;
    char* name;
    char* songName;
};

//Tested
State stateCreate(unsigned int stateId, const char* stateName, const char* songName)
{
    if (stateName == NULL || songName == NULL)
    {
        return NULL;
    }

    State state = malloc(sizeof(struct state_t));
    if(state==NULL)
    {
        return NULL;
    }
    state->stateId = stateId;
    state->name = (char*) malloc((strlen(stateName)+1)*sizeof(char));
    if (state->name == NULL)
    {
        free(state);
        return NULL;
    }
    state->songName = (char*) malloc((strlen(songName)+1)*sizeof(char*));
    if(state->songName==NULL)
    {
        free(state->name);
        free(state);
        return NULL;
    }
    strcpy(state->name,stateName);
    strcpy(state->songName,songName);
    state->statesScore = 0;
    state->judgesScore = 0;
    return state;
}

//Tested
bool stateCompareToId(State s, unsigned int id)
{
    assert(s!=NULL);
    return (s->stateId == id);
}

//Tested, for list
int stateScoresCompare (State s1, State s2)
{
    assert(s1!=NULL && s2!=NULL);
    if (s1->statesScore > s2->statesScore)
        return -1;
    else if (s1->statesScore == s2->statesScore)
        return 0;
    else
        return 1;
}

//Tested, for set
int stateCompare(State s1, State s2)
{
    return (s1->stateId - s2->stateId);
}

//Tested
State stateCopy(State s)
{
    if (s==NULL)
        return NULL;
    State cpy = stateCreate(s->stateId,s->name, s->songName);
    cpy->statesScore = s->statesScore;
    cpy->judgesScore = s->judgesScore;
    return cpy;
}

//Tested
char* stateGetName(State s)
{
    return s->name;
}

//Tested
unsigned int stateGetId(State s)
{
    return s->stateId;
}

//Tested
void statePrint (State s)
{
    assert(s!=NULL);
    printf("\nState Id: %d \nState Name: %s \nSong Name: %s \nScores: %f %f\n"
            ,s->stateId,s->name,s->songName,s->statesScore,s->judgesScore);
}

//Tested
void setStatesScore (State s, double score)
{
    assert(s!=NULL && score>=0);
    s->statesScore = score;
}

//Tested
void setJudgesScore(State s, double score)
{
    assert(s != NULL && score >= 0);
    s->judgesScore = score;
}

//Tested
void setTotalScore (State s, int audiencePercent)
{
    assert(s!=NULL && audiencePercent>=0 && audiencePercent<=100);
    double savg = s->statesScore, javg = s->judgesScore, ap = audiencePercent/100.0;
    s->statesScore = ((savg*ap) + (javg*(1-ap)));
}

//Tested
void setScoreToAverages (State s, int numOfStates, int numOfJudges)
{
    s->statesScore/=numOfStates;
    s->judgesScore/=numOfJudges;
}

void stateDestroy(State state)
{
    if (state == NULL)
        return;
    free(state->name);
    free(state->songName);
    free(state);
}

//Tested
static void appendFriendlyStatesByFormat (char* str, const char* s1, const char* s2)
{
    strcpy(str,s1);
    strcat(str," - ");
    strcat(str,s2);
}

//Tested
char* getFriendlyStatesString(State s1, State s2)
{
    char* str= (char*) malloc((strlen(stateGetName(s1)) +
            strlen(" - ") + strlen(stateGetName(s2))+1)*sizeof(char));
    if (strcmp(s1->name,s2->name)<0)
    {
        appendFriendlyStatesByFormat(str,s1->name,s2->name);
    }
    else
    {
        appendFriendlyStatesByFormat(str,s2->name,s1->name);
    }
    return str;
}

void namefree(char* str)
{
    free(str);
}

char* namecpy (char* str)
{
    char* strc = (char*)malloc((strlen(str)+1)*sizeof(char));
    if (strc == NULL) return NULL;
    strcpy(strc, str);
    return strc;
}

/**
//Tested
char* friendlyStatesPairCopy(char* str)
{
    char* nstr = malloc(sizeof(char*));
    strcpy(nstr,str);
    return nstr;
}

//Tested
void friendlyStatesPairDestroy(char* str)
{
    free(str);
}*/




