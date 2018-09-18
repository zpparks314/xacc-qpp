add_xacc_test(QPPVisitor)
target_link_libraries(QPPVisitorTester xacc-quantum-gate xacc-qpp-accelerator)
add_xacc_test(QPPAccelerator)
target_link_libraries(QPPAcceleratorTester xacc-quantum-gate xacc-qpp-accelerator)
