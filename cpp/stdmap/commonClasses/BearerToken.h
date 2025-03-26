#ifndef BEARERTOKEN_H
#define	BEARERTOKEN_H
#include <string>
#include "CommonTypesDef.h"
#include "ReaderWriterLock.h"
namespace commonClass {
    class BearerToken {
        public:
            BearerToken();
            BearerToken(const String &intialValue);
            void setValue(const String &newValue);
            String getValue();
        protected:
            String m_currentValue;
            ReaderWriterLock m_currentValueLock;
    };
}
#endif