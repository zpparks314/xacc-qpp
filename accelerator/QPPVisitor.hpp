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

          int nQubits, nStates;

          std::string measurement, classicalAddresses;

          std::vector<int> measuredQubits;

          std::map<int, int> qubitToClassicalBitIndex;

public:

        QPPVisitor() {}

        void initialize(std::shared_ptr<AcceleratorBuffer> buffer) {
          nQubits = buffer->size();
          qubitState = st.z0;
          for (int i = 0; i < nQubits - 1; i++) {
            // Generate qubitState ket
            qubitState = kron(qubitState, st.z0);
          }
          // checking
          std::cout << "qubitState: \n" << disp(qubitState) << std::endl;
        }

        void visit(Hadamard &h) {
          // not sure if these bit loops are necessary
          // but I'm gonna keep them for sake of consistency
          std::vector<idx> qbits;
          for (int i = 0; i < h.bits().size(); i++){
            qbits.push_back(h.bits()[i]);
          }
          // translate the XACC IR to QPP type
          cmat had = gt.H;
          // apply the gate to the corresponding qubit on the state vector
          qubitState = apply(qubitState, had, qbits);
          // checking..
          std::cout << "Hadamard applied on:\n" << qbits[0] << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Identity &i) {
          std::vector<idx> qbits;
          for (int j = 0; j < i.bits().size(); j++){
            qbits.push_back(i.bits()[j]);
          }
          // translate
          cmat id = gt.Id();
          // apply the gate
          qubitState = apply(qubitState, id, qbits);
          // checking
          std::cout << "Identity applied on:\n" << qbits[0] << std::endl;
          std::cout << disp(qubitState) << std::endl;
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
          std::cout << "CZ applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
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

          std::cout << "CNOT applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;

        }

        void visit(X &x) {
          std::vector<idx> qbits;
          for (int i = 0; i < x.bits().size(); i++) {
            qbits.push_back(x.bits()[i]);
          }
          // translate
          cmat nx = gt.X;
          // apply that bad boy
          qubitState = apply(qubitState, nx, qbits);

          std::cout << "X applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Y &y) {
          std::vector<idx> qbits;
          for (int i = 0; i < y.bits().size(); i++) {
            qbits.push_back(y.bits()[i]);
          }
          cmat ny = gt.Y;

          qubitState = apply(qubitState, ny, qbits);

          std::cout << "Y applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Z& z) {
          std::vector<idx> qbits;
          for (int i = 0; i < z.bits().size(); i++) {
            qbits.push_back(z.bits()[i]);
          }
          cmat nz = gt.Z;

          qubitState = apply(qubitState, nz, qbits);

          std::cout << "Z applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Measure& m) {
          // not sure exactly if this is how measuements should be done
          // but we'll give it a go
          std::vector<idx> qbits;
          int classicalBitIdx = m.getClassicalBitIndex();
          qbits.push_back(m.bits()[0]);
          measuredQubits.push_back(m.bits()[0]);
          // for (int i = 0; i < m.bits().size(); i++) {
          //   qbits.push_back(m.bits()[i]);
          //   measuredQubits.push_back(m.bits()[i]);
          // }
          // just putting these because I think I might need them later
          classicalAddresses += std::to_string(classicalBitIdx);
          qubitToClassicalBitIndex.insert(std::make_pair(m.bits()[0], classicalBitIdx));
          auto result = measure_seq(qubitState, qbits);
          for (int i = 0; i < std::get<0>(result).size(); i++) {
            measurement += std::to_string(std::get<0>(result)[i]);
          }
          std::cout << "Measure applied on:\n" << qbits << "\nclassical: " << classicalBitIdx << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(ConditionalFunction& c) {
          // not sure what to do here
        }

        void visit(Rx& rx) {
          auto param = boost::get<double>(rx.getParameter(0));
          std::vector<idx> qbits;
          for (int i = 0; i < rx.bits().size(); i++) {
            qbits.push_back(rx.bits()[i]);
          }
          cmat nrx = gt.Rn(param, {1, 0, 0});

          qubitState = apply(qubitState, nrx, qbits);

          std::cout << "Rx applied on:\n" << qbits << "with parameter: " << param << std::endl;
          std::cout << disp(qubitState) << std::endl;

        }

        void visit(Ry& ry) {
          auto param = boost::get<double>(ry.getParameter(0));
          std::vector<idx> qbits;
          for (int i = 0; i < ry.bits().size(); i++) {
            qbits.push_back(ry.bits()[i]);
          }
          cmat nry = gt.Rn(param, {0, 1, 0});

          qubitState = apply(qubitState, nry, qbits);

          std::cout << "Ry applied on:\n" << qbits << "with parameter: " << param << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(Rz& rz) {
          auto param = boost::get<double>(rz.getParameter(0));
          std::vector<idx> qbits;
          for (int i = 0; i < rz.bits().size(); i++) {
            qbits.push_back(rz.bits()[i]);
          }
          cmat nrz = gt.Rn(param, {0, 0, 1});

          qubitState = apply(qubitState, nrz, qbits);

          std::cout << "Rz applied on:\n" << qbits << "with parameter: " << param << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(CPhase& cp) {
          // not sure what to use here - CZ is controlled-phase in QPP
        }

        void visit(Swap& s) {
          std::vector<idx> qbits;
          for (int i = 0; i < s.bits().size(); i++) {
            qbits.push_back(s.bits()[i]);
          }
          cmat ns = gt.SWAP;

          qubitState = apply(qubitState, ns, qbits);

          std::cout << "SWAP applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(GateFunction& gf) { return; }

        qpp::ket getState() {
          return qubitState;
        }

        std::string getMeasurementString() { return measurement; }

        std::vector<int> getMeasuredQubits() { return measuredQubits; }

        virtual ~QPPVisitor() {}
};
}}
#endif
