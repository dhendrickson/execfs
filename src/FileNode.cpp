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

#include <FileNode.h>

using namespace std;

Logger* FileNode::logger = Logger::getLogger();

FileNode::FileNode()
: handleGenerator(1)
{
}

FileNode::~FileNode()
{
}

FileSystemNodeType FileNode::getNodeType()
{
    return FILE_NODE;
}

void FileNode::setProgram(string program)
{
    this->program = program;
}

string FileNode::getProgram()
{
    return this->program;
}

ProgramInstance* FileNode::getInstance(unsigned long handle)
{
    return this->programInstances[handle];
}

unsigned long FileNode::open(char mode)
{
    unsigned long handle = this->handleGenerator;
    ProgramInstance* programInstance = new ProgramInstance(this->program, mode);

    this->programInstances[handle] = programInstance;
    this->handleGenerator++;

    return handle;
}

void FileNode::close(unsigned long handle)
{
    delete programInstances[handle];
    this->programInstances.erase(handle);
}
