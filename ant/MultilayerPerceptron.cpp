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

#include "MultilayerPerceptron.h"

#include <floatfann.h>

#include <fstream>
#include <sstream>

#include <cstdarg>

#include <iostream>

// this class hides internal dependency on the FANN library
class TMultilayerPerceptron::TNetClass
  {
  public:
  fann * net;
  std::string filename;

  TUintVector layers_size;
  TRealUintMapUintMap connections;

  uint GetOutputCount() {return layers_size.back(); }
  uint GetInputCount() {return layers_size.front(); }

  TNetClass(std::string fname,const TUintVector & lsize,const TRealUintMapUintMap & conn)
    {
    connections = conn;
    filename = fname;
    layers_size = lsize;

    // === CREATE the NET!! ==
      {
      uint size = layers_size.size();
      unsigned int * layers = new unsigned int[size];
      for (uint i = 0; i < size; i++)
        layers[i] = layers_size[i];

      net = fann_create_standard_array(size,layers);
      delete [] layers;
      }

    // for compatibility with WEKA
    fann_set_activation_function_hidden(net, FANN_SIGMOID);
    fann_set_activation_function_output(net, FANN_SIGMOID);

    fann_set_activation_steepness_output(net,0.5);
    fann_set_activation_steepness_hidden(net,0.5);

    // init the weights
    for (TRealUintMapUintMap::iterator n = connections.begin(); n != connections.end(); n++)
      {
      const TRealUintMap::iterator begin = n->second.begin();
      const TRealUintMap::iterator end = n->second.end();
      for (TRealUintMap::iterator c = begin; c != end; c++)
        {
        fann_set_weight(net,c->first,n->first,c->second);
        }
      }
    }

  virtual ~TNetClass()
    {
    fann_destroy(net);
    }

  std::string GetStringDescription() const
    {
    std::ostringstream stream;

    stream << "Loaded file: " << filename << "\n";

    stream << "Net configuration: ";
    for (uint i = 0; i < layers_size.size(); i++)
      stream << layers_size[i] << " ";
    stream << "\n";

    for (TRealUintMapUintMap::const_iterator n = connections.begin(); n != connections.end(); n++)
      {
      stream << "Node FANN id: " << n->first << "\n";
      const TRealUintMap::const_iterator begin = n->second.begin();
      const TRealUintMap::const_iterator end = n->second.end();
      for (TRealUintMap::const_iterator c = begin; c != end; c++)
        stream << "  Connection from " << c->first << " with weight " << c->second << "\n";
      }

    return stream.str();
    }

  private:
  // disable default constructors
  TNetClass() {}
  TNetClass(const TNetClass & /*other*/) {}
  };

TMultilayerPerceptron::TMultilayerPerceptron()
  {
  m_net = NULL;
  }

void TMultilayerPerceptron::Clear()
  {
  if (m_net)
    {
    delete m_net;
    m_net = NULL;
    }
  }

TMultilayerPerceptron::~TMultilayerPerceptron()
  {
  Clear();
  }

// builds a vector for each layer containing the progressive ids of the node
// to be used in the FANN library
// at result[layer][0] is always the id of the "Always 1" node
// nnumber contains the number of neurons for each layer
// at least two layers are required: input and output
TMultilayerPerceptron::TUintVectorVector TMultilayerPerceptron::BuildFANNNeuronIds(const TUintVector& nnumbers)
  {
  TUintVectorVector result;
  uint nlayers = nnumbers.size();
  result.resize(nlayers);
  for (uint i = 0; i < nlayers; i++)
    result[i].resize(nnumbers[i] + 1);

  uint idcounter = 0; // auto-increment for node ids

  for (uint i = 0; i < nlayers; i++)
    {
    TUintVector & layer = result[i];
    uint layer_size = nnumbers[i];

    for (uint i = 0; i < layer_size; i++)
      layer[i + 1] = idcounter++; // real nodes
    layer[0] = idcounter++;       // always 1
    }

  return result;
  }

// builds a vector for each layer containing the ids of the nodes
// as assigned by the WEKA library
TMultilayerPerceptron::TUintVectorVector TMultilayerPerceptron::BuildWEKANeuronIds(const TUintVector &nnumbers)
  {
  TUintVectorVector result;
  uint nlayers = nnumbers.size();
  result.resize(nlayers);
  for (uint i = 0; i < nlayers; i++)
    result[i].resize(nnumbers[i]);

  uint idcounter = 0; // auto-increment for node ids

  // WEKA allocates the output nodes first
  for (uint i = 0; i < nnumbers[nlayers - 1]; i++)
    result[nlayers - 1][i] = idcounter++;

  // then, WEKA allocates hidden nodes
  for (uint i = 1; i < nlayers - 1; i++)
    {
    TUintVector & layer = result[i];
    uint layer_size = nnumbers[i];

    for (uint i = 0; i < layer_size; i++)
      layer[i] = idcounter++; // real nodes
    }

  // input nodes do not have an id
  result[0].clear();

  return result;
  }

TMultilayerPerceptron::TLayerAndPositionIds
  TMultilayerPerceptron::FindLayerAndPosition(const TUintVectorVector & vec, uint id)
  {
  uint size = vec.size();
  uint p;

  for (uint i = 0; i < size; i++)
    if ((p = FindPosition(vec[i],id)) < vec[i].size())
      return TLayerAndPositionIds(i,p);

  return TLayerAndPositionIds();
  }

TMultilayerPerceptron::uint TMultilayerPerceptron::FindPosition(const TUintVector & vec,uint id)
  {
  uint size = vec.size();
  for (uint i = 0; i < size; i++)
    if (vec[i] == id)
      return i;

  return size;
  }

