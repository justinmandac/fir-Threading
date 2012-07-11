/*
 * buffQueue.c
 *
 *  Created on: Jul 11, 2012
 *      Author: Justin
 */
#include <stdio.h>
#include <stdlib.h>
#include "buffQueue.h"

struct datBuffer* pollBuff(struct buffQueue* s)
{
       struct datBuffer *tmp;

       if(s == NULL)
       {
            printf("Queue is not initialized.");
            return NULL;
       }
       else if (s->head == NULL)
       {
            printf("Queue is empty.");
            return NULL;
        }
        tmp = s->head;
       if (s->head == s->tail)
       {
            s->tail = NULL;

       }
       s->head = s->head->next;
       tmp->next = NULL;

       return tmp;

}

struct buffQueue *newQueue(void){
       struct buffQueue* p = malloc(sizeof(p));

       p->head = p->tail = NULL;

       return p;
}

struct buffQueue *addBuff(struct buffQueue *s, struct datBuffer *dat){

        dat->next = NULL;

         if ( s->head == NULL )
        {     s->head = s->tail = dat;
             return s;
        }

             s->tail->next = dat;
             s->tail = dat;


         return s;
}

struct buffQueue *removeElems(struct buffQueue* s)
{
       struct datBuffer* h = NULL;
       struct datBuffer* p = NULL;

       if(s == NULL)
       {

            return s;
            }
            else if (NULL == s->head && NULL == s->tail )
            {

                 return s;
             }
       else if (NULL == s->head || NULL == s->tail )
       {
            return s;
        }

        h = s->head;
        p = h->next;
        free(h);
        s->head = p;
        if( NULL == s->head )  s->tail = s->head;   /* The element tail was pointing to is free(), so we need an update */

        return s;
}
struct buffQueue *freeQueue(struct buffQueue* s)
{
       while(s->head){
          removeElems(s);
                      }
       return s;
}

void listPrint (const struct buffQueue* buff)
{
     struct datBuffer *tmp = NULL;

     if( !(buff->head == NULL && buff->tail == NULL) ){
              for(tmp = buff->head; tmp; tmp = tmp->next){
                      listPrintElems(tmp);
                      }
              }
     else
     {

         }

 }
void listPrintElems(const struct datBuffer* buff){
     int x;
     if(buff){
           for(x = 0; x < SAMPLES; x++){
                 printf("[%d ] X:%lf  Y:%lf  Z:%lf\n",(x+1),buff->buffDat.data.inX[x],buff->buffDat.data.inY[x],buff->buffDat.data.inZ[x] );
                 }
              }
      else{
          printf("Can't print NULL struct\n");
          }
}
