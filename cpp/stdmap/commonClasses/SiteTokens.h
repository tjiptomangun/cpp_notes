#ifndef SITETOKENS_H_
#define	SITETOKENS_H_
#include <map>
#include <string>
#include "CommonTypesDef.h"
#include "BearerToken.h"
/**
    https://refactoring.guru/design-patterns/singleton/cpp/example
    https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern
*/
namespace commonClass {
        class SiteTokens {
private:
            SiteTokens() {}
            static SiteTokens *singleton_;
            SiteTokens(SiteTokens const&);
            void operator=(SiteTokens const&);

public:
            std::map<String, BearerToken *> mmap;
            void setKeyValue(String key, String value = "");
            bool isKeyEmpty(String key);
            String getValueOfKey(String key);
            SiteTokens(SiteTokens &other) = delete;
            static SiteTokens& getInstance()
            {
                static SiteTokens    instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
                return instance;
            }
            void cleanAllKeys();
        };
}
#endif