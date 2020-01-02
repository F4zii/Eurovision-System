//
// Created by ethan on 12/14/2019.
//

#include"judge.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<assert.h>

struct judge_t
{
    unsigned int judgeId;
    char* name;
};

//Tested
Judge judgeCreate(unsigned int judgeId, const char* name)
{
    if (name==NULL) return NULL;
    Judge judge = (Judge) malloc(sizeof(*judge));
    if (judge==NULL) return NULL;
    judge->judgeId = judgeId;
    judge->name = (char*) malloc((strlen(name)+1)*sizeof(char));
    if (judge->name==NULL)
    {
        free(judge);
        return NULL;
    }
    strcpy(judge->name,name);
    return judge;
}

//Tested
Judge judgeCopy (Judge j)
{
    Judge cpy = judgeCreate(j->judgeId,j->name);
    return cpy;

}

//Tested
bool compareJudgeToId (Judge judge,int judgeId)
{
    assert(judge!=NULL && judgeId>=0);
    return (judge->judgeId == judgeId);
}

//Tested
int judgeCompare(Judge j1, Judge j2)
{
    if (j1 == NULL || j2 == NULL) return -1;
    if (j1->judgeId == j2->judgeId)
        return 0;
    return 1;
}

unsigned int judgeGetId(Judge j)
{
    return (j->judgeId);
}

//Tested
void judgeDestroy (Judge j)
{
    if (j==NULL)
        return;
    free(j->name);
    free(j);
}

//Tested
void judgePrint (Judge j)
{
    assert(j!=NULL);
    printf("%d %s\n",j->judgeId,j->name);
}
