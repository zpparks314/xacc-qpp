#include "QPPVisitor.hpp"
#include "Hadamard.hpp"
#include "GateFunction.hpp"
#include <gtest/gtest.h>

using namespace qpp;

using namespace xacc::quantum;

TEST(QPPVisitorTester, checkHadamardGate) {

  auto f = std::make_shared<GateFunction>("foo");


}

int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
