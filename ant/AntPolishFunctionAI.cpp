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

#include "AntPolishFunctionAI.h"

#include <fstream>
using std::istream;
using std::ifstream;
#include <sstream>

#define K 100

class TAntPolishFunctionAI::TPolish
  {
  public:
  virtual ~TPolish() {}
  virtual double Evaluate(const TGrid &status,uint randomseed) = 0;
  virtual bool IsLoadOk() = 0;
  virtual std::string ToMathString() = 0;
  };

namespace NAntPolishFunctionAI
  {
  TAntPolishFunctionAI::TPolish * FuncFactory(istream * ifile);

  class BinaryOp: public TAntPolishFunctionAI::TPolish
    {
    public:
    BinaryOp(istream * ifile)
      {
      first = FuncFactory(ifile);
      second = FuncFactory(ifile);
      }

    ~BinaryOp()
      {
      if (first)
        delete first;
      if (second)
        delete second;
      }

    virtual double BinaryEval(const TGrid &status,uint randomseed,double v1,double v2) = 0;

    virtual std::string BinaryMathString(std::string s1,std::string s2) = 0;

    double Evaluate(const TGrid &status,uint randomseed)
      {
      return BinaryEval(status,randomseed,first->Evaluate(status,randomseed),second->Evaluate(status,randomseed));
      }

    bool IsLoadOk()
      {
      return first && second && first->IsLoadOk() && second->IsLoadOk();
      }

    std::string ToMathString()
      {
      return BinaryMathString(first->ToMathString(),second->ToMathString());
      }

    private:
    TAntPolishFunctionAI::TPolish * first;
    TAntPolishFunctionAI::TPolish * second;
    };

  class Plus: public BinaryOp
    {
    public:
    Plus(istream * stream): BinaryOp(stream) {}

    virtual double BinaryEval(const TGrid &,uint,double v1,double v2)
      {
      return v1 + v2;
      }

    virtual std::string BinaryMathString(std::string s1,std::string s2)
      {
      return "(" + s1 + "+" + s2 + ")";
      }
    };

  class Minus: public BinaryOp
    {
    public:
    Minus(istream * stream): BinaryOp(stream) {}

    virtual double BinaryEval(const TGrid &,uint,double v1,double v2)
      {
      return v1 - v2;
      }

    virtual std::string BinaryMathString(std::string s1,std::string s2)
      {
      return "(" + s1 + "-" + s2 + ")";
      }
    };

  class Multiply: public BinaryOp
    {
    public:
    Multiply(istream * stream): BinaryOp(stream) {}

    virtual double BinaryEval(const TGrid &,uint,double v1,double v2)
      {
      return v1 * v2;
      }

    virtual std::string BinaryMathString(std::string s1,std::string s2)
      {
      return "(" + s1 + "*" + s2 + ")";
      }
    };

  class Divide: public BinaryOp
    {
    public:
    Divide(istream * stream): BinaryOp(stream) {}

    virtual double BinaryEval(const TGrid &,uint,double v1,double v2)
      {
      if (v2 == 0.0)
        v2 = 1.0;

      return v1 / v2;
      }

    virtual std::string BinaryMathString(std::string s1,std::string s2)
      {
      return "(" + s1 + "/" + s2 + ")";
      }
    };

  class ProtectedDivide: public BinaryOp
    {
    public:
    ProtectedDivide(istream * stream): BinaryOp(stream) {}

    virtual double BinaryEval(const TGrid &,uint,double v1,double v2)
      {
      if (v2 == 0.0)
        return 1.0;

      return v1 / v2;
      }

    virtual std::string BinaryMathString(std::string s1,std::string s2)
      {
      return "(SD(" + s1 + "," + s2 + "))";
      }
    };

  class Constant: public TAntPolishFunctionAI::TPolish
    {
    public:
    Constant(std::string valstr)
      {
      std::istringstream iss(valstr);
      val = 0.0;
      iss >> val;
      isOk = !iss.fail();
      }

    ~Constant()
      {
      }

    double Evaluate(const TGrid &/*status*/,uint /*randomseed*/)
      {
      return val;
      }

    bool IsLoadOk()
      {
      return isOk;
      }

    std::string ToMathString()
      {
      std::ostringstream oss;
      oss << val;
      return oss.str();
      }

    private:
    double val;
    bool isOk;
    };

  class CellValue: public TAntPolishFunctionAI::TPolish
    {
    public:
    CellValue(istream * ifile)
      {
      (*ifile) >> x;
      (*ifile) >> y;
      is_ok = !(*ifile).fail();
      }

    ~CellValue()
      {
      }

    double Evaluate(const TGrid &status,uint)
      {
      return status[y][x];
      }

    bool IsLoadOk()
      {
      return is_ok;
      }

    std::string ToMathString()
      {
      std::ostringstream out;
      out << "[" << x << "," << y << "]";
      return out.str();
      }

    private:
    uint x,y;
    bool is_ok;
    };

  class IfOp: public TAntPolishFunctionAI::TPolish
    {
    public:
    IfOp(istream * ifile)
      {
      first = FuncFactory(ifile);
      second = FuncFactory(ifile);
      third = FuncFactory(ifile);
      }

    ~IfOp()
      {
      if (first)
        delete first;
      if (second)
        delete second;
      if (third)
        delete third;
      }

    double Evaluate(const TGrid &status,uint randomseed)
      {
      double cond = first->Evaluate(status,randomseed);
      if (cond >= 0.0)
        return second->Evaluate(status,randomseed);
      return third->Evaluate(status,randomseed);
      }

    bool IsLoadOk()
      {
      return first && second && third && first->IsLoadOk() && second->IsLoadOk() && third->IsLoadOk();
      }

    std::string ToMathString()
      {
      return "(IF(" + first->ToMathString() + ">=0)THEN" + second->ToMathString() + "ELSE" + third->ToMathString() + ")";
      }

    private:
    TAntPolishFunctionAI::TPolish * first;
    TAntPolishFunctionAI::TPolish * second;
    TAntPolishFunctionAI::TPolish * third;
    };

  class Threshold: public TAntPolishFunctionAI::TPolish
    {
    public:
    Threshold(istream * ifile)
      {
      first = FuncFactory(ifile);
      }

    ~Threshold()
      {
      if (first)
        delete first;
      }

    double Evaluate(const TGrid &status,uint randomseed)
      {
      double cond = first->Evaluate(status,randomseed);
      if (cond >= 0.0)
        return 1.0;
      return -1.0;
      }

    bool IsLoadOk()
      {
      return first && first->IsLoadOk();
      }

    std::string ToMathString()
      {
      return "(TH" + first->ToMathString() + ")";
      }

    private:
    TAntPolishFunctionAI::TPolish * first;
    };

  TAntPolishFunctionAI::TPolish * FuncFactory(istream * ifile)
    {
    std::string name;
    if (!ifile)
      return NULL;

    if (!((*ifile) >> name))
      return NULL;

    if (name == "+")
      return new NAntPolishFunctionAI::Plus(ifile);
    else if (name == "-")
      return new NAntPolishFunctionAI::Minus(ifile);
    else if (name == "*")
      return new NAntPolishFunctionAI::Multiply(ifile);
    else if (name == "/")
      return new NAntPolishFunctionAI::Divide(ifile);
    else if (name == "SD")
      return new NAntPolishFunctionAI::ProtectedDivide(ifile);
    else if (name == "V")
      return new NAntPolishFunctionAI::CellValue(ifile);
    else if (name == "IF")
      return new NAntPolishFunctionAI::IfOp(ifile);
    else if (name == "TH")
      return new NAntPolishFunctionAI::Threshold(ifile);
    else
      return new NAntPolishFunctionAI::Constant(name);
    }
  }

