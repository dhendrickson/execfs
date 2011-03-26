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

#ifndef FILENODE_H_
#define FILENODE_H_

#include <Logger.h>
#include <FileSystemNode.h>
#include <Buffer.h>
#include <ProgramInstance.h>

#include <string>
#include <tr1/unordered_map>

class FileNode : public FileSystemNode
{
public:

    FileNode();
    ~FileNode();

    FileSystemNodeType getNodeType();

    void setProgram(std::string program);
    std::string getProgram();

    unsigned long open(char mode);
    void close(unsigned long handle);

    ProgramInstance* getInstance(unsigned long handle);

    static const int MAX_BUFFER = 4096;

private:

    static Logger* logger;

    unsigned long handleGenerator;

    std::string program;

    std::tr1::unordered_map<unsigned long, ProgramInstance*> programInstances;
};

#endif
