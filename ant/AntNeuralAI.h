#ifndef ANTNEURALAI_H
#define ANTNEURALAI_H

#include "AntAI.h"

#include "MultilayerPerceptron.h"

class TAntNeuralAI : public TAntAI
  {
  public:
  TAntNeuralAI(std::string fromfile);
  ~TAntNeuralAI();

  virtual TDirection Evaluate(TGrid status,uint randomseed);
  virtual std::string ToString();

  private:
  std::string filename;
  bool load_ok;

  TMultilayerPerceptron net;
  };

#endif // ANTNEURALAI_H
