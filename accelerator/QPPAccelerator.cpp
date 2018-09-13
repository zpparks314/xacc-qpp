#include "QPPAccelerator.hpp"
#include "XACC.hpp"

namespace xacc { namespace quantum {


std::shared_ptr<AcceleratorBuffer> QPPAccelerator::createBuffer(const std::string& varId) { return createBuffer(varId, 100); }


std::shared_ptr<AcceleratorBuffer> QPPAccelerator::createBuffer(const std::string& varId, const int size) {
  if (!isValidBufferSize(size)) {
    xacc::error("QPP - Invalid buffer size.");
  }
  auto buffer = std::make_shared<AcceleratorBuffer>(varId, size);
  storeBuffer(varId, buffer);
  return buffer;
}

bool QPPAccelerator::isValidBufferSize(const int nBits) { return nBits <= 1000; }

std::vector<std::shared_ptr<AcceleratorBuffer>> QPPAccelerator::execute(
                                      std::shared_ptr<AcceleratorBuffer> buffer,
                                      const std::vector<std::shared_ptr<Function>> functions) {
        int counter = 0;
        std::vector<std::shared_ptr<AcceleratorBuffer>> tmpBuffers;
        for (auto f : functions) {
          auto tmpBuffer = createBuffer(buffer->name() + std::to_string(counter), buffer->size());
          execute(tmpBuffer, f);
          tmpBuffers.push_back(tmpBuffer);
          counter++;
        }
        return tmpBuffers;
}

void QPPAccelerator::execute(std::shared_ptr<AcceleratorBuffer> buffer, const std::shared_ptr<Function> kernel) {

        auto visitor = std::make_shared<QPPVisitor>();

        InstructionIterator it(kernel);
        while (it.hasNext()) {
              auto nextInst = it.next();
              if (nextInst->isEnabled())
                  nextInst->accept(visitor);
        }
}




}}