TMultilayerPerceptron::uint TMultilayerPerceptron::WEKAToFANNId(const TUintVectorVector &fann_ids,
  const TUintVectorVector &weka_ids,uint weka_id)
  {
  TLayerAndPositionIds ptr = FindLayerAndPosition(weka_ids,weka_id);
  if (!ptr.valid)
    return 0;

  return fann_ids[ptr.layer][ptr.pos + 1];
  }

bool TMultilayerPerceptron::ReadHeader(std::ifstream & ifile,TUintVector & layers_size)
  {
  std::string line;
  std::istringstream linestream;

  uint hidden_layers_count;

  std::getline(ifile,line);
  linestream.str(line);

  std::string tempstring;
  linestream >> tempstring;

  if (tempstring != "Hidden")
    return false;

  if (!(linestream >> hidden_layers_count))
    return false;

  std::getline(ifile,line);
  linestream.clear();
  linestream.str(line);

  linestream >> tempstring;
  if (tempstring != "Nodes")
    return false;

  layers_size.resize(hidden_layers_count + 2);
  if (!(linestream >> layers_size[0]))
    return false; // read input count

  for (uint i = 0; i < hidden_layers_count; i++)
    if (!(linestream >> layers_size[i + 1]))
      return false; // read hidden nodes count

  if (!(linestream >> layers_size[hidden_layers_count + 1]))
    return false; // read output count

  return true;
  }

bool TMultilayerPerceptron::ReadNeuron(std::ifstream & ifile,const std::string firstline,
  const TUintVectorVector & fann_ids,const TUintVectorVector & weka_ids,
  uint & fann_id,TRealUintMap &connections,std::string & lastline)
  {
  connections.clear();
  std::string line;

  std::istringstream firstlinestream;
  firstlinestream.str(firstline);

  std::string nodetype,node;
  uint weka_id;
  if (!(firstlinestream >> nodetype >> node >> weka_id))
    return false;
  if (node != "Node")
    return false;

  fann_id = WEKAToFANNId(fann_ids,weka_ids,weka_id);
  uint layer = FindLayerAndPosition(weka_ids,weka_id).layer;

  std::getline(ifile,line); // discard "Input Weights"

  uint input_auto_increment = 0;

  while (std::getline(ifile,line))
    {
    std::istringstream linestream;
    linestream.str(line);

    if (line.empty())
      continue;
    if (line[0] != ' ')
      {
      lastline = line;
      return true; // node ended
      }

    std::string conntype;
    linestream >> conntype;
    if (conntype == "Node")
      {
      uint id;
      Real w;
      if (!(linestream >> id >> w))
        return false;

      id = WEKAToFANNId(fann_ids,weka_ids,id);

      connections.insert(std::make_pair(id,w));
      }
    else if (conntype == "Attrib")
      {
      std::string name;
      Real w;
      if (!(linestream >> name >> w))
        return false;
      connections.insert(std::make_pair(input_auto_increment++,w));
      }
    else if (conntype == "Threshold")
      {
      Real w;
      if (!(linestream >> w))
        return false;
      connections.insert(std::make_pair(fann_ids[layer - 1][0],w));
      }
    else
      return false;
    }

  return true;
  }

bool TMultilayerPerceptron::Load(std::string filename)
  {
  Clear();

  std::ifstream ifile(filename.c_str());
  if (!ifile)
    return false;

  TUintVector layers_size;

  if (!ReadHeader(ifile,layers_size))
    return false;

  TUintVectorVector weka_node_ids = BuildWEKANeuronIds(layers_size);
  TUintVectorVector fann_node_ids = BuildFANNNeuronIds(layers_size);
  uint node_count = 0;
  for (uint i = 1; i < layers_size.size(); i++)
    node_count += layers_size[i];

  std::string firststring;
  if (!std::getline(ifile,firststring))
    return false;

  TRealUintMapUintMap connections;
  uint counter = 0;

  uint fann_id;
  TRealUintMap tempmap;
  while (ReadNeuron(ifile,firststring,fann_node_ids,weka_node_ids,fann_id,tempmap,firststring))
    {
    connections.insert(std::make_pair(fann_id,tempmap));
    tempmap.clear();
    counter++;
    }

  if (counter != node_count)
    return false;

  m_net = new TNetClass(filename,layers_size,connections);

  return true;
  }


TMultilayerPerceptron::TRealVector TMultilayerPerceptron::Evaluate(TRealVector input) const
  {
  TRealVector result;

  if (!m_net)
    return result;

  if (input.size() != m_net->GetInputCount())
    return result;

  result.resize(m_net->GetOutputCount());

  fann_type * invec = new fann_type[m_net->GetInputCount()];
  for (uint i = 0; i < m_net->GetInputCount(); i++)
    invec[i] = input[i];

  fann_type * outvec = fann_run(m_net->net,invec);

  for (uint i = 0; i < m_net->GetOutputCount(); i++)
    result[i] = outvec[i];

  delete [] invec;

  return result;
  }

std::string TMultilayerPerceptron::GetDescription() const
  {
  if (!m_net)
    return "Net not loaded!\n";

  return m_net->GetStringDescription();
  }

TMultilayerPerceptron::uint TMultilayerPerceptron::GetInputCount() const
  {
  return m_net ? m_net->GetInputCount() : 0;
  }

TMultilayerPerceptron::uint TMultilayerPerceptron::GetOutputCount() const
  {
  return m_net ? m_net->GetOutputCount() : 0;
  }
