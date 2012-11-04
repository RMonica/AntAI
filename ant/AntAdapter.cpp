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

#include "AntAdapter.h"

TQAntAdapter::TQAntAdapter(QObject *parent, uint x, uint y, uint range, TGrid &map):QObject(parent),TAnt(x,y,range,map)
  {

  }

void TQAntAdapter::AntMoved(TMove move)
  {
  for (uint r = ROTATION_NONE; r < MAX_ROTATION; r++)
    {
    TMove rotated = move.Rotate(TRotation(r));
    for (uint s = SYMMETRY_NONE; s < MAX_SYMMETRY; s++)
      {
      TMove reflected = rotated.Reflect(TSymmetry(s));

      std::string movestr = reflected.ToString();
      emit antMoved(QString(movestr.c_str()));
      }
    }
  }
