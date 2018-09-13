#ifndef QUANTUM_GATE_ACCELERATORS_QPP_QPPACCELERATOR_HPP
#define QUANTUM_GATE_ACCELERATORS_QPP_QPPACCELERATOR_HPP

#include "XACC.hpp"
#include "InstructionIterator.hpp"
#include "QPPVisitor.hpp"

using namespace xacc;
using namespace qpp;

namespace xacc { namespace quantum {

class QPPAccelerator : public Accelerator {

public:

  QPPAccelerator() {}

  virtual void initialize() override { }

  std::shared_ptr<options_description> getOptions() override {
    auto desc = std::make_shared<options_description>(
                    "QPP Accelerator Options");
    desc->add_options()("qpp-shots", value<std::string>()->default_value("1024"), "Provide the number of shots to execute.");
    return desc;
  }

  virtual bool handleOptions(variables_map& map) override { return false; }

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId) override;

  std::shared_ptr<AcceleratorBuffer> createBuffer(const std::string& varId, const int size) override;

  virtual bool isValidBufferSize(const int nbits) override;

  virtual void execute(std::shared_ptr<AcceleratorBuffer> buffer,
                const std::shared_ptr<Function> kernel) override;

  virtual std::vector<std::shared_ptr<AcceleratorBuffer>> execute(
                std::shared_ptr<AcceleratorBuffer> buffer,
                const std::vector<std::shared_ptr<Function>> functions) override;

  virtual AcceleratorType getType() override { return AcceleratorType::qpu_gate; }

  virtual const std::string name() const override {
    return "qpp";
  }

  virtual const std::string description() const override {
    return "The QPP Accelerator allows for interacting with the Quantum++ simulator library "
              "to launch XACC quantum kernels.";
  }

  virtual std::vector<std::shared_ptr<xacc::IRTransformation>> getIRTransformations() override {
    std::vector<std::shared_ptr<xacc::IRTransformation>> v;
    return v;
  }

  virtual ~QPPAccelerator() { }

};


}}




#endif
