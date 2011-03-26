/*
 * Copyright (c) 2011 Dusty Hendrickson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <stdarg.h>

enum LogLevel
{
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE
};

class Logger
{
public:

    static Logger* getLogger();
    static void setName(std::string name);
    static void setLevel(LogLevel level);

    ~Logger();

    void trace(const char* message, ...);
    void debug(const char* message, ...);
    void info(const char* message, ...);
    void warn(const char* message, ...);
    void error(const char* message, ...);
    void fatal(const char* message, ...);

private:

    static Logger* logger;
    static std::string name;
    static LogLevel level;

    Logger();

    void log(LogLevel level, const char* message, va_list args);
};

#endif
