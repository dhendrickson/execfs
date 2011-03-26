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

#include <Utility.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sstream>

using namespace std;

int execute(const char* command, Buffer* stdin, Buffer* stdout, Buffer* stderr)
{
    int pid;
    int stdinPipe[2];
    int stdoutPipe[2];
    int stderrPipe[2];
    int status;
    char commandBuffer[256];
    int size;
    char buf[256];

    strcpy(commandBuffer, command);

    status = pipe(stdinPipe);
    status = pipe(stdoutPipe);
    status = pipe(stderrPipe);

    pid = fork();

    // Check fork error
    if( pid < 0 )
    {
        return -1;
    }

    // Child process
    else if( pid == 0 )
    {
        // Close portions of pipes not used by child
        close(stdinPipe[1]);
        close(stdoutPipe[0]);
        close(stderrPipe[0]);

        // Read stdin from pipe
        dup2(stdinPipe[0], 0);
        close(stdinPipe[0]);

        // Write stdout to pipe
        dup2(stdoutPipe[1], 1);
        close(stdoutPipe[1]);

        // Write stderr to pipe
        dup2(stderrPipe[1], 2);
        close(stderrPipe[1]);

        char* cmd = NULL;
        char* args[256];
        int argc = 0;
        int length;

        char* token = strtok(commandBuffer, " ");
        while( token != NULL )
        {
            length = strlen(token) + 1;
            if( cmd == NULL )
            {
                cmd = new char[length];
                args[0] = new char[length];
                strcpy(cmd, token);
                strcpy(args[0], token);
            }
            else
            {
                args[argc] = new char[length];
                strcpy(args[argc], token);
            }
            argc++;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        // Execute command
        status = execvp(cmd, args);
        if( status == -1 )
        {
            exit(-1);
        }
    }

    // Parent process
    else if( pid != 0 )
    {
        // Close portions of pipes not used by parent
        close(stdinPipe[0]);
        close(stdoutPipe[1]);
        close(stderrPipe[1]);

        // Write stdin to child
        if( stdin != NULL )
        {
            write(stdinPipe[1], stdin->getArray(), stdin->getSize());
        }
        close(stdinPipe[1]);

        // Wait for child to finish
        waitpid(pid, &status, 0);

        // Read stdout from child
        if( stdout != NULL)
        {
            stdout->clear();
            do
            {
                size = read(stdoutPipe[0], buf, 256);
                stdout->push(buf, size);
            }
            while( size > 0 );
        }
        close(stdoutPipe[0]);

        // Read stderr from child
        if( stderr != NULL)
        {
            stderr->clear();
            do
            {
                size = read(stderrPipe[0], buf, 256);
                stderr->push(buf, size);
            }
            while( size > 0 );
        }
        close(stderrPipe[0]);

        return status;
    }

    return -1;
}

vector<string> splitPath(string path)
{
    stringstream stream(path);
    string item;
    vector<string> items;

    while( getline(stream, item, '/') )
    {
        if( item != "" )
        {
            items.push_back(item);
        }
    }

    return items;
}
