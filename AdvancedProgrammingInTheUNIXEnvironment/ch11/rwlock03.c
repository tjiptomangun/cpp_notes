#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../apue.h"
#define INSUF_BALANCE -1
#define NOT_ALLOWED -2
#define SUCCESS 0

typedef struct acc{
	struct acc* acc_next;
	struct acc* acc_prev;
	unsigned long acc_id; /** tells which thread handles this strAccount*/ 
	/*... more stuff here ...*/
	long int balance;
	pthread_rwlock_t q_lock;
	
} strAccount, *pstrAccount;

pstrAccount newAccount(unsigned long acc_id, long int balance) {
	pstrAccount pAcc = (pstrAccount) calloc(1, sizeof(strAccount));
	if(pAcc) {
		pAcc->acc_id = acc_id;
		pAcc->balance = balance; 
		int err = pthread_rwlock_init(&pAcc->q_lock, NULL);
		if (err != 0) {
			err_quit("can't init rwlock : %s", strerror(err));
		}
	}
	return pAcc;
}

int transfer(strAccount *accToDebet, strAccount *accToCredit, long int amount) {
	pthread_rwlock_t *firstLock;
	pthread_rwlock_t *secondLock;
	if (accToDebet->acc_id == accToCredit->acc_id){
		return NOT_ALLOWED;
	}
	else if (accToDebet->acc_id > accToCredit->acc_id) {
		firstLock = &accToCredit->q_lock;
		secondLock = &accToDebet->q_lock;
	}
	else {
		firstLock = &accToDebet->q_lock;
		secondLock = &accToCredit->q_lock;
	}
	pthread_rwlock_wrlock(firstLock);
	pthread_rwlock_wrlock(secondLock);
	if (accToDebet->balance >= amount) {
		accToDebet->balance -= amount;
		accToCredit->balance += amount;
	}
	else {
		pthread_rwlock_unlock(secondLock);
		pthread_rwlock_unlock(firstLock);
		return INSUF_BALANCE;
	}
	pthread_rwlock_unlock(secondLock);
	pthread_rwlock_unlock(firstLock);
	return SUCCESS;
}

long int inquiryBalance(strAccount *accToCheck) {		
	if (accToCheck) {
		long int val = 0;
		pthread_rwlock_rdlock(&accToCheck->q_lock);
		val = accToCheck->balance;
		pthread_rwlock_unlock(&accToCheck->q_lock);
		return val;
	}
	else {
		return -1;
	}
}

typedef struct queue {
	strAccount *q_head;
	strAccount *q_tail;
	pthread_rwlock_t q_lock;
} strQueue, *pstrQueue;

/**
 * Initialize a queue
 */

int queue_init(struct queue *qp) {
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;
	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0) {
		err_quit("can't init rwlock : %s", strerror(err));
		return 1;
	}
	else {
		//continue initialization
		return 0;
	}
}

/** 
 * Insert a strAccount at the head of the queue
 */
