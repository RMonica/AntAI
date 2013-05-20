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

#include "Move.h"

#include <sstream>
#include <algorithm>

TMove::TMove()
  {
  dir = DIR_NONE;
  }

TMove::~TMove()
  {

  }

TMove::TMove(const TMove & other):TGrid(other)
  {
  dir = other.dir;
  }

TMove::TMove(const TGrid & other):TGrid(other)
  {
  dir = DIR_NONE;
  }

TMove::TMove(uint width,uint height,TCellValue fillWith):TGrid(width,height,fillWith)
  {
  dir = DIR_NONE;
  }

TMove & TMove::operator =(const TMove & other)
  {
  (*dynamic_cast<TGrid *>(this)) = other;
  dir = other.dir;

  return *this;
  }

TGrid & TMove::operator =(const TGrid & other)
  {
  (*dynamic_cast<TGrid *>(this)) = other;
  dir = DIR_NONE;

  return *this;
  }

std::string TMove::ToString() const
  {
  if (dir < 0 || dir >= MAX_DIR)
    return ""; // no move defined yet

  uint width = GetWidth();
  uint height = GetHeight();

  std::stringstream result;

  for (uint i = 0; i < height; i++)
    for (uint h = 0; h < width; h++)
      result << ((*this)[i][h]) << ",";

  result << DIRECTION_NAME[dir];

  return result.str();
  }

TMove TMove::Rotate(TRotation rotation) const
  {
  // create rotation composing symmetries
  switch (rotation)
    {
    case ROTATION_NONE:
      return (*this);
    case ROTATION_ONE:
      return Reflect(SYMMETRY_SECDIAG).Reflect(SYMMETRY_VERTICAL);
    case ROTATION_TWO:
      return Reflect(SYMMETRY_VERTICAL).Reflect(SYMMETRY_HORIZONTAL);
    case ROTATION_THREE:
      return Reflect(SYMMETRY_MAINDIAG).Reflect(SYMMETRY_VERTICAL);
    default:
      return (*this);
    }
  }

TMove TMove::Reflect(TSymmetry symmetry) const
  {
  uint mywidth = GetWidth();
  uint myheight = GetHeight();

  switch (symmetry)
    {
    case SYMMETRY_NONE:
      return (*this);
    case SYMMETRY_MAINDIAG:
      {
      TMove result(myheight,mywidth,0);
      // switch coordinates
      for (uint v = 0; v < myheight; v++)
        for (uint h = 0; h < mywidth; h++)
          result[h][v] = (*this)[v][h];

      if (GetChoice() >= 0)
        result.SetChoice(SYMMETRY_APPLY_DIRECTION[GetChoice()][symmetry]);

      return result;
      }
    case SYMMETRY_SECDIAG:
      {
      TMove result(myheight,mywidth,0);
      // switch coordinates and subtract from width/height
      for (uint v = 0; v < myheight; v++)
        for (uint h = 0; h < mywidth; h++)
          result[h][v] = (*this)[myheight - v - 1][mywidth - h - 1];

      if (GetChoice() >= 0)
        result.SetChoice(SYMMETRY_APPLY_DIRECTION[GetChoice()][symmetry]);

      return result;
      }
    case SYMMETRY_VERTICAL:
      {
      TMove result(mywidth,myheight,0);
      // reverse X coord
      for (uint v = 0; v < myheight; v++)
        for (uint h = 0; h < mywidth; h++)
          result[v][h] = (*this)[v][mywidth - h - 1];

      if (GetChoice() >= 0)
        result.SetChoice(SYMMETRY_APPLY_DIRECTION[GetChoice()][symmetry]);

      return result;
      }
    case SYMMETRY_HORIZONTAL:
      {
      TMove result(mywidth,myheight,0);
      // reverse Y coord
      for (uint v = 0; v < myheight; v++)
        for (uint h = 0; h < mywidth; h++)
          result[v][h] = (*this)[myheight - v - 1][h];

      if (GetChoice() >= 0)
        result.SetChoice(SYMMETRY_APPLY_DIRECTION[GetChoice()][symmetry]);

      return result;
      }
    default:
      return (*this);
    }
  }

TMoveList TMove::ProduceAllRotationsAndReflections() const
  {
  TMoveList result;

  for (uint r = ROTATION_NONE; r < MAX_ROTATION; r++)
    {
    TMove rotated = this->Rotate(TRotation(r));
    for (uint s = SYMMETRY_NONE; s < MAX_SYMMETRY; s++)
      {
      TMove reflected = rotated.Reflect(TSymmetry(s));

      TMoveList::iterator o = std::find(result.begin(),result.end(),reflected);
      if (o == result.end()) // avoid duplicates
        result.push_back(reflected);
      }
    }

  return result;
  }

bool TMove::operator==(const TMove & other) const
  {
  return TGrid::operator==(other) && dir == other.dir;
  }

bool TMove::operator==(const TGrid & other) const
  {
  const TMove * maybeMove = dynamic_cast<const TMove *>(&other);

  if (!maybeMove)
    return TGrid::operator==(other);

  return operator==(*maybeMove);
  }

