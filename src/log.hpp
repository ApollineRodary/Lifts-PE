#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <streambuf>
#include <string>
#include <iomanip>

using namespace std;

class DebugStreamBuffer: public streambuf {
    public:
    DebugStreamBuffer() {}
    int overflow(int c) override;
};

class DebugStream: public ostream {
    private:
    DebugStreamBuffer buffer;
    string name;

    public:
    DebugStream& operator()(const string prefix);
    DebugStream(): ostream(&buffer) {}
};

extern DebugStream debugStream;

#endif