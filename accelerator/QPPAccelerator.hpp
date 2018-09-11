#ifndef QUANTUM_GATE_ACCELERATORS_QPP_QPPACCELERATOR_HPP
#define QUANTUM_GATE_ACCELERATORS_QPP_QPPACCELERATOR_HPP

#include "XACC.hpp"
#include "InstructionIterator.hpp"
#include "QPPVisitor.hpp"

using namespace xacc;
using namespace qpp;

namespace xacc { namespace quantum {

class QPPAccelerator : public Accelerator {


protected:



public:

  QPPAccelerator() {}

  virtual void initialize() { }

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId);

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId, const int size);

  virtual bool isValidBufferSize(const int nbits);

  virtual void execute(std::shared_ptr<AcceleratorBuffer> buffer, const std::shared_ptr<Function> kernel);

  virtual std::vector<std::shared_ptr<AcceleratorBuffer>> execute(std::shared_ptr<AcceleratorBuffer> buffer, const std::vector<std::shared_ptr<Function>> functions);








};


}}




#endif
