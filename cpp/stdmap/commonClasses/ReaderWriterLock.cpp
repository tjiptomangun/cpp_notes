/* 
 * File:   ReaderWriterLock.cpp
 * Author: khhi
 * 
 * Created on September 17, 2012, 10:06 AM
 */

#include <stdio.h>
#include <sys/errno.h>
#include "ReaderWriterLock.h"


using namespace commonClass;
ReaderWriterLock::ReaderWriterLock(const pthread_rwlockattr_t *att) {
    int retCode = 0;
    retCode = pthread_rwlock_init(&m_rwlock, att);
#ifdef _DEBUG
    if (retCode != 0) {
        fprintf(stderr, "Initialize rwlock failed [ErrCode:%d]", retCode);
    }
#endif   
}


ReaderWriterLock::~ReaderWriterLock() {
    pthread_rwlock_destroy(&m_rwlock);
}

/*
 * Default behavior for the lock method is acquire the lock object for writing.
 */
void ReaderWriterLock::lock() {
    lockForWriting();
}
void ReaderWriterLock::unlock() {
     int retCode = 0;
    retCode = pthread_rwlock_unlock(&m_rwlock);
#ifdef _DEBUG
    if (retCode != 0) {
        fprintf(stderr, "rwlock unlock failed [ErrCode:%d]", retCode);
    }
#endif   
}
/*
 * Default behavior for the lock method is trying acquire the lock object for writing.
 */
bool ReaderWriterLock::tryLock(unsigned long timeoutSec) {
    int retCode = 0;
    if (timeoutSec <= 0) {
        retCode = pthread_rwlock_trywrlock(&m_rwlock);
    } else {
        struct timespec abs_time;
        clock_gettime(CLOCK_REALTIME, &abs_time);
        abs_time.tv_sec += timeoutSec;
        retCode = pthread_rwlock_timedwrlock(&m_rwlock, &abs_time);
    }
    if (retCode != 0) {
        if (retCode != ETIMEDOUT) {
            #ifdef _DEBUG    
            if (retCode != 0) {
                fprintf(stderr, "rwlock tryLock failed [ErrCode:%d]", retCode);
            }
             #endif    
        }
    }
    return retCode == 0;
}

void ReaderWriterLock::lockForWriting() {
    int retCode = 0;
    retCode = pthread_rwlock_wrlock(&m_rwlock);
#ifdef _DEBUG
    if (retCode != 0) {
        fprintf(stderr, "rwlock lock for writing failed [ErrCode:%d]", retCode);
    }
#endif   
}
void ReaderWriterLock::lockForReading() {
     int retCode = 0;
    retCode = pthread_rwlock_rdlock(&m_rwlock);
#ifdef _DEBUG
    if (retCode != 0) {
        fprintf(stderr, "rwlock failed lock for reading failed [ErrCode:%d]", retCode);
    }
#endif   
}
