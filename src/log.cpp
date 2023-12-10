#include "log.hpp"

DebugStream debugStream;

DebugStream& DebugStream::operator()(const string prefix) {
    //return *this;
    cout << "@" << prefix << string(30 - prefix.length(), ' ');
    return *this;
}

int DebugStreamBuffer::overflow(int c) {
    //return c;
    switch(c) {
        case '!':
            debugStream << setw(10);
            break;
        
        case '?':
            debugStream << setw(20);
            break;
        
        default:
            cout.put(c);
            break;
    }
    return c;
}
