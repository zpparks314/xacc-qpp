add_xacc_test(QPPVisitor)
target_link_libraries(QPPVisitorTester xacc-quantum-gate)
add_xacc_test(QPPAccelerator)
target_link_libraries(QPPAcceleratorTester xacc-quantum-gate)
