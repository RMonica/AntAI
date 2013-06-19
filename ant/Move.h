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

#ifndef MOVE_H
#define MOVE_H

#include "Grid.h"
#include "Direction.h"

#include <string>
#include <list>

class TMove: public TGrid
  {
  public:
  TMove();
  virtual ~TMove();
  TMove(const TMove & other);
  explicit TMove(const TGrid & other);
  TMove(uint width,uint height,TCellValue fillWith);

  TMove &operator =(const TMove & other);
  TGrid &operator =(const TGrid & other);

  virtual bool operator==(const TMove & other) const;
  virtual bool operator==(const TGrid & other) const;

  TDirection GetChoice() const {return dir; }
  void SetChoice(TDirection choice) {if (choice >= 0 && choice < MAX_DIR) dir = choice; }

  // extracts the data in CSV format
  std::string ToString() const;

  // rotate the move counter-clockwise
  TMove Rotate(TRotation rotation) const;
  TMove Reflect(TSymmetry symmetry) const;

  // produces a list of moves by applying all the rotations and simmetries to this move
  // without duplicates
  std::list<TMove> ProduceAllRotationsAndReflections() const;

  private:
  TDirection dir;
  };

typedef std::list<TMove> TMoveList;

#endif // MOVE_H
