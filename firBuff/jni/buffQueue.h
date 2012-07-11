/*
 * buffQueue.h
 *
 *  Created on: Jul 11, 2012
 *      Author: Justin
 */

#ifndef BUFFQUEUE_H_
#define BUFFQUEUE_H_

#define SAMPLES 80 //number of Samples to be passed to the filter.
#define MAX_IN_LEN 80
#define MAX_FLT_LEN 63
#define BUFF_LEN (MAX_FLT_LEN  -1 + MAX_IN_LEN)
#define FLT_ORD 8
#define FLT_LEN (FLT_ORD + 1)

struct inBuff {
  double inX[SAMPLES];
  double inY[SAMPLES];
  double inZ[SAMPLES];
};
//filter parameters to be passed on to FIR thread.
struct filtParams {
       struct inBuff data;
       int len;
       int fltLen;
};

struct buffNode {
       int id;
       struct inBuff data;
};
// buffer object
struct datBuffer {
       struct buffNode buffDat;
       struct datBuffer *next;

};

//structs for queue
struct buffQueue {
       struct datBuffer *head;
       struct datBuffer *tail;
};

extern struct datBuffer* pollBuff(struct buffQueue* s);
extern struct buffQueue *addBuff(struct buffQueue* s, struct datBuffer* p);
extern struct buffQueue *removeBuff(struct buffQueue*s);
extern struct buffQueue *newQueue(void);
extern struct buffQueue *freeQueue(struct buffQueue*s);
extern struct buffQueue *removeElems(struct buffQueue*s);
extern void listPrint (const struct buffQueue*s);
extern void listPrintElems(const struct datBuffer*p);

#endif /* BUFFQUEUE_H_ */
