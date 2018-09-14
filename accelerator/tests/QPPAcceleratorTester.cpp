#include "QPPAccelerator.hpp"
#include "XACC.hpp"
#include <gtest/gtest.h>


using namespace xacc::quantum;

TEST(QPPAcceleratorTester, simpleCheck) {

  auto acc = std::make_shared<QPPAccelerator>();
  std::shared_ptr<AcceleratorBuffer> buffer = acc->createBuffer("qreg", 3);

  auto f = std::make_shared<GateFunction>("foo");
  auto h = std::make_shared<Hadamard>(0);
  f->addInstruction(h);

  acc->execute(buffer, f);
}

TEST(QPPAcceleratorTester, checkGate) {

  auto acc = std::make_shared<QPPAccelerator>();
  std::shared_ptr<AcceleratorBuffer> buffer = acc->createBuffer("qreg", 2);

  auto f = std::make_shared<GateFunction>("foo");
  auto x = std::make_shared<X>(0);
  double theta = -0.0887738;
  auto ry = std::make_shared<Ry>(1, theta);
  auto cn = std::make_shared<CNOT>(1, 0);
  auto measure = std::make_shared<Measure>(1, 0);

  f->addInstruction(x);
  f->addInstruction(ry);
  f->addInstruction(cn);
  f->addInstruction(measure);
  std::cout << "Ansatz:\n" << f->toString("qreg") << std::endl;

  acc->execute(buffer, f);

  // EXPECT_TRUE(buffer->getExpectationValueZ() == 1);
  std::cout << "Measured value: " << buffer->getExpectationValueZ() << std::endl;




}
int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
