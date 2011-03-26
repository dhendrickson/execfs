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

#include <Logger.h>

#include <stdio.h>
#include <syslog.h>


using namespace std;

Logger* Logger::logger = NULL;
string Logger::name = "daemon";
LogLevel Logger::level = INFO;

Logger::Logger()
{
    openlog(Logger::name.c_str(), LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
}

Logger::~Logger()
{
    closelog();
}

Logger* Logger::getLogger()
{
    if( Logger::logger == NULL )
    {
        Logger::logger = new Logger();
    }

    return Logger::logger;
}

void Logger::setName(string name)
{
    Logger::name = name;
}

void Logger::setLevel(LogLevel level)
{
    Logger::level = level;

    switch(Logger::level)
    {
    case TRACE:
        setlogmask(LOG_UPTO(LOG_INFO)); break;
    case DEBUG:
        setlogmask(LOG_UPTO(LOG_INFO)); break;
    case INFO:
        setlogmask(LOG_UPTO(LOG_INFO)); break;
    case WARN:
        setlogmask(LOG_UPTO(LOG_WARNING)); break;
    case ERROR:
        setlogmask(LOG_UPTO(LOG_ERR)); break;
    case FATAL:
        setlogmask(LOG_UPTO(LOG_CRIT)); break;
    }
}

void Logger::log(LogLevel level, const char* message, va_list args)
{
    if( Logger::level >= level )
    {
        char buf[1024];
        int status = vsprintf(buf, message, args);
        if( status >= 0 )
        {
            syslog(LOG_INFO, buf);
        }
    }
}

void Logger::trace(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(TRACE, message, args);
    va_end(args);
}

void Logger::debug(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(DEBUG, message, args);
    va_end(args);
}

void Logger::info(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(INFO, message, args);
    va_end(args);
}

void Logger::warn(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(WARN, message, args);
    va_end(args);
}

void Logger::error(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(ERROR, message, args);
    va_end(args);
}

void Logger::fatal(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    log(FATAL, message, args);
    va_end(args);
}
