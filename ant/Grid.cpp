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

#include "Grid.h"

#include <cstring>

TGrid::TGrid()
  {
  width = 1;
  height = 1;
  grid = new TCellValue[width * height];
  std::memset(grid,0,width * height * sizeof(TCellValue));
  }

TGrid::~TGrid()
  {
  delete [] grid;
  }

void TGrid::_copy(const TGrid & other)
  {
  width = other.GetWidth();
  height = other.GetHeight();
  grid = new TCellValue[width * height];
  std::memcpy(grid,other[0],width * height * sizeof(TCellValue));
  }

TGrid::TGrid(const TGrid & other)
  {
  _copy(other);
  }

TGrid::TGrid(uint w,uint h,TCellValue fillWith)
  {
  // do not allow empty grids
  if (w < 1)
    w = 1;
  if (h < 1)
    h = 1;

  width = w;
  height = h;
  uint count = width * height;
  grid = new TCellValue[count];
  for (uint i = 0; i < count; i++)
    grid[i] = fillWith;
  }

TGrid & TGrid::operator=(const TGrid & other)
  {
  delete [] grid;

  _copy(other);

  return *this;
  }

TCellValue * TGrid::operator[](uint idx)
  {
  return grid + idx * width;
  }

const TCellValue * TGrid::operator[](uint idx) const
  {
  return grid + idx * width;
  }

TGrid TGrid::ExtractNeighborhood(sint x, sint y, uint range, TCellValue defValue) const
  {
  // width and height of the returned grid
  uint rwidth = 2 * range + 1;

  TGrid result(rwidth,rwidth,defValue);

  for (sint i = - sint(range); i <= sint(range); i++)
    for (sint h = - sint(range); h <= sint(range); h++)
      {
      sint newx = x + i;
      sint newy = y + h;

      // check validity, otherwise the default value will be left here
      if (newx >= 0 && newy >= 0 &&
        newx < sint(width) && newy < sint(height))
        result[h + range][i + range] = (*this)[newy][newx];
      }

  return result;
  }

bool TGrid::operator==(const TGrid & other) const
  {
  if (other.width != width)
    return false;

  if (other.height != height)
    return false;

  uint ncell = height * width;
  for (uint i = 0; i < ncell; i++)
    if (grid[i] != other.grid[i])
      return false;

  return true;
  }
