#include "BearerToken.h"

using namespace commonClass;
BearerToken::BearerToken(): m_currentValue("") {
}

BearerToken::BearerToken(const String &initialValue): m_currentValue(initialValue) {
}
void BearerToken::setValue(const String &newValue) {
    m_currentValueLock.lockForWriting();
    m_currentValue = String(newValue);
    m_currentValueLock.unlock();
}

String BearerToken::getValue() {
    m_currentValueLock.lockForReading();
    String toREturn = String(m_currentValue);
    m_currentValueLock.unlock();
    return toREturn;
}