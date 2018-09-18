#ifndef QUANTUM_GATE_ACCELERATORS_QPP_QPPVISITOR_HPP
#define QUANTUM_GATE_ACCELERATORS_QPP_QPPVISITOR_HPP

#include <iostream>

#include "qpp.h"
#include "AllGateVisitor.hpp"
#include "AcceleratorBuffer.hpp"

using namespace qpp;

namespace xacc { namespace quantum {

class QPPVisitor : public AllGateVisitor {

protected:

          ket qubitState;

          int nQubits, nStates, qpp_shots;

          std::string measurement, classicalAddresses;

          std::vector<int> measuredQubits;

          std::map<int, int> qubitToClassicalBitIndex;

          std::map<int, std::string> measurementMap;

          std::shared_ptr<AcceleratorBuffer> accBuffer;


public:

        QPPVisitor() {}

        double ipToDouble(xacc::InstructionParameter p) {
          if (p.which() == 0) {
            return (double)boost::get<int>(p);
          } else if (p.which() == 1) {
            return boost::get<double>(p);
          } else if (p.which() == 2) {
            return (double)boost::get<float>(p);
          } else {
            std::stringstream s;
            s << p;
            xacc::error("QPPVisitor: Invalid gate parameter " +
                  std::to_string(p.which()) + ", " + s.str());
                }
                return 0.0;
        }

        void initialize(std::shared_ptr<AcceleratorBuffer> buffer);

        void visit(Hadamard &h);

        void visit(Identity &i);

        void visit(CZ& cz);

        void visit(CNOT& cn);

        void visit(X &x);

        void visit(Y &y);

        void visit(Z& z);

        void visit(Measure& m);

        void visit(ConditionalFunction& c);

        void visit(Rx& rx);

        void visit(Ry& ry);

        void visit(Rz& rz);

        void visit(CPhase& cp);

        void visit(Swap& s);

        void visit(GateFunction& gf) { return; }

        void finalize() {
          for (auto const& kv : measurementMap){
            accBuffer->appendMeasurement(kv.second);
          }
        }

        qpp::ket getState() {
          return qubitState;
        }

        std::string getMeasurementString() { return measurement; }

        std::vector<int> getMeasuredQubits() { return measuredQubits; }

        virtual ~QPPVisitor() {}
};
}}
#endif
