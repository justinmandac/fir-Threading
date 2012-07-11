#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ADARNA-SensorAquisitionMod", __VA_ARGS__))

#define SAMPLES 80
struct inBuff {
  long 	 timestamp;
  double inX[SAMPLES];
  double inY[SAMPLES];
  double inZ[SAMPLES];
};

// buffer object
struct datBuffer {
       struct inBuff buffDat;
       struct datBuffer *next;

};

//structs for queue
struct buffQueue {
       struct datBuffer *head;
       struct datBuffer *tail;
};

 struct datBuffer* pollBuff(struct buffQueue* s);
 struct buffQueue *addBuff(struct buffQueue* s, struct datBuffer* p);
 struct buffQueue *newQueue(void);
 struct buffQueue *freeQueue(struct buffQueue*s);
 struct buffQueue *removeElems(struct buffQueue*s);

struct buffQueue *bufferList; //will hold filtered data
struct buffQueue *fBufferList;
struct datBuffer *dataAcc;

void *filtManager(); //loads data onto the filter
void *streamManager(); //keeps on reading data from files. simulates sensor callbacks.


FILE *fInX;
FILE *fInY;
FILE *fInZ;

int stopFlag;
int buffAddFlag = 0;
pthread_t filtManager_t;
pthread_t strmManager_t;

JNIEXPORT void JNICALL Java_com_fir_firBuff_firBuffTest
  (JNIEnv * env, jclass class) {
	stopFlag = 0;

    struct buffQueue *list;
    struct datBuffer *buff;
    int x;
    int cond = 0;

    bufferList = newQueue();

    if( (fInX = fopen("/sdcard/inputx.txt","r")) != NULL)
    	LOGI("inputx.txt lives!");
    if((fInY = fopen("/sdcard/inputy.txt","r"))!= NULL)
    	LOGI("inputy.txt lives!");
    if((fInZ = fopen("/sdcard/inputz.txt","r"))!= NULL)
    	LOGI("inputz.txt lives!");


    pthread_create(&strmManager_t,NULL,streamManager,NULL);

    freeQueue(bufferList);
    free(bufferList);

}

void *filtManager()
{
	while (buffAddFlag)	LOGI("Thread run...\n");

}
 void *streamManager()
 {
	    char strBuffX[32];
	    char strBuffY[32];
	    char strBuffZ[32];
	    int x;
	    int buffCount = 0;
	    long time=0;
	    LOGI("Inside streamManager.");
	    //initialize string buffers.
	    LOGI("Initializing string buffers...");
	    memset(strBuffX,0,sizeof(strBuffX));
	    memset(strBuffY,0,sizeof(strBuffY));
	    memset(strBuffZ,0,sizeof(strBuffZ));

	    if(feof(fInX))
	    	LOGI("SHIT");

	 while(!feof(fInX) && !feof(fInY) && !feof(fInZ))
	 {
		 dataAcc = malloc(sizeof(struct datBuffer));
		 for(x = 0; x < SAMPLES && (!feof(fInX) && !feof(fInY) && !feof(fInZ)); x++)
		 {	//scan and sleep
			 fscanf(fInX,"%s",strBuffX);
			 fscanf(fInY,"%s",strBuffY);
			 fscanf(fInZ,"%s",strBuffZ);
			 dataAcc->buffDat.timestamp = time;
			 dataAcc->buffDat.inX[x] = atof(strBuffX);
			 dataAcc->buffDat.inY[x] = atof(strBuffY);
			 dataAcc->buffDat.inZ[x] = atof(strBuffZ);

			 usleep(1000);
			 	time+= 1000; //simulated timestamp
			    memset(strBuffX,0,sizeof(strBuffX));
			    memset(strBuffY,0,sizeof(strBuffY));
			    memset(strBuffZ,0,sizeof(strBuffZ));
		 }

		 if (x < SAMPLES) //ran out of samples to read
		 {
			 //pad the remaining array entries with (SAMPLE - 1 - x) zeros.
			 for(; x < SAMPLES; x++)
			 {
				 dataAcc->buffDat.inX[x] = 0.0;
				 dataAcc->buffDat.inY[x] = 0.0;
				 dataAcc->buffDat.inZ[x] = 0.0;
			 }
		 }

		 LOGI("Buffer #%d",++buffCount);
		 //push into Queue
		 addBuff(bufferList, dataAcc);
		 buffAddFlag = 1;
	 }
	 LOGI("Closing files...");
	 fclose(fInX);
	 fclose(fInY);
	 fclose(fInZ);
 }


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
