#include <string>
#include <iostream>
#include <unistd.h>
typedef std::string String;
class BearerToken {
        public:
            BearerToken(const String &name);
            BearerToken(const String &name, const String &intialValue);
            void setValue(const String &newValue);
            String getValue();
            String m_name;
        protected:
            String m_currentValue;
    };
BearerToken::BearerToken(const String &name): m_name(name), m_currentValue("") {
}
BearerToken::BearerToken(const String &name, const String &initialValue): m_name(name), m_currentValue(initialValue) {
}

void BearerToken::setValue(const String &newValue) {
    m_currentValue = String(newValue);
}

String BearerToken::getValue() {
    String toReturn = String(m_currentValue);
    return toReturn;
}

void setOneHundredTimes(BearerToken &toSet, String valueToSet) {
	for (int i = 0; i < 100; i++) { 
		toSet.setValue(valueToSet);
	}
}
void getOneHundredTimes(BearerToken &toGet) {
	for (int i = 0; i < 100; i++) { 
		std::cout << toGet.m_name << ":" << toGet.getValue() << std::endl;
	}
}

int main(int argc, char **argv) {
	BearerToken  token1 = BearerToken("token1");
	BearerToken  token2 = BearerToken("token2", "abc");

	while(1) {
		setOneHundredTimes(token1, String("def"));
		setOneHundredTimes(token2, String("ghi "));
		getOneHundredTimes(token1);
		getOneHundredTimes(token2);
		usleep(5000);
	}
}
