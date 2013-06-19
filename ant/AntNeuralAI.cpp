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

#include "AntNeuralAI.h"

#include "GameDefines.h"

// if two result are closer than this, will choose randomly
#define SIMILARITY_LIMIT 0.2

TAntNeuralAI::TAntNeuralAI(std::string fromfile)
  {
  filename = fromfile;

  load_ok = net.Load(fromfile);
  load_ok = load_ok && (net.GetOutputCount() == MAX_DIR);
  load_ok = load_ok && (net.GetInputCount() ==
    (GAME_ANT_VISUAL_RANGE * 2 + 1) * (GAME_ANT_VISUAL_RANGE * 2 + 1));
  }

TAntNeuralAI::~TAntNeuralAI()
  {
  }

TDirection TAntNeuralAI::Evaluate(TGrid status,uint randomseed)
  {
  if (!load_ok)
    return DIR_NORTH; // error

  std::vector<float> input;
  uint height = status.GetHeight();
  uint width = status.GetWidth();

  for (uint h = 0; h < height; h++)
    for (uint w = 0; w < width; w++)
      input.push_back(status[h][w]);

  std::vector<float> output = net.Evaluate(input);

  if (output.size() != MAX_DIR)
    return DIR_NORTH; // error!!

  // find max
  uint maxi = 0;

  for (uint i = 1; i < MAX_DIR; i++)
    if (output[maxi] < output[i])
      maxi = i;

  float tot = 0.0;
  std::vector<float> diff(4,0.0);
  for (uint i = 0; i < MAX_DIR; i++)
    if (output[maxi] - output[i] < SIMILARITY_LIMIT)
      {
      diff[i] = SIMILARITY_LIMIT - (output[maxi] - output[i]);
      tot += diff[i];
      }

  std::vector<float> prob(4,0.0);
  for (uint i = 0; i < MAX_DIR; i++)
    prob[i] = diff[i] / tot;

  float random = float(randomseed % 100) / 99.0;
  for (uint i = 0; i < MAX_DIR; i++)
    {
    if (prob[i] >= random)
      return TDirection(i);

    random -= prob[i];
    }

  return TDirection(maxi);
  }

std::string TAntNeuralAI::ToString()
  {
  std::string result;
  result += std::string("TAntNeuralAI\n") + filename + std::string("\n");
  result += std::string("Load: ") + std::string(load_ok ? "OK" : "FAILED") + std::string("\n");

  if (load_ok)
    result += std::string("TMultilayerPerceptron:\n") + net.GetDescription();

  return result;
  }
