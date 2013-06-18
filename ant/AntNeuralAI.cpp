#include "AntNeuralAI.h"

#include "GameDefines.h"

#include <iostream>

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
