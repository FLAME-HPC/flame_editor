/*!
 * \file transition.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model function transition
 */
#include "./transition.h"

Transition::Transition() {
    myName = "";
    myCurrentState = 0;
    myNextState = 0;
}

Transition::Transition(State * cs, QString n, State * ns) {
    myCurrentState = cs;
    myMpre = Mpre();
    myName = n;
    myMpost = Mpost();
    myNextState = ns;
}

Transition::Transition(State * cs, Communication i, Mpre pre, QString n,
        Mpost post, Communication o, State * ns) {
    myCurrentState = cs;
    myInput = i;
    myMpre = pre;
    myName = n;
    myMpost = post;
    myOutput = o;
    myNextState = ns;
}

bool Transition::passesCondition(MemoryModel* memory) {
    bool rc = false;
    double d;

    if (myMpre.enabled() == false) return true;

    /* Find variable in memory */
    for (int i = 0; i < memory->getNames().size(); i++) {
        if (memory->getNames().at(i) == myMpre.name()) {
            // if(memory->variables[i].type == "int")
            // d = memory->getintValues().at(i);
            // else if(memory->variables[i].type == "double")
            // d = memory->getdoubleValues().at(i);
            d = memory->variables[i].getValue();
            if (myMpre.op() == "==") rc = (d == myMpre.value());
            if (myMpre.op() == "!=") rc = (d != myMpre.value());
            if (myMpre.op() == ">") rc = (d > myMpre.value());
            if (myMpre.op() == "<") rc = (d < myMpre.value());
            if (myMpre.op() == ">=") rc = (d >= myMpre.value());
            if (myMpre.op() == "<=") rc = (d <= myMpre.value());

            if (myMpre.isNot()) return !rc;
            else
                return rc;
        }
    }

    return false;
}

void Transition::updateMemory(MemoryModel* memory) {
    if (myMpost.enabled() == false) return;

    QList<statement> statements = myMpost.getStatements();
    // Loop though Mpost statements
    for (int i = 0; i < statements.size(); i ++) {
        // If the statement is valid
        if (statements.at(i).isValid) {
            double d1 = 0.0, d2 = 0.0, d3 = 0.0;
            int n = 0;
            // Find variable in memory and set values
            for (int j = 0; j < memory->getNames().size(); j++) {
                if (statements.at(i).lhs == memory->variables[j].name) {
                    d1 = memory->variables[j].getValue();
                    n = j;
                }
                if (statements.at(i).isVariable)
                    if (statements.at(i).rhs == memory->variables[j].name)
                        d2 = memory->variables[j].getValue();
            }
            // if third name is number then set number from value
            if (statements.at(i).isVariable == false)
                d2 = statements.at(i).drhs;
            if (statements.at(i).op == "=") d3 = d2;
            else if (statements.at(i).op == "+=") d3 = d1+d2;
            else if (statements.at(i).op == "-=") d3 = d1-d2;
            else if (statements.at(i).op == "*=") d3 = d1*d2;
            else if (statements.at(i).op == "/=") d3 = d1/d2;
            // else qDebug() << "Operator not found in mpost";
            // Update memory value
            memory->replaceValue(n, d3);
        }
    }
}
