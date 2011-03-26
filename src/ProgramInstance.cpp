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

#include <ProgramInstance.h>

#include <Utility.h>
#include <string.h>

using namespace std;

Logger* ProgramInstance::logger = Logger::getLogger();

ProgramInstance::ProgramInstance(std::string program, char mode)
: program(program),
  mode(mode)
{
    if( this->mode == 'r' )
    {
        execute(this->program.c_str(), NULL, &stdout, &stderr);
        logger->info("stdout size: %i", stdout.getSize());
    }
}

ProgramInstance::~ProgramInstance()
{
    if( this->mode == 'w' )
    {
        execute(this->program.c_str(), NULL, &stdout, &stderr);
    }
}

int ProgramInstance::read(char* buf, int size, int offset)
{
    int length;

    if( offset > this->stdout.getSize() )
    {
        length = 0;
    }
    else if( offset + size > this->stdout.getSize() )
    {
        length = this->stdout.getSize() - offset;
    }
    else
    {
        length = size;
    }

    memcpy(buf, this->stdout.getArray() + offset, length);

    return length;
}

int ProgramInstance::write(const char* buf, int size, int offset)
{
    return 0;
}