void account_insert(struct queue *qp, strAccount *jp) {
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->acc_next = qp->q_head;
	jp->acc_prev = NULL;
	if (qp->q_head != NULL){
		qp->q_head->acc_prev = jp;
	}
	else {
		qp->q_tail = jp;/** list was empty */
	}
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

void account_append(struct queue *qp, strAccount *jp) {	
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->acc_next = NULL;
	jp->acc_prev = qp->q_tail;
	if (qp->q_tail != NULL){
		qp->q_tail->acc_next = jp;
	}
	else {
		qp->q_head = jp;/** list was empty */
	}
	qp->q_tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/**
 * Remove the given strAccount from a queue 
 */
void account_remove(struct queue *qp, strAccount *jp) {
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head) {
		qp->q_head = jp->acc_next;
		if(qp->q_tail == jp) {
			//or if jp->acc_next = NULL
			//qp->q->tail = jp->acc_next
			qp->q_tail = NULL;
		}
		else {
			jp->acc_next->acc_prev = jp->acc_prev;
		}
	}
	else if (jp == qp->q_tail) {
		qp->q_tail = jp->acc_prev;
		//qp->q_tail->acc_next = NULL;
		jp->acc_prev->acc_next = jp->acc_next; //which is null
	}
	else {
		jp->acc_prev->acc_next = jp->acc_next;
		jp->acc_next->acc_prev = jp->acc_prev;
	}
	pthread_rwlock_unlock(&qp->q_lock);
}

/** 
 * Find a strAccount for the given thread ID
 */

strAccount* account_find(struct queue *qp, unsigned int acc_id) {
	strAccount* jp = NULL;
	if (pthread_rwlock_rdlock(&qp->q_lock) != 0) {
		return NULL;
	}
	for(jp = qp->q_head; jp != NULL; jp = jp->acc_next) {
		if (jp->acc_id == acc_id) {
			break;
		}
	}
	pthread_rwlock_unlock(&qp->q_lock);
	return jp;
} 
//use bank transfer a to b as an example
typedef struct transferViaQueue {	
	pstrQueue pqueue;
	unsigned long acc_from;
	unsigned long acc_to;
	long int amount; 
} strTransfer, *pstrTransfer;

void *transfer_thread(void *tmp) {
	pstrTransfer transDetail = (pstrTransfer) tmp;
	pstrQueue pqueue = transDetail->pqueue;
	unsigned long acc_from = transDetail->acc_from;
	unsigned long acc_to= transDetail->acc_to;
	long int amount = transDetail->amount;
	for (int i = 0; i < 100000; i ++) {
		pstrAccount accFrom = account_find(pqueue, acc_from);
		if(accFrom) { 
			pstrAccount accTo = account_find(pqueue, acc_to);
			if (accTo) {
				transfer(accFrom, accTo, amount);
			}
			else {
				fprintf(stdout, "Failed to find destination account %lu\n", acc_to);
			}
		}
		else {
			fprintf(stdout, "Failed to find source account %lu\n", acc_from);
		} 
	} 
	return NULL;
}

typedef struct {	
	pstrQueue pqueue;
	unsigned long acc_id;
} strInquiryBalance, *pstrInquiryBalance;

void *inquiry_balance_thread(void *tmp) {
		
	pstrInquiryBalance pDetail= (pstrInquiryBalance) tmp;
	unsigned long acc_id = pDetail->acc_id;
	pstrQueue pqueue = pDetail->pqueue;
	for(unsigned long i = 0; i< 100; i++) { 
		pstrAccount accFrom = account_find(pqueue, acc_id);
		if (accFrom) {
			long int balance = inquiryBalance(accFrom);
			fprintf(stdout, "%ld balance %lu\n", acc_id, balance);
		}
		else {
			fprintf(stdout, "account not found %ld\n", acc_id);
		}
		usleep(15000);
		
		
	}
	return NULL;	
}

int main(int argc, char **argv) {
	pstrQueue pqueue = (pstrQueue)calloc(1, sizeof(strQueue));
	int err = 0;
	void *tret;
	pthread_t tidt1, tidt2, tidib1, tidib2;
	queue_init(pqueue);
	pstrAccount acc1 = newAccount(1,  10000);
	pstrAccount acc2 = newAccount(2,  10000); 
	account_insert(pqueue, acc1);
	account_insert(pqueue, acc2);
	pstrTransfer t1 = (pstrTransfer) calloc(1, sizeof(strTransfer));
	t1->pqueue = pqueue;
	t1->acc_from = acc1->acc_id;
	t1->acc_to = acc2->acc_id;
	t1->amount = 1;
	
	pstrTransfer t2 = (pstrTransfer) calloc(1, sizeof(strTransfer));
	t2->pqueue = pqueue;
	t2->acc_from = acc2->acc_id;
	t2->acc_to = acc1->acc_id;
	t2->amount = 1;

	pstrInquiryBalance ib1 = (pstrInquiryBalance) calloc(1, sizeof(strInquiryBalance));
	ib1->pqueue = pqueue;
	ib1->acc_id = acc1->acc_id;

	pstrInquiryBalance ib2 = (pstrInquiryBalance) calloc(1, sizeof(strInquiryBalance));
	ib2->pqueue = pqueue;
	ib2->acc_id = acc2->acc_id;
	
	if (( err = pthread_create(&tidt1, NULL, transfer_thread, (void *)t1))){
		err_quit("can't create thread t1", strerror(err));
	}
	else if (( err = pthread_create(&tidt2, NULL, transfer_thread, (void *)t2))){
		err_quit("can't create thread t2", strerror(err));
	}
	else if (( err = pthread_create(&tidib1, NULL, inquiry_balance_thread, (void *)ib1))){
		err_quit("can't create thread ib1", strerror(err));
	}
	else if (( err = pthread_create(&tidib2, NULL, inquiry_balance_thread, (void *)ib2))){
		err_quit("can't create thread ib2", strerror(err));
	}
	else if (( err = pthread_join(tidt1, &tret))) {
		 err_quit("can't join thread t1: %s\n", strerror(err));
	}
	else if (( err = pthread_join(tidt2, &tret))) {
		 err_quit("can't join thread t2: %s\n", strerror(err));
	}
	else if (( err = pthread_join(tidib1, &tret))) {
		 err_quit("can't join thread ib1: %s\n", strerror(err));
	}
	else if (( err = pthread_join(tidib2, &tret))) {
		 err_quit("can't join thread ib2: %s\n", strerror(err));
	}
	else {
		return 0;
	}

	
}
