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

#ifndef GRID_H
#define GRID_H

#include "defines.h"

class TGrid
  {
  public:
  TGrid();
  virtual ~TGrid();
  TGrid(const TGrid & other);
  TGrid(uint width,uint height,TCellValue fillWith);

  TGrid & operator=(const TGrid & other);

  virtual bool operator==(const TGrid & other) const;

  TCellValue * operator[](uint idx);

  const TCellValue * operator[](uint idx) const;

  uint GetWidth() const {return width; }
  uint GetHeight() const {return height; }

  TGrid ExtractNeighborhood(sint x, sint y, uint range, TCellValue defValue) const;

  private:

  uint width;
  uint height;
  TCellValue * grid;

  void _copy(const TGrid & other);
  };

#endif // GRID_H
