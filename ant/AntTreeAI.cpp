/*
 * Copyright (C) 2012 Riccardo Monica
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AntTreeAI.h"

#include <fstream>

#include <iostream>

#include <cstdio>
#include <cstring>
#include <sstream>

TAntTreeAI::TAntTreeAI(std::string loadfromfile)
  {
  tree = NULL;

  std::ifstream ifile(loadfromfile.c_str());
  if (!ifile)
    return; // error

  std::string line;

  uint currentDepth = 0;

  TAntTreeNode * curNode = NULL;

  while (std::getline(ifile,line))
    {
    if (line == "")
      continue;

    // ---- extract data from the line ----
    uint i = 0;
    uint actualDepth = 0; // calculated counting | before the string
    unsigned int x = 0;   // x coord referred by the node
    unsigned int y = 0;   // y coord
    int l;                // limit of the node
    bool leaf = false;    // if the line specifies a leaf node, this is true
    TDirection leafdir = DIR_NONE; // the direction of the leaf node

    while (line[i] != 'c' && i < line.size())
      {
      if (line[i] == '|')
        actualDepth++;

      i++;
      }

    if (i + 4 >= line.size())
      continue; // error

    line = line.substr(i);

    line = line.substr(4); // remove "cell"

    // extract y
      {
      i = 0;
      while (line[i] != 'v' && i < line.size())
        i++;

      if (i + 1 >= line.size())
        continue; // error

      std::string ystring = line.substr(0,i);
      line = line.substr(i+1);
      sscanf(ystring.c_str(),"%u",&y);
      }

    // extract x
      {
      i = 0;
      while (line[i] != 'h' && i < line.size())
        i++;

      if (i + 1 >= line.size())
        continue; // error

      std::string xstring = line.substr(0,i);
      line = line.substr(i+1);
      sscanf(xstring.c_str(),"%u",&x);
      }

    if (line.size() < 4)
      continue; // error

    line = line.substr(1); // remove the space
    bool hchild;
    if (line[0] == '>') // h child is being defined
      {
      hchild = true;
      line = line.substr(2); // remove "> "
      }
      else
        {
        hchild = false;
        line = line.substr(3); // remove "<= "
        }

    l = 0;
    sscanf(line.c_str(),"%d",&l); // read the limit

    i = 0;
    while (line[i] != ':' && i < line.size()) // check if there is ": RESULT"
      i++;

    leaf = i < line.size();
    leafdir = DIR_NONE;
    if (leaf)
      {
      line = line.substr(i+2);

      i = 0;
      while (line[i] != ' ' && i < line.size()) // separate the next word (we hope it's a result)
        i++;

      std::string resstring = line.substr(0,i);
      for (uint h = 0; h < MAX_DIR; h++)
        if (resstring == DIRECTION_NAME[h])
          leafdir = TDirection(h);
      }

    // ---- data extraction complete ----

    while (actualDepth < currentDepth) // if the depth decreased, go to parent node
      {
      curNode = curNode->GetParent();
      currentDepth--;
      }

    if (!hchild)
      {
      TAntTreeNode * newNode = new TAntTreeNode(curNode,x,y,l);

      if (curNode)
        {
        if (!curNode->HasChildLE())
          curNode->SetChildLE(newNode);
        else if (!curNode->HasChildH())
          curNode->SetChildH(newNode);
        else std::cerr << "Adding to parent node twice\n";
        }

      if (leafdir >= 0)
        newNode->SetChildLE(new TAntTreeNode(newNode,leafdir));
        else
          currentDepth++;

      curNode = newNode;
      }
      else
        {
        if (leafdir >= 0)
          curNode->SetChildH(new TAntTreeNode(curNode,leafdir));
          else
            currentDepth++;
        }
    }

  // close all pending
  while (curNode->GetParent())
    curNode = curNode->GetParent();

  tree = curNode;

  filename = loadfromfile;
  }

TAntTreeAI::~TAntTreeAI()
  {
  if (tree)
    delete tree;
  }

TDirection TAntTreeAI::Evaluate(TGrid status, uint /*randomseed*/)
  {
  if (tree)
    return tree->Evaluate(status);

  return DIR_NORTH; // error
  }

std::string TAntTreeAI::ToString()
  {
  if (tree)
    return std::string("TAntTreeAI:\n") + filename + "\n" + tree->Print(0);

  return "TAntTreeAI: INVALID";
  }

TAntTreeNode::TAntTreeNode(TAntTreeNode * p,uint x,uint y,uint l)
  {
  parent = p;
  pointx = x;
  pointy = y;
  limit = l;
  choice = DIR_NONE;

  childh = NULL;
  childle = NULL;
  }

TAntTreeNode::TAntTreeNode(TAntTreeNode * p,TDirection c)
  {
  parent = p;
  pointx = 0;
  pointy = 0;
  limit = 0;
  choice = c;

  childh = NULL;
  childle = NULL;
  }

TAntTreeNode::~TAntTreeNode()
  {
  if (childh)
    delete childh;

  if (childle)
    delete childle;
  }

TDirection TAntTreeNode::Evaluate(TGrid & status)
  {
  if (choice != DIR_NONE)
    return choice; // this is a leaf node

  if (!childle)
    {
    std::cout << "Error: childle not found, node " << pointx << " " << pointy << "\n";
    return DIR_NORTH;
    }

  if (!childh)
    {
    std::cout << "Error: childh not found, node " << pointx << " " << pointy << "\n";
    return DIR_NORTH;
    }

  if (status.GetWidth() <= pointx || status.GetHeight() <= pointy)
    {
    std::cout << "Error: point not found in map: " << pointx << " " << pointy << "\n";
    return DIR_NORTH;
    }

  if (status[pointy][pointx] <= limit)
    return childle->Evaluate(status);
  return childh->Evaluate(status);
  }

std::string TAntTreeNode::Print(uint depth)
  {
  std::stringstream result;

  for (uint i = 0; i < depth; i++)
    result << "|   ";

  if (choice != DIR_NONE)
    {
    result << DIRECTION_NAME[choice] << "\n";
    return result.str();
    }

  result << "If cell" << pointy << "v" << pointx << "h <= " << limit << ":\n";
  result << childle->Print(depth+1);

  for (uint i = 0; i < depth; i++)
    result << "|   ";
  result << "else\n";
  result << childh->Print(depth+1);

  return result.str();
  }
