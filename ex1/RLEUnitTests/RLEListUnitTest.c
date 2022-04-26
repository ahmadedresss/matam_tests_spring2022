#include <stdio.h>
#include "mm_malloc.h"

#define ROW_DOWN '\n'

typedef char (*MapFunction)(char);

typedef enum {
    RLE_LIST_SUCCESS,
    RLE_LIST_OUT_OF_MEMORY,
    RLE_LIST_NULL_ARGUMENT,
    RLE_LIST_INDEX_OUT_OF_BOUNDS,
    RLE_LIST_ERROR
} RLEListResult;

typedef struct RLEList_t {
    char letter;
    int appearances;
    struct RLEList_t* next_letter;
} *RLEList;

RLEList RLEListCreate()
{

    RLEList ptr=malloc(sizeof(*ptr));
    if(ptr==NULL)
        return 0;
    ptr->next_letter=NULL;
    ptr->appearances=1;
    return ptr;
}

RLEListResult RLEListAppend (RLEList list,char value)
{
    if(list==NULL)
        return RLE_LIST_NULL_ARGUMENT;

    RLEList temp= list;


    while(temp->next_letter!=NULL)
    {
        temp=temp->next_letter;
    }


    if(temp->letter==value)
    {
        temp->appearances++;
        return RLE_LIST_SUCCESS;
    }
    else
    {
        RLEList newHead = RLEListCreate();
        if (newHead==NULL)
        {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        newHead->letter = value;
        temp->next_letter = newHead;

        return RLE_LIST_SUCCESS;
    }
}

void RLEListDestroy(RLEList list)
{
    while (list->next_letter!=NULL)
    {
        RLEList toDelete=list;
        list=list->next_letter;
        free(toDelete);
    }
}

int RLEListSize(RLEList list)
{
    if(list==NULL)
        return -1;

    RLEList tmp=list;
    int sum=0;
    while(tmp!=NULL)
    {
        sum+=tmp->appearances;
        tmp=tmp->next_letter;
    }
    return sum-1;
}

RLEListResult RLEListRemove(RLEList list, int index)
{
    int sum=0;

    if(list==NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    RLEList prev=list;
    RLEList fwr=list;
    if(prev->letter==0)
    {
        prev=prev->next_letter;
    }
    if(fwr->letter==0)
    {
        fwr=fwr->next_letter;
    }
    if(index> RLEListSize(list)||index<0)
    {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    else
    {
        while(sum<index)
        {
            sum+=fwr->appearances;
            fwr=fwr->next_letter;
        }
        fwr->appearances--;
        if(fwr->appearances==0)
        {
            //now we have three options
            if (fwr->next_letter==NULL)
            {
                while (prev->next_letter!=fwr)
                {
                    prev=prev->next_letter;
                }
                prev->next_letter=NULL;
                free(fwr);
            }
            else if ( fwr==list)
            {
                list=list->next_letter;
                free(prev);
                free(fwr);
            }
            else if (fwr->next_letter!=NULL && fwr!=list )
            {
                while (prev->next_letter != fwr)
                {
                    prev=prev->next_letter;
                }
                prev->next_letter=fwr->next_letter;
                free(fwr);
            }
        }
    }
    return  RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index,RLEListResult *result)
{

    if(list== NULL)
    {
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if(result == NULL)
    {
        return 0;
    }
    if(index > RLEListSize(list)||index<0)
    {
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    RLEList temp=list;
    if(temp->letter==0)
    {
        temp=list->next_letter;
    }

    int sum=0;
    while(temp->next_letter!=NULL)
    {
        if (index <= sum)
        {
            *result=RLE_LIST_SUCCESS;
            return temp->letter;
        }
        else
        {
            sum+=temp->appearances;
            //temp=temp->next_letter;
        }
    }
    return 0;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
    if(list==NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList tmp=list;
    while(tmp!=NULL)
    {
        tmp->letter=map_function(tmp->letter);
        tmp=tmp->next_letter;
    }
    return RLE_LIST_SUCCESS;
}

char* RLEListExportToString(RLEList list, RLEListResult* result)
{
    if(list==NULL)
        *result=RLE_LIST_NULL_ARGUMENT;
    char *arr= malloc(sizeof(char)*(3* RLEListSize(list)));
    RLEList tmp=list;
    int index=0;
    while(tmp!=NULL&& index!=RLEListSize(list) &&  index+1!=RLEListSize(list) && index+2!=RLEListSize(list) )
    {
        arr[index] = tmp->letter;
        arr[index + 1] =(char) tmp->appearances;
        arr[index + 2] = ROW_DOWN;
        index++;
        tmp=tmp->next_letter;
    }
    *result=RLE_LIST_SUCCESS;
    return arr ;
}
