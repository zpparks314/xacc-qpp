#include "QPPAccelerator.hpp"
#include "XACC.hpp"
#include <gtest/gtest.h>


using namespace xacc::quantum;

TEST(QPPAcceleratorTester, simpleCheck) {

  QPPAccelerator acc;

}






int main(int argc, char **argv) {
  xacc::Initialize();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  xacc::Finalize();
}
