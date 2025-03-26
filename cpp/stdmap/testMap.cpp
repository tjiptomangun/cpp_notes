#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <map>
#include <fstream>
#include <vector>
#include "strtool.h"
#include <pthread.h>
enum commonRetCode {
          errorSuccess = 0              /* Error Success */
};
typedef std::string String;
class Lockable {
public:
	Lockable();
	virtual ~Lockable();
	/**
	 * Acquire lock to take the owner of synchronize object.
	 */
	virtual void lock() = 0;
	/**
	 * Release the lock.
	 */
	virtual void unlock() = 0;
	/**
	 * This function equivalent the lock() function except if the lock object are
	 * locked by the other thread the function will return immediately in case we
	 * not specific the timeout (timeout=0). But if we given timeout value, the caller
	 * thread will be wait for the specified timeout and return.
	 * given time period.
	 * @param timeout 
	 * @return True if can acquire lock object in the given time, 
	 * otherwise return false.
	 */
	virtual bool tryLock(ulong timeout = 0) = 0;
};

Lockable::Lockable() {

}

Lockable::~Lockable() {

}

class ReaderWriterLock : public Lockable {
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

ReaderWriterLock::ReaderWriterLock(const pthread_rwlockattr_t *att) {
    int retCode = errorSuccess;
    retCode = pthread_rwlock_init(&m_rwlock, att);
    if (retCode != errorSuccess) {
        fprintf(stderr, "Initialize rwlock failed [ErrCode:%d]", retCode);
    }

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
     int retCode = errorSuccess;
    retCode = pthread_rwlock_unlock(&m_rwlock);
    if (retCode != errorSuccess) {
        fprintf(stderr, "rwlock unlock failed [ErrCode:%d]", retCode);
    }

}
/*
 * Default behavior for the lock method is trying acquire the lock object for writing.
 */
bool ReaderWriterLock::tryLock(unsigned long timeoutSec) {
    int retCode = errorSuccess;
    if (timeoutSec <= 0) {
        retCode = pthread_rwlock_trywrlock(&m_rwlock);
    } else {
        struct timespec abs_time;
        clock_gettime(CLOCK_REALTIME, &abs_time);
        abs_time.tv_sec += timeoutSec;
        retCode = pthread_rwlock_timedwrlock(&m_rwlock, &abs_time);
    }
    if (retCode != errorSuccess) {
        if (retCode != ETIMEDOUT) {
            #ifdef _DEBUG    
            if (retCode != errorSuccess) {
                fprintf(stderr, "rwlock tryLock failed [ErrCode:%d]", retCode);
            }
             #endif    
        }
    }
    return retCode == errorSuccess;
}

void ReaderWriterLock::lockForWriting() {
    int retCode = errorSuccess;
    retCode = pthread_rwlock_wrlock(&m_rwlock);
    if (retCode != errorSuccess) {
        fprintf(stderr, "rwlock lock for writing failed [ErrCode:%d]", retCode);
    }
}
void ReaderWriterLock::lockForReading() {
     int retCode = errorSuccess;
    retCode = pthread_rwlock_rdlock(&m_rwlock);
    if (retCode != errorSuccess) {
        fprintf(stderr, "rwlock failed lock for reading failed [ErrCode:%d]", retCode);
    }
}
class BearerToken {
        public:
            BearerToken();
            BearerToken(const String &intialValue);
            void setValue(const String &newValue);
            String getValue();
        protected:
            String m_currentValue;
			ReaderWriterLock m_rwLock;
    };
BearerToken::BearerToken(): m_currentValue("") {
}
BearerToken::BearerToken(const String &initialValue): m_currentValue(initialValue) {
}

void BearerToken::setValue(const String &newValue) {
    m_rwLock.lockForWriting();
    m_currentValue = String(newValue);
    m_rwLock.unlock();
}

String BearerToken::getValue() {
    m_rwLock.lockForReading();
    String toReturn = String(m_currentValue);
    m_rwLock.unlock();
    return toReturn;
}

class SiteTokens {
   public: 
	SiteTokens();
	~SiteTokens();
	void setKeyValue(String key, String value = "");
	String getKey(String key);
   protected: 
   	std::map<String, BearerToken *> mmap;
};
SiteTokens::SiteTokens() {
}
SiteTokens::~SiteTokens() {
	if(this->mmap.size() > 0) {
		 for(std::map<String, BearerToken *>::iterator it=this->mmap.begin() ; it!=this->mmap.end() ; it++) {
			 delete it->second;
		 }
		 this->mmap.clear();
	}
}
void SiteTokens::setKeyValue(String key, String value) {
	if (this->mmap.count(key) > 0) {
		this->mmap[key]->setValue(value);
	}
	else {
		this->mmap[key] = new BearerToken(value);
	} 
}

String SiteTokens::getKey(String key) {
	if(this->mmap.count(key) > 0) {
		return this->mmap[key]->getValue();
	} else {
		return String("");
	}
}
void checkKey(char *key, std::map<std::string, BearerToken*> m) {
	const char *absentStr = "absent";

	if (m.count(key) > 0) {
		printf("[%s] is %s\n", key, m[key]-> getValue().c_str());
	}
	else {
		printf("[%s] is %s\n", key, absentStr);
	}
}
//https://stackoverflow.com/a/46931770
std::vector<String> split(String s, String delim) {
	size_t pos_start = 0, pos_end, delim_len = delim.length();
	String token;
	std::vector<String> res;
    while ((pos_end = s.find(delim, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;	
}
SiteTokens siteTokens;
#ifdef MAPTEST

// https://www.delftstack.com/howto/cpp/how-to-read-a-file-line-by-line-cpp/
void *writerThread(void *tmp) {
	char fname [200] = {0};
	const char * strTmp = (char * )tmp;
	std::strncpy(fname, strTmp, 200);
	while(1) {
		std::ifstream input_file(fname);
		String line;
		if (! input_file.is_open()) {
			std::cerr << "Could not open the file - '" << fname << "'" << std::endl;
			pthread_exit((void *)2);
		} else {
			while (getline(input_file, line)) {
				std::vector<String> kv  = split(line, ":");
				siteTokens.setKeyValue(trim_copy(kv[0]), trim_copy(kv[1]));
			}
		}
		input_file.close();
		usleep(2000);
	}
	pthread_exit((void *)0);
}

void *readerThread(void *tmp) {
	char fname [200] = {0};
	const char * strTmp = (char * )tmp;
	std::strncpy(fname, strTmp, 200);
	while(1) {
		std::ifstream input_file(fname);
		String line;
		if (! input_file.is_open()) {
			std::cerr << "Could not open the file - '" << fname << "'" << std::endl;
			pthread_exit((void *)2);
		} else {
			while (getline(input_file, line)) {
				std::vector<String> kv  = split(line, ":");
				const String res = siteTokens.getKey(trim_copy(kv[0]));
				std::cout << "key "  << trim_copy(kv[0]) << " value " << res <<std::endl;
			}
		}
		input_file.close();
		usleep(2000);
	}
	
	pthread_exit((void *)0);

}
int main(int argc, char **argv) {

	pthread_t w1, w2, w3;
	pthread_t r1, r2, r3, r4, r5;

	const char * file1 = "file1.txt";
	const char * file2 = "file2.txt";
	const char * file3 = "file3.txt";
	if (pthread_create(&w1, NULL, writerThread, (void *)file1)) {
		return 1;
	} else if (pthread_create(&w2, NULL, writerThread, (void *)file2)) {
		return 1;
	} else if (pthread_create(&w3, NULL, writerThread, (void *)file3)) {
		return 1;
	} else if (pthread_create(&r1, NULL, readerThread, (void *)file1)) {
		return 1;
	} else if (pthread_create(&r2, NULL, readerThread, (void *)file1)) {
		return 1;
	} else if (pthread_create(&r3, NULL, readerThread, (void *)file2)) {
		return 1;
	} else if (pthread_create(&r4, NULL, readerThread, (void *)file2)) {
		return 1;
	} else if (pthread_create(&r5, NULL, readerThread, (void *)file3)) {
		return 1;
	} else if (pthread_join(w1, NULL)) {
		return 1;
	} else if (pthread_join(w2, NULL)) {
		return 1;
	} else if (pthread_join(w3, NULL)) {
		return 1;
	} else if (pthread_join(r1, NULL)) {
		return 1;
	} else if (pthread_join(r2, NULL)) {
		return 1;
	} else if (pthread_join(r3, NULL)) {
		return 1;
	} else if (pthread_join(r4, NULL)) {
		return 1;
	} else if (pthread_join(r5, NULL)) {
		return 1;
	}

	
	
	/*
	 std::map<std::string, BearerToken*> m{{"a", new BearerToken()}};
	 m["b"] = new BearerToken("xyz");
	 checkKey("a", m); 
	 checkKey("b", m); 
	 checkKey("c", m); 

	
	delete m["a"] ;
	delete m["b"] ;
	*/

}
#endif