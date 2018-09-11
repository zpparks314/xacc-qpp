#include "QPPVisitor.hpp"
#include "Hadamard.hpp"
#include "GateFunction.hpp"
#include "AcceleratorBuffer.hpp"
#include "InstructionIterator.hpp"

#include <gtest/gtest.h>

using namespace qpp;

using namespace xacc::quantum;

TEST(QPPVisitorTester, simpleCheck) {

  auto buffer = std::make_shared<xacc::AcceleratorBuffer>("qreg", 1);

  auto f = std::make_shared<GateFunction>("foo");
  auto h = std::make_shared<Hadamard>(0);
  f->addInstruction(h);
  auto id = std::make_shared<Identity>(0);
  f->addInstruction(id);
  EXPECT_TRUE(f->nInstructions() == 2);
  auto visitor = std::make_shared<QPPVisitor>();
  visitor->initialize(buffer);
  xacc::InstructionIterator it(f);
  while (it.hasNext()) {
    auto nextInst = it.next();
    if (nextInst->isEnabled())
        nextInst->accept(visitor);
  }
}

TEST(QPPVisitorTester, checkMeasurement) {

  auto buffer = std::make_shared<xacc::AcceleratorBuffer>("qreg", 1);

  auto f = std::make_shared<GateFunction>("foo");
  auto x = std::make_shared<X>(0);
  auto meas = std::make_shared<Measure>(0, 1);
  f->addInstruction(x);
  f->addInstruction(meas);

  auto visitor = std::make_shared<QPPVisitor>();
  visitor->initialize(buffer);
  xacc::InstructionIterator it(f);
  while (it.hasNext()) {
    auto nextInst = it.next();
    if (nextInst->isEnabled())
        nextInst->accept(visitor);
  }
  std::string measured = visitor->getMeasurementString();
  std::cout << "Measured: " << measured << std::endl;
  EXPECT_TRUE(measured == "1");
}

TEST(QPPVisitorTester, checkCNOTGate) {

  auto buffer = std::make_shared<xacc::AcceleratorBuffer>("qreg", 2);

  auto f = std::make_shared<GateFunction>("foo");
  auto x = std::make_shared<X>(0);
  auto cn = std::make_shared<CNOT>(1, 0);
  f->addInstruction(x);
  f->addInstruction(cn);
  auto visitor = std::make_shared<QPPVisitor>();
  visitor->initialize(buffer);
  xacc::InstructionIterator it(f);
  while (it.hasNext()) {
    auto nextInst = it.next();
    if (nextInst->isEnabled())
        nextInst->accept(visitor);
  }

}

int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
