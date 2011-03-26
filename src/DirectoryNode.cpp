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

#include <DirectoryNode.h>

using namespace std;

Logger* DirectoryNode::logger = Logger::getLogger();

DirectoryNode::DirectoryNode()
{
}

DirectoryNode::~DirectoryNode()
{
    // Cleanup files
    for(unsigned int a=0; a < this->fileNodes.size(); a++)
    {
        if( this->fileNodes[a] != NULL )
        {
            delete this->fileNodes[a];
        }
    }

    // Cleanup directories
    for(unsigned int a=0; a < this->directoryNodes.size(); a++)
    {
        if( this->directoryNodes[a] != NULL )
        {
            delete this->directoryNodes[a];
        }
    }
}

FileSystemNodeType DirectoryNode::getNodeType()
{
    return DIRECTORY_NODE;
}

void DirectoryNode::addNode(FileSystemNode* node)
{
    if( node->getNodeType() == FILE_NODE )
    {
        this->fileNodes.push_back((FileNode*)node);
    }
    else if( node->getNodeType() == DIRECTORY_NODE )
    {
        this->directoryNodes.push_back((DirectoryNode*)node);
    }
}

std::vector<FileNode*> DirectoryNode::getFiles()
{
    return fileNodes;
}

std::vector<DirectoryNode*> DirectoryNode::getDirectories()
{
    return directoryNodes;
}

FileSystemNode* DirectoryNode::getNode(std::string path)
{
    FileSystemNode* node;
    bool isFound;
    DirectoryNode* directoryNode = this;
    vector<string> pathItems = splitPath(path);

    if( pathItems.size() == 0 )
    {
        return this;
    }

    for(unsigned int index=0; index < pathItems.size(); index++)
    {
        // Find directory
        isFound = false;
        vector<DirectoryNode*> directoryNodes = directoryNode->getDirectories();
        for(unsigned int a=0; a < directoryNodes.size(); a++)
        {
            if( directoryNodes[a]->getName() == pathItems[index] )
            {
                directoryNode = directoryNodes[a];
                isFound = true;
                break;
            }
        }

        // If not the last element in path
        if( index != pathItems.size()-1 )
        {
            // Couldn't find next directory in path
            if( !isFound )
            {
                node = NULL;
                break;
            }
        }
        // Last element in path, also check file nodes
        else
        {
            if( !isFound )
            {
                vector<FileNode*> fileNodes = directoryNode->getFiles();
                for(unsigned int a=0; a < fileNodes.size(); a++)
                {
                    if( fileNodes[a]->getName() == pathItems[index] )
                    {
                        node = fileNodes[a];
                        isFound = true;
                        break;
                    }
                }
                if( !isFound )
                {
                    node = NULL;
                }
            }
            else
            {
                node = directoryNode;
            }
        }
    }

    return node;
}

void DirectoryNode::merge(DirectoryNode* mergeNode)
{
    bool isFound;

    DirectoryNode* rootNode = this;

    // Can't merge if root of trees do not match
    if( rootNode->getName() != mergeNode->getName() )
    {
        return;
    }

    // Merge files
    vector<FileNode*> mergeFiles = mergeNode->getFiles();
    for(unsigned int mergeIndex=0; mergeIndex<mergeFiles.size(); mergeIndex++)
    {
        isFound = false;

        vector<FileNode*> rootFiles = rootNode->getFiles();
        for(unsigned int rootIndex=0; rootIndex<rootFiles.size(); rootIndex++)
        {
            if( rootFiles[rootIndex]->getName() == mergeFiles[mergeIndex]->getName() )
            {
                isFound = true;
                break;
            }
        }

        if( !isFound )
        {
            rootNode->addNode(mergeFiles[mergeIndex]);
        }
    }

    // Merge files
    vector<DirectoryNode*> mergeDirectories = mergeNode->getDirectories();
    for(unsigned int mergeIndex=0; mergeIndex<mergeDirectories.size(); mergeIndex++)
    {
        isFound = false;

        vector<DirectoryNode*> rootDirectories = rootNode->getDirectories();
        for(unsigned int rootIndex=0; rootIndex<rootDirectories.size(); rootIndex++)
        {
            if( rootDirectories[rootIndex]->getName() == mergeDirectories[mergeIndex]->getName() )
            {
                isFound = true;
                break;
            }
        }

        if( !isFound )
        {
            rootNode->addNode(mergeDirectories[mergeIndex]);
        }
    }
}
