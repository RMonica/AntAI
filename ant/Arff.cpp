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

#include "Arff.h"

#include <sstream>

namespace NArff
  {
  //builds a valid Arff Header for WEKA given the ant
  std::string GenerateHeader(TAnt & ant)
    {
    std::stringstream result;
    result << "@RELATION Ant\n";

    uint range = ant.GetVisualRange();
    uint count = 2 * range + 1;

    // create cell attributes
    for (uint i = 0; i < count; i++)
      for (uint h = 0; h < count; h++)
        result << "@ATTRIBUTE cell" << i << "v" << h << "h" << " REAL\n";

    // create move attribute, enum all directions
    result << "@ATTRIBUTE move {";
    for (uint i = 0; i < MAX_DIR; i++)
      {
      if (i != 0)
        result << ",";
      result << DIRECTION_NAME[i];
      }

    result << "}\n\n@DATA\n";

    return result.str();
    }


  }
