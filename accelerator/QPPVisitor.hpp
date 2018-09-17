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

        void initialize(std::shared_ptr<AcceleratorBuffer> buffer) {
          try {
            qpp_shots = std::stoi(xacc::getOption("qpp-shots"));
            xacc::info("Using the QPPAccelerator with " + std::to_string(qpp_shots) + " measurement shots.");
          }
          catch(...) {
            std::cout << "QPPVisitor: Invalid number of shots to execute." << std::endl;
          }
          accBuffer = buffer;
          nQubits = accBuffer->size();
          for (int i = 0; i < qpp_shots; i++){
            std::string bitString = "";
            for (int j = 0; j < nQubits; j++){
              bitString += "0";
            }
            measurementMap[i] = bitString;
          }
          std::cout << "PRINT MAPPED STUFF: " << measurementMap[2] << std::endl;
          qubitState = st.z0;
          for (int i = 0; i < nQubits-1; i++) {
            // Generate qubitState ket
            qubitState = kron(qubitState, st.z0);
          }
          // checking
          // std::cout << "initial qubitState: \n" << disp(qubitState) << std::endl;
        }

        void visit(Hadamard &h) {
          std::vector<idx> qbits;
          qbits.push_back(h.bits()[0]);
          // translate the XACC IR to QPP type
          cmat had = gt.H;
          // apply the gate to the corresponding qubit on the state vector
          qubitState = apply(qubitState, had, qbits);
          // checking..
          // std::cout << "Hadamard applied on:" << qbits[0] << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(Identity &i) {
          std::vector<idx> qbits;
          qbits.push_back(i.bits()[0]);
          // translate
          cmat id = gt.Id();
          // apply the gate
          qubitState = apply(qubitState, id, qbits);
          // checking
          // std::cout << "Identity applied on:" << qbits[0] << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(CZ& cz) {
          std::vector<idx> qbits;
          for (int i = 0; i < cz.bits().size(); i++) {
            qbits.push_back(cz.bits()[i]);
          }
          // translate
          cmat CZ = gt.CZ;
          // apply the gate
          qubitState = apply(qubitState, CZ, qbits);
          // checking...
          // std::cout << "CZ applied on:" << qbits << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(CNOT& cn) {
          std::vector<idx> qbits;
          for (int i = 0; i < cn.bits().size(); i++) {
            qbits.push_back(cn.bits()[i]);
          }
          // translate
          cmat ncn = gt.CNOT;
          // apply the gate
          qubitState = apply(qubitState, ncn, qbits);

          std::cout << "CNOT applied on:" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;

        }

        void visit(X &x) {
          std::vector<idx> qbits;
          qbits.push_back(x.bits()[0]);
          // translate
          cmat nx = gt.X;
          // apply that bad boy
          qubitState = apply(qubitState, nx, qbits);

          std::cout << "X applied on:" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Y &y) {
          std::vector<idx> qbits;
          qbits.push_back(y.bits()[0]);

          cmat ny = gt.Y;

          qubitState = apply(qubitState, ny, qbits);

          // std::cout << "Y applied on:" << qbits << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(Z& z) {
          std::vector<idx> qbits;
          qbits.push_back(z.bits()[0]);
          cmat nz = gt.Z;

          qubitState = apply(qubitState, nz, qbits);

          // std::cout << "Z applied on:" << qbits << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(Measure& m) {
          // not sure exactly if this is how measuements should be done
          // but we'll give it a go
          std::vector<idx> qbits;
          int classicalBitIdx = m.getClassicalBitIndex();
          qbits.push_back(m.bits()[0]);
          measuredQubits.push_back(m.bits()[0]);
          // just putting these because I think I might need them later
          classicalAddresses += std::to_string(classicalBitIdx);
          qubitToClassicalBitIndex.insert(std::make_pair(m.bits()[0], classicalBitIdx));
          for (int i = 0; i < qpp_shots; i++) {
            auto result = measure(qubitState, gt.Z, qbits);
            int index = nQubits - 1 - m.bits()[0];
            // std::cout << "Result[0]: " << std::get<0>(result) << std::endl;
            // std::string bitString = measurementMap[i];
            // std::cout << "MeasurementMap[i]: " << measurementMap[i] << std::endl;
            measurementMap[i][index] = std::to_string(std::get<0>(result)).c_str()[0];
            // bitString[index] = (char)std::get<0>(result);
            // measurementMap[i] = bitString;
            // measurementMap[i].replace(index, 1, std::to_string(std::get<0>(result)));
            // std::cout << "MeasurementMap[i] (replaced): " << measurementMap[i] << std::endl;
          }
          // std::cout << "Measure applied on:" << qbits << "\nclassical: " << classicalBitIdx << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(ConditionalFunction& c) {
          // not sure what to do here yet
        }

        void visit(Rx& rx) {
          auto param = ipToDouble(rx.getParameter(0));
          std::vector<idx> qbits;
          qbits.push_back(rx.bits()[0]);

          cmat nrx = gt.Rn(param, {1, 0, 0});

          qubitState = apply(qubitState, nrx, qbits);

          // std::cout << "Rx applied on:" << qbits << "with parameter: " << param << std::endl;
          // std::cout << disp(qubitState) << std::endl;

        }

        void visit(Ry& ry) {
          auto param = ipToDouble(ry.getParameter(0));
          std::vector<idx> qbits;
          qbits.push_back(ry.bits()[0]);
          std::cout << "Rotation around qubit: " << qbits << std::endl;
          cmat nry = gt.Rn(param, {0, 1, 0});

          qubitState = apply(qubitState, nry, qbits);

          std::cout << "Ry applied on:" << qbits << "with parameter: " << param << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Rz& rz) {
          auto param = ipToDouble(rz.getParameter(0));
          std::vector<idx> qbits;
          qbits.push_back(rz.bits()[0]);
          cmat nrz = gt.Rn(param, {0, 0, 1});

          qubitState = apply(qubitState, nrz, qbits);

          // std::cout << "Rz applied on:" << qbits << "with parameter: " << param << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

        void visit(CPhase& cp) {
          auto param = ipToDouble(cp.getParameter(0));
          std::vector<idx> qbits;
          for (int i = 0; i < cp.bits().size(); i++) {
            qbits.push_back(cp.bits()[i]);
          }
          cmat ncp(2, 2);
          ncp << 1, 0, 0, exp(param);
          qubitState = applyCTRL(qubitState, ncp, {qbits[0]}, {qbits[1]});

          // std::cout << "CPhase applied on:" << qbits << "with parameter: " << param << std::endl;
          // std::cout << disp(qubitState) << std::endl;

        }

        void visit(Swap& s) {
          std::vector<idx> qbits;
          for (int i = 0; i < s.bits().size(); i++) {
            qbits.push_back(s.bits()[i]);
          }
          cmat ns = gt.SWAP;

          qubitState = apply(qubitState, ns, qbits);

          // std::cout << "SWAP applied on:" << qbits << std::endl;
          // std::cout << disp(qubitState) << std::endl;
        }

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
