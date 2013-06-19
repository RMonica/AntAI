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

#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <string>
#include <vector>
#include <map>

class TMultilayerPerceptron
  {
  public:
  TMultilayerPerceptron();
  ~TMultilayerPerceptron();
  void Clear();

  bool Load(std::string filename);

  typedef float Real;
  typedef std::vector<Real> TRealVector;
  typedef unsigned int uint;
  typedef std::vector<uint> TUintVector;
  typedef std::vector<TUintVector> TUintVectorVector;
  typedef std::map<uint,Real> TRealUintMap;
  typedef std::map<uint,TRealUintMap> TRealUintMapUintMap;

  uint GetInputCount() const;
  uint GetOutputCount() const;

  TRealVector Evaluate(TRealVector input) const;

  std::string GetDescription() const;

  private:
  static bool ReadHeader(std::ifstream & ifile,
    TUintVector &layers_size);

  static bool ReadNeuron(std::ifstream & ifile,const std::string firstline,
    const TUintVectorVector &fann_ids, const TUintVectorVector &weka_ids,
    uint &fann_id, TRealUintMap &connections, std::string &lastline);

  static TUintVectorVector BuildFANNNeuronIds(const TUintVector &nnumbers);
  static TUintVectorVector BuildWEKANeuronIds(const TUintVector &nnumbers);

  struct TLayerAndPositionIds
    {
    public:
    uint layer;
    uint pos;

    bool valid;

    TLayerAndPositionIds(uint l,uint p)
      {
      layer = l;
      pos = p;
      valid = true;
      }

    TLayerAndPositionIds()
      {
      valid = false;
      layer = 0;
      pos = 0;
      }
    };

  static TLayerAndPositionIds FindLayerAndPosition(const TUintVectorVector & vec, uint id);
  static uint FindPosition(const TUintVector & vec, uint id);
  static uint WEKAToFANNId(const TUintVectorVector &fann_ids,const TUintVectorVector &weka_ids,uint weka_id);

  class TNetClass;
  TNetClass * m_net;
  };

#endif // MULTILAYERPERCEPTRON_H
