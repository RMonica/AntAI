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

#ifndef ANTAI_H
#define ANTAI_H

#include "Grid.h"
#include "Direction.h"

#include <string>

// artificial intelligence of an ant
class TAntAI
  {
  public:
  virtual TDirection Evaluate(TGrid status,uint randomseed) = 0;
  virtual std::string ToString() = 0;
  virtual ~TAntAI() {}

  private:

  };

class TStupidAntAI: public TAntAI
  {
  public:
  virtual TDirection Evaluate(TGrid status,uint randomseed);

  virtual std::string ToString() {return "TStupidAI: always north."; }
  };

class TRandomAntAI: public TAntAI
  {
  public:
  virtual TDirection Evaluate(TGrid status,uint randomseed);

  virtual std::string ToString() {return "TRandomAI: random."; }
  };

#endif // ANTAI_H
