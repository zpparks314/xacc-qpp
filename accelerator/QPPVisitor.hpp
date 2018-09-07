#ifndef QUANTUM_GATE_ACCELERATORS_QPP_QPPVISITOR_HPP
#define QUANTUM_GATE_ACCELERATORS_QPP_QPPVISITOR_HPP

#include <iostream>

#include "qpp.h"
#include "AllGateVisitor.hpp"


using namespace qpp;

namespace xacc { namespace quantum {

class QPPVisitor: public AllGateVisitor {


protected:

        std::shared_ptr<AcceleratorBuffer> buffer;

        ket qbitState;


public:

        virtual void initialize(std::shared_ptr<AcceleratorBuffer> buffer) {
          qbitState = ket::Zero(buffer.size());
        }


        void visit(Hadamard& h) {
          int bit = h.bits()[0];


        }

        void visit(Identity& i) {

        }

        void visit(CZ& cz) {

        }

        void visit(CNOT& cn) {

        }

        void visit(X& x) {

        }

        void visit(Y& y) {

        }

        void visit(Z& z) {

        }



}
}}