TAntPolishFunctionAI::TAntPolishFunctionAI(std::string fromfile)
  {
  func = NULL;
  load_ok = false;
  filename = fromfile;

  ifstream ifile(fromfile.c_str());
  if (!ifile)
    return;

  func = NAntPolishFunctionAI::FuncFactory(&ifile);
  load_ok = func && func->IsLoadOk();
  }

TAntPolishFunctionAI::~TAntPolishFunctionAI()
  {
  if (func)
    delete func;
  }

TDirection TAntPolishFunctionAI::Evaluate(TGrid status,uint randomseed)
  {
  if (!load_ok || !func)
    return DIR_NORTH;

  double result = func->Evaluate(status,randomseed);

  /*
  if (movestr.equalsIgnoreCase("EAST"))
    dir = 2;
  else if (movestr.equalsIgnoreCase("SOUTH"))
    dir = 1;
  else if (movestr.equalsIgnoreCase("NORTH"))
    dir = -1;
  else if (movestr.equalsIgnoreCase("WEST"))
    dir = -2;
  */

  if (result >= K)
    return DIR_EAST;
  if (result < K && result >= 0)
    return DIR_SOUTH;
  if (result < 0 && result >= -K)
    return DIR_NORTH;
  if (result < -K)
    return DIR_WEST;

  return DIR_NORTH; // should never happen
  }

std::string TAntPolishFunctionAI::ToString()
  {
  std::string result = "TAntPolishFunctionAI:\n";
  result += "Load: ";
  if (load_ok)
    result += "OK";
    else
      result += "FAILED";
  result += "\n";

  if (load_ok)
    {
    result += "Loaded function is:\n";
    result += func->ToMathString();
    }

  return result;
  }
