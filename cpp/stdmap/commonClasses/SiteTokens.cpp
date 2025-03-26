#include <map>
#include <string>
#include "CommonTypesDef.h"
#include "BearerToken.h"
#include "SiteTokens.h"

using namespace commonClass;
void SiteTokens::setKeyValue(String key, String value){
    if (this->mmap.count(key) == 0) {
        this->mmap[key] = new BearerToken(value);
    } else {
        this->mmap[key]->setValue(value);
    }
}

bool SiteTokens::isKeyEmpty(String key){
    if (this->mmap.count(key) == 0) {
        return true;
    } else {
        return this->mmap[key]->getValue().empty();
    }
}

String SiteTokens::getValueOfKey(String key) {
    if (this->mmap.count(key) == 0) {
        return String("");
    } else {
        return this->mmap[key]->getValue();
    }
}

void SiteTokens::cleanAllKeys() {
    	if(this->mmap.size() > 0) {
		 for(std::map<String, BearerToken *>::iterator it=this->mmap.begin() ; it!=this->mmap.end() ; it++) {
			 delete it->second;
		 }
		 this->mmap.clear();
	}
}