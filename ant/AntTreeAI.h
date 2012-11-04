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

#ifndef ANTTREEAI_H
#define ANTTREEAI_H

#include "AntAI.h"
#include "defines.h"
#include "Grid.h"

#include <string>

class TAntTreeNode
  {
  public:
  TAntTreeNode(TAntTreeNode * parent,uint x,uint y,uint limit); // branching node
  TAntTreeNode(TAntTreeNode * parent,TDirection choice); // leaf node
  ~TAntTreeNode();

  TDirection Evaluate(TGrid & status);

  std::string Print(uint depth);

  TAntTreeNode * GetParent() {return parent; }
  void SetChildH(TAntTreeNode * child) {childh = child; }
  void SetChildLE(TAntTreeNode * child) {childle = child; }

  bool HasChildLE() {return childle; }
  bool HasChildH() {return childh; }

  private:
  TAntTreeNode * parent;
  TAntTreeNode * childh; // child called if status[pointx][pointy] is higher than limit
  TAntTreeNode * childle; // child called if lower or equal

  uint pointx;
  uint pointy;
  sint limit;

  TDirection choice; // the choice (set if this is a leaf node)
  };

class TAntTreeAI : public TAntAI
  {
  public:
  TAntTreeAI(std::string loadfromfile);
  ~TAntTreeAI();

  virtual TDirection Evaluate(TGrid status);

  virtual std::string ToString();

  private:
  TAntTreeNode * tree;

  std::string filename;
  };

#endif // ANTTREEAI_H
