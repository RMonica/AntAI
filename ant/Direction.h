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

#ifndef DIRECTION_H
#define DIRECTION_H

#include "defines.h"

enum TDirection
  {
  DIR_NONE  = -1,
  DIR_NORTH =  0,
  DIR_WEST  =  1,
  DIR_SOUTH =  2,
  DIR_EAST  =  3,
  MAX_DIR
  };

// convert from direction to what must be added to x and y
// when moving into that direction
static const sint DIRECTION_X_MOD[4] = { 0,-1, 0, 1};
static const sint DIRECTION_Y_MOD[4] = {-1, 0, 1, 0};

static const char * const DIRECTION_NAME[4] =
  {
  "NORTH",
  "WEST",
  "SOUTH",
  "EAST",
  };

// counter-clockwise 90° rotations
enum TRotation
  {
  ROTATION_NONE  = 0,
  ROTATION_ONE   = 1,
  ROTATION_TWO   = 2,
  ROTATION_THREE = 3,
  MAX_ROTATION
  };

// get the right rotation to reverse a previous rotation
static const TRotation ROTATION_REVERSE[MAX_ROTATION] =
  {
  ROTATION_NONE,
  ROTATION_THREE,
  ROTATION_TWO,
  ROTATION_ONE,
  };

// simmetry axis
enum TSymmetry
  {
  SYMMETRY_NONE       = 0,
  SYMMETRY_VERTICAL   = 1,
  SYMMETRY_HORIZONTAL = 2,
  SYMMETRY_MAINDIAG   = 3,
  SYMMETRY_SECDIAG    = 4,
  MAX_SYMMETRY
  };

// get the right symmetry to reverse a previous symmetry
static const TSymmetry SYMMETRY_REVERSE[MAX_SYMMETRY] =
  {
  SYMMETRY_NONE,
  SYMMETRY_VERTICAL,
  SYMMETRY_HORIZONTAL,
  SYMMETRY_MAINDIAG,
  SYMMETRY_SECDIAG,
  };

// applies a symmetry to a direction
static const TDirection SYMMETRY_APPLY_DIRECTION[MAX_DIR][MAX_SYMMETRY] =
  {
  // NONE      VERTICAL  HORIZONTAL MAINDIAG  SECDIAG
    // direction 0: NORTH
    {DIR_NORTH,DIR_NORTH,DIR_SOUTH, DIR_WEST, DIR_EAST },
    // direction 1: WEST
    {DIR_WEST, DIR_EAST, DIR_WEST,  DIR_NORTH,DIR_SOUTH},
    // direction 2: SOUTH
    {DIR_SOUTH,DIR_SOUTH,DIR_NORTH, DIR_EAST, DIR_WEST },
    // direction 3: EAST
    {DIR_EAST, DIR_WEST ,DIR_EAST,  DIR_SOUTH,DIR_NORTH},
  };

#endif // DIRECTION_H
