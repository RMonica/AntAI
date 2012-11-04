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

#ifndef ANT_H
#define ANT_H

#include "defines.h"
#include "Grid.h"
#include "Direction.h"
#include "Move.h"
#include "GameDefines.h"
#include "AntAI.h"

#include <string>

class TAnt
  {
  public:
  TAnt(uint x,uint y,uint range,TGrid & map);
  ~TAnt();

  void ResetPosition(uint x,uint y);

  sint GetX() const {return x; }
  sint GetY() const {return y; }

  sint GetPoints() const {return points; }
  uint GetMoves() const {return moves; }
  uint GetVisualRange() const {return visualRange; }
  TGrid GetNeighborhood() const {return neighborhood; }

  TMove GetLastMove() const {return lastmove; }

  void DoMove(TDirection dir);

  // event emitted when the ant just moved
  virtual void AntMoved(TMove /*move*/) {}

  // uses AI to predict next move
  TDirection Predict() const;
  // set the current AI
  void SetAI(TAntAI * ai);
  std::string GetAIInfo() const {return AI->ToString(); }

  private:
  // current position
  sint x,y;

  // points scored
  sint points;

  // moves done
  uint moves;

  // visual range: if 1, sees 3x3 neighbors, if 2, 5x5
  uint visualRange;

  // part of map known after last move
  TGrid neighborhood;
  TMove lastmove;

  TAntAI * AI;

  TGrid & map;
  };

#endif // ANT_H
