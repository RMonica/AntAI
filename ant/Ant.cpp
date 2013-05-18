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

#include "Ant.h"

TAnt::TAnt(uint cx,uint cy,uint range,TGrid & m): map(m)
  {
  visualRange = range;
  AI = new TStupidAntAI();

  ResetPosition(cx,cy);
  }

TAnt::~TAnt()
  {
  if (AI)
    delete AI;
  }

void TAnt::ResetPosition(uint cx,uint cy)
  {
  x = cx;
  y = cy;

  points = 0;
  moves = 0;

  neighborhood = map.ExtractNeighborhood(x,y,visualRange,GAME_BOUNDARY_PENALTY);

  lastmove = neighborhood;
  lastmove.SetChoice(DIR_NONE);
  }

void TAnt::SetAI(TAntAI * ai)
  {
  if (AI)
    delete AI;
  AI = ai;
  }

void TAnt::DoMoveAuto()
  {
  DoMove(Predict());
  }

void TAnt::DoMove(TDirection dir)
  {
  if (dir < 0 || dir >= MAX_DIR)
    return; // invalid

  sint diffx = DIRECTION_X_MOD[dir];
  sint diffy = DIRECTION_Y_MOD[dir];

  moves++;

  lastmove = neighborhood;
  lastmove.SetChoice(dir);

  sint newx = x + diffx;
  sint newy = y + diffy;

  if (newx < 0 || newx >= sint(map.GetWidth()) ||
    newy < 0 || newy >= sint(map.GetHeight()))
    {
    points += GAME_BOUNDARY_PENALTY;
    return; // hit the edge of the map
    }

  points += map[newy][newx];

  if (map[newy][newx] > 0)
    map[newy][newx] = 0; // eat all you can
    else
      map[newy][newx]--; // do not return on previous steps

  x = newx;
  y = newy;
  neighborhood = map.ExtractNeighborhood(x,y,visualRange,GAME_BOUNDARY_PENALTY);

  AntMoved(lastmove);
  }

TDirection TAnt::Predict() const
  {
  if (AI)
    return AI->Evaluate(neighborhood);

  return DIR_NORTH;
  }
