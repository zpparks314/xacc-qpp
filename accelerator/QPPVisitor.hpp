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
          cmat CZ = gt.CZ;

          qubitState = apply(qubitState, CZ, qbits);

          std::cout << "CZ applied on:\n" << qbits << std::endl;
          std::cout << disp(qubitState) << std::endl;
        }

        void visit(CNOT& cn) {

        }

        void visit(X &x) {
          std::vector<idx> qbits;
          for (int i = 0; i < x.bits().size(); i++) {
            qbits.push_back(x.bits()[i]);
          }
          cmat nx = gt.X;

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

        }

        void visit(ConditionalFunction& c) {

        }

        void visit(Rx& rx) {

        }

        void visit(Ry& ry) {

        }

        void visit(Rz& rz) {

        }

        void visit(CPhase& cp) {

        }

        void visit(Swap& s) {

        }

        void visit(GateFunction& gf) {

        }

        qpp::ket getState() {
          return qubitState;
        }

        virtual ~QPPVisitor() {}
};
}}
#endif
