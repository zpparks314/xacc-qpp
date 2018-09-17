#include "QPPAccelerator.hpp"
#include "XACC.hpp"
#include <gtest/gtest.h>


using namespace xacc::quantum;

TEST(QPPAcceleratorTester, simpleCheck) {

  auto acc = std::make_shared<QPPAccelerator>();
  std::shared_ptr<AcceleratorBuffer> buffer = acc->createBuffer("qreg", 1);

  auto f = std::make_shared<GateFunction>("foo");
  auto h = std::make_shared<Hadamard>(0);
  auto measure = std::make_shared<Measure>(0, 0);

  f->addInstruction(h);
  f->addInstruction(measure);

  acc->execute(buffer, f);
  std::cout << "Measured Expectation Value: " << buffer->getExpectationValueZ() << std::endl;

  auto map = buffer->getMeasurementCounts();
  for (auto& kv : map) {
    std::cout << kv.first << ":" << kv.second << "\n";
  }
}

TEST(QPPAcceleratorTester, checkGate) {
  auto acc = std::make_shared<QPPAccelerator>();
  std::shared_ptr<AcceleratorBuffer> buffer = acc->createBuffer("qreg", 2);

  auto f = std::make_shared<GateFunction>("foo");
  auto x = std::make_shared<X>(0);
  double theta = 0.07321341;
  auto ry = std::make_shared<Ry>(1, theta);
  auto cn = std::make_shared<CNOT>(1, 0);
  auto measure = std::make_shared<Measure>(1, 0);

  f->addInstruction(x);
  f->addInstruction(ry);
  f->addInstruction(cn);
  f->addInstruction(measure);
  std::cout << "Ansatz:\n" << f->toString("qreg") << std::endl;

  acc->execute(buffer, f);

  std::cout << "Measured Expectation Value: " << buffer->getExpectationValueZ() << std::endl;
  auto map = buffer->getMeasurementCounts();
  for (auto& kv : map) {
    std::cout << kv.first << ":" << kv.second << "\n";
  }
}

TEST(QPPAcceleratorTester, littleTest) {
  auto acc = std::make_shared<QPPAccelerator>();
  std::shared_ptr<AcceleratorBuffer> buffer = acc->createBuffer("qreg", 2);

  auto f = std::make_shared<GateFunction>("foo");
  auto h = std::make_shared<Hadamard>(0);
  auto h2 = std::make_shared<Hadamard>(1);
  auto meas = std::make_shared<Measure>(0, 0);
  auto meas2 = std::make_shared<Measure>(1, 1);

  f->addInstruction(h);
  f->addInstruction(h2);
  f->addInstruction(meas);
  f->addInstruction(meas2);

  acc->execute(buffer, f);

  std::cout << "Measure expectation values: " << buffer->getExpectationValueZ() << std::endl;
  auto map = buffer->getMeasurementCounts();
  for (auto& kv : map) {
    std::cout << kv.first << ":" << kv.second << "\n";
  }
}

int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
