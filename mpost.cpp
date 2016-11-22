/*!
 * \file mpost.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of mpost
 */
#include <QtGui>
#include "./mpost.h"

Mpost::Mpost(QString n, QString op, int v, bool enabled) {
    myName = n;
    myOp = op;
    myValue = v;
    myEnabled = enabled;
    text = "";
}

void Mpost::setText(QString t) {
    this->text = t;
    statements.clear();

    // QString t = "   var1 =   var2\nvar3    += 1\n\tvar3 *= 22   ";
    // qDebug() << "t: " << t;
    QStringList list1 = t.split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < list1.size(); ++i) {
        statement s1;
        s1.lhs = "";
        s1.op = "";
        s1.rhs = "";
        s1.isVariable = true;

        QString t1 = list1.at(i);
        t1 = t1.simplified();  // replaces all white space with spaces
        // qDebug() << "t1: " << t1;
        // split on the spaces removing any empty elements
        QStringList list2 = t1.split(" ", QString::SkipEmptyParts);
        qDebug() << "list2: " << list2;
        /* Make sure lhs is a memory var */

        // If list is not 3 member long then skip
        if (list2.size() < 3) {
            if (list2.size() > 0) s1.lhs = list2.at(0);
            if (list2.size() > 1) s1.op  = list2.at(1);
            qDebug() << "Error: Statement is less than 3 elements long";
        } else {
            // For all memory variables
            for (int j = 0; j < memory->getNames().size(); j++) {
                // If the first word == memory variable
                if (list2.at(0) == memory->getNames().at(j)) {
                    // Then make statement lhs = memory variable
                    s1.lhs = list2.at(0);
                }
                // If third word == memory variable
                if (list2.at(2) == memory->getNames().at(j)) {
                    // Then make statement rhs = memory variable
                    s1.rhs = list2.at(2);
                }
            }
            // Validate
            s1.isValid = true;  // Start with statement being valid
            // If the lhs is not set then error
            if (s1.lhs == "") {
                s1.lhs = list2.at(0);
                qDebug() << "Error: lhs doesn't equal a memory variable - "
                        << s1.lhs;
                s1.isValid = false;
            }
            s1.op = list2.at(1);  // Make statement op = second word
            // If the op is not an operator then error
            if (s1.op != "=" && s1.op != "+=" && s1.op != "-=" &&
               s1.op != "/=" && s1.op != "*=") {
                qDebug() << "Error: op doesn't equal one of =,+=,-=,/=,*= - "
                        << s1.op;
                s1.isValid = false;
            }
            // If the lhs is not set
            if (s1.rhs == "") {
                // Make statement rhs = text of value
                s1.rhs = list2.at(2);
                // Check if word is a number
                bool ok;
                // Make statement drhs = double value
                s1.drhs = list2.at(2).toDouble(&ok);
                // If string can convert to double
                if (ok) {
                    // Make statement isVariable = false
                    s1.isVariable = false;
                } else {
                    qDebug() <<
                        "Error: rhs is not a memory value or a valid number - "
                            << list2.at(2);
                    s1.isValid = false;
                }
            }
        }
        // Print out statement
        if (s1.isVariable) qDebug() << s1.lhs << " " << s1.op << " " << s1.rhs;
        else
            qDebug() << s1.lhs << " " << s1.op << " " << s1.drhs;
        if (s1.isValid) qDebug() << "Statement is valid";
        else
            qDebug() << "Statement is not valid";

        // Append statement to the list of statements
        statements.append(s1);
    }

    getText();
    this->myEnabled = true;
}

QString Mpost::getText() {
    QString s;
    for (int i = 0; i < statements.size(); i++) {
        s.append(statements.at(i).lhs);
        s.append(" ");
        s.append(statements.at(i).op);
        s.append(" ");
        s.append(statements.at(i).rhs);
        s.append("\n");
    }
    this->text = s;
    return s;
}

void Mpost::paint(QPainter *painter, const QRect &rect,
                        const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    if (myEnabled) {
        painter->drawText(rect, text);
    }

     painter->restore();
}

