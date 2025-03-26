#include "commonClasses/SiteTokens.h"
#include "commonClasses/BearerToken.h"
#include <iostream>
using namespace commonClass;
using namespace std;
int main (int argc, char **argv) {
    SiteTokens::getInstance().setKeyValue("abc", "123");
    const String k = SiteTokens::getInstance().getValueOfKey(String("abc"));
    cout << "first value" << k << endl;
    SiteTokens::getInstance().setKeyValue("abc", "345");
    const String l = SiteTokens::getInstance().getValueOfKey(String("abc"));
    cout << "last value" << l << endl;
    SiteTokens::getInstance().cleanAllKeys();
}
