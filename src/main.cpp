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

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <vector>
#include <string>
#include <fstream>

#include <Logger.h>
#include <FileNode.h>
#include <DirectoryNode.h>
#include <ProgramInstance.h>
#include <Utility.h>
#include <XMLFilesystemLoader.h>

using namespace std;

Logger* logger;
DirectoryNode* rootNode;

int fuse_getattr(const char* path, struct stat* stbuf)
{
    int status = -ENOENT;

    memset(stbuf, 0, sizeof(struct stat));

    logger->info("Get attribute: path - %s", path);
    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == DIRECTORY_NODE )
        {
            stbuf->st_mode = S_IFDIR | 0755;
            stbuf->st_nlink = 2;
            status = 0;
        }
        else if( node->getNodeType() == FILE_NODE )
        {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = 65536;
            status = 0;
        }
    }

    return status;
}

int fuse_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* info)
{
    int status = -ENOENT;

    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == DIRECTORY_NODE )
        {
            filler(buf, ".", NULL, 0);
            filler(buf, "..", NULL, 0);

            vector<DirectoryNode*> directoryNodes = ((DirectoryNode*)node)->getDirectories();
            for(unsigned int a=0; a < directoryNodes.size(); a++)
            {
                filler(buf, directoryNodes[a]->getName().c_str(), NULL, 0);
            }

            vector<FileNode*> fileNodes = ((DirectoryNode*)node)->getFiles();
            for(unsigned int a=0; a < fileNodes.size(); a++)
            {
                filler(buf, fileNodes[a]->getName().c_str(), NULL, 0);
            }

            status = 0;
        }
    }

    return status;
}

int fuse_open(const char* path, struct fuse_file_info* info)
{
    int status = -ENOENT;
    int handle;

    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == FILE_NODE )
        {
            handle = ((FileNode*)node)->open('r');
            info->fh = handle;
            status = 0;
        }
    }

    return status;
}

int fuse_release(const char* path, struct fuse_file_info* info)
{
    int status = -ENOENT;

    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == FILE_NODE )
        {
            ((FileNode*)node)->close(info->fh);
            status = 0;
        }
    }

    return status;
}


int fuse_read(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* info)
{
    logger->debug("Entering read\n");

    int status = -ENOENT;

    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == FILE_NODE )
        {
            ProgramInstance* instance = ((FileNode*)node)->getInstance(info->fh);
            status = instance->read(buf, size, offset);
        }
    }

    return status;
}

int fuse_write(const char* path, const char* buf, size_t size, off_t offset, struct fuse_file_info* info)
{
    logger->debug("Entering write\n");

    int status = -ENOENT;

    FileSystemNode* node = rootNode->getNode(string(path));
    if( node != NULL )
    {
        if( node->getNodeType() == FILE_NODE )
        {
            ProgramInstance* instance = ((FileNode*)node)->getInstance(info->fh);
            status = instance->write(buf, size, offset);
        }
    }

    return status;
}

int main(int argc, char *argv[])
{
    int status;

    Logger::setName("fuse");
    Logger::setLevel(TRACE);
    logger = Logger::getLogger();

    static fuse_operations fuseOps;
    fuseOps.getattr = fuse_getattr;
    fuseOps.readdir = fuse_readdir;
    fuseOps.open = fuse_open;
    fuseOps.release = fuse_release;
    fuseOps.read = fuse_read;
    fuseOps.write = fuse_write;

    rootNode = new DirectoryNode();
    rootNode->setName("/");

    ifstream stream("conf/filesystem.xml");
    XMLFilesystemLoader loader(stream);
    DirectoryNode* tmpNode = loader.generateFilesystem();
    rootNode->merge(tmpNode);

    status = fuse_main(argc, argv, &fuseOps, NULL);

    return status;
}
