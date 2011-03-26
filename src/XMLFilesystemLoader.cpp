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

#include <XMLFilesystemLoader.h>

#include <FileNode.h>

#include <fstream>
#include <string.h>

using namespace std;
using namespace rapidxml;

XMLFilesystemLoader::XMLFilesystemLoader(ifstream& stream)
{
    string xmlData((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
    this->buffer = new char[xmlData.size()+1];
    strcpy(this->buffer, xmlData.c_str());
}

XMLFilesystemLoader::~XMLFilesystemLoader()
{
    if( this->buffer != NULL )
    {
        delete this->buffer;
    }
}

DirectoryNode* XMLFilesystemLoader::generateFilesystem()
{
    xml_document<> doc;
    doc.parse<0>(this->buffer);

    DirectoryNode* directoryNode = new DirectoryNode();
    xml_node<>* xmlNode = doc.first_node();

    return parseXML(xmlNode, directoryNode);
}

DirectoryNode* XMLFilesystemLoader::parseXML(rapidxml::xml_node<>* xmlNode, DirectoryNode* directoryNode)
{
    if( strcmp(xmlNode->name(), "file") == 0 )
    {
        FileNode* childNode = new FileNode();
        for(xml_attribute<>* attribute = xmlNode->first_attribute(); attribute != 0; attribute = attribute->next_attribute())
        {
            if( strcmp(attribute->name(), "name") == 0 )
            {
                childNode->setName(attribute->value());
            }
            if( strcmp(attribute->name(), "program") == 0 )
            {
                childNode->setProgram(attribute->value());
            }
        }
        directoryNode->addNode(childNode);
    }

    if( strcmp(xmlNode->name(), "dir") == 0 )
    {
        DirectoryNode* childNode = new DirectoryNode();
        for(xml_attribute<>* attribute = xmlNode->first_attribute(); attribute != 0; attribute = attribute->next_attribute())
        {
            if( strcmp(attribute->name(), "name") == 0 )
            {
                childNode->setName(attribute->value());
            }
        }
        directoryNode->addNode(childNode);

        for(xml_node<>* child = xmlNode->first_node(); child != 0; child = child->next_sibling())
        {
            parseXML(child, childNode);
        }
    }

    if( strcmp(xmlNode->name(), "root") == 0 )
    {
        directoryNode->setName("/");

        for(xml_node<>* child = xmlNode->first_node(); child != 0; child = child->next_sibling())
        {
            parseXML(child, directoryNode);
        }
    }

    return directoryNode;
}
