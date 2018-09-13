#include "QPPVisitor.hpp"
#include "Hadamard.hpp"
#include "GateFunction.hpp"
#include "AcceleratorBuffer.hpp"
#include "InstructionIterator.hpp"
#include "QFT.hpp"
#include "InverseQFT.hpp"
#include "GateIR.hpp"
#include "QPPAccelerator.hpp"

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
  auto x = std::make_shared<Hadamard>(0);
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
  // EXPECT_TRUE(measured == "1");
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

TEST(QPPVisitorTester, CheckQFTandIQFT) {
  auto qft = std::make_shared<QFT>();
  auto iqft = std::make_shared<InverseQFT>();

  auto buffer = std::make_shared<xacc::AcceleratorBuffer>("qreg", 3);
  auto visitor = std::make_shared<QPPVisitor>();
  visitor->initialize(buffer);

  auto initialState = visitor->getState();
  std::cout << "Initial State: \n" << disp(initialState) << std::endl;
  auto qftKernel = qft->generate(buffer);
  auto ir = std::make_shared<GateIR>();
  ir->addKernel(qftKernel);

  xacc::InstructionIterator it(ir->getKernel("qft"));
  while (it.hasNext()) {
    auto nextInst = it.next();
    if (nextInst->isEnabled())
        nextInst->accept(visitor);
  }
  auto qftState = visitor->getState();
  std::cout << "QFT Performed, Final State: \n" << disp(qftState) << std::endl;

  auto iqftKernel = iqft->generate(buffer);
  auto ir2 = std::make_shared<GateIR>();
  ir2->addKernel(iqftKernel);

  xacc::InstructionIterator it2(ir2->getKernel("inverse_qft"));
  while (it2.hasNext()) {
    auto nextInst = it2.next();
    if (nextInst->isEnabled())
        nextInst->accept(visitor);
  }
  auto iqftState = visitor->getState();
  std::cout << "InverseQFT performed, Final State: \n" << disp(iqftState) << std::endl;


}

int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
