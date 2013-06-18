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
