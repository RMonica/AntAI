/*
 * Copyright (C) 2012-2013 Riccardo Monica
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

#ifndef ANTPOLISHFUNCTIONAI_H
#define ANTPOLISHFUNCTIONAI_H

#include "AntAI.h"
#include "defines.h"

class TAntPolishFunctionAI : public TAntAI
  {
  public:
  TAntPolishFunctionAI(std::string fromfile);
  ~TAntPolishFunctionAI();

  virtual TDirection Evaluate(TGrid status,uint randomseed);
  virtual std::string ToString();

  class TPolish;

  private:
  bool load_ok;
  std::string filename;
  TPolish * func;
  };

#endif // ANTPOLISHFUNCTIONAI_H
