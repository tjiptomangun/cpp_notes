/* 
 * File:   ReaderWriterLock.h
 * Author: khhi
 *
 * Created on September 17, 2012, 10:06 AM
 */

#ifndef READERWRITERLOCK_H
#define	READERWRITERLOCK_H

#include <pthread.h>
#include "CommonTypesDef.h"


namespace commonClass {

    /*
     * Single writer and multi-reader at the certain time.
     */
    class ReaderWriterLock  {
    public:
        ReaderWriterLock(const pthread_rwlockattr_t *att = NULL);
        virtual ~ReaderWriterLock();
        virtual void lock();
        virtual void unlock();
        virtual bool tryLock(ulong timeoutSec = 0);
        /**
         * The caller threads will acquires the lock if no other reader or writer thread
         * is owner of the lock. Otherwise, the caller will be blocked until the the lock
         * become available.
         */
        virtual void lockForWriting();
        /** 
         * The calling thread  acquires  the read  lock  if a writer does not hold 
         * the lock and there are no writers blocked on the lock.
         */
        virtual void lockForReading();
    private:
        pthread_rwlock_t m_rwlock;
    };
}
#endif	/* READERWRITERLOCK_H */

