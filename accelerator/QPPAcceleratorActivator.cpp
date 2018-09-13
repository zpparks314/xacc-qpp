#include "cppmicroservices/BundleActivator.h"
#include "cppmicroservices/BundleContext.h"
#include "cppmicroservices/ServiceProperties.h"
#include "XACC.hpp"
#include "QPPAccelerator.hpp"

using namespace cppmicroservices;

class US_ABI_LOCAL QPPAcceleratorActivator : public BundleActivator {

public:
        QPPAcceleratorActivator() {}
        void Start(BundleContext context) {
          auto acc = std::make_shared<xacc::quantum::QPPAccelerator>();
          auto vis = std::make_shared<xacc::quantum::QPPVisitor>();

          context.RegisterService<xacc::OptionsProvider>(acc);
          context.RegisterService<xacc::Accelerator>(acc);
          context.RegisterService<xacc::BaseInstructionVisitor>(vis);
        }
        
        void Stop(BundleContext context) {}
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(QPPAcceleratorActivator)
