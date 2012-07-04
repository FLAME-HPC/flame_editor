/*!
 * \file condition.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Implementation of model condition
 */
#include <QDebug>
#include "./condition.h"

Condition::Condition() {
    // Set variables using defaults
    parentCondition = 0;
    isNot = false;
    isTime = false;
    isValues = true;
    isConditions = false;
    lhsCondition = 0;
    rhsCondition = 0;
    lhsIsAgentVariable = false;
    rhsIsAgentVariable = false;
    lhsIsMessageVariable = false;
    rhsIsMessageVariable = false;
    lhsIsValue = true;
    rhsIsValue = true;
    timePeriod = "";
    timePhaseVariable = "";
    timeDuration = 0;
    lhs = "";
    op = "==";
    rhs = "";
    enabled = false;
    lhsDouble = 0.0;
    rhsDouble = 0.0;
}

Condition::~Condition() {
}

void Condition::processConditionSide(QString * hs, bool * isAgentVariable,
    bool * isMessageVariable, bool * isValue, double * hsDouble) {
    // Handle agent variables
    if (hs->startsWith("a.")) {
        *isAgentVariable = true;
        *isValue = false;
        hs->remove(0, 2);
    // Handle message variables
    } else if (hs->startsWith("m.")) {
        *isMessageVariable = true;
        *isValue = false;
        hs->remove(0, 2);
    // Handle values
    } else {
        *isValue = true;
        *hsDouble = hs->toDouble();
    }
}

void Condition::processSymbols() {
    // Handle LHS
    processConditionSide(&lhs, &lhsIsAgentVariable,
                         &lhsIsMessageVariable, &lhsIsValue, &lhsDouble);

    // Handle RHS
    processConditionSide(&rhs, &rhsIsAgentVariable,
                         &rhsIsMessageVariable, &rhsIsValue, &rhsDouble);

    // Handle time
    if (isTime) {
        if (timePhaseVariable.startsWith("a.")) {
            timePhaseVariable.remove(0, 2);
            timePhaseIsVariable = true;
        } else {
            timePhaseValue = timePhaseVariable.toInt();
            timePhaseIsVariable = false;
        }
    }

    // Handle operator
    if (op == "EQ") op = "==";
    else if (op == "NEQ") op = "!=";
    else if (op == "LEQ") op = "<=";
    else if (op == "GEQ") op = ">=";
    else if (op == "LT") op = "<";
    else if (op == "GT") op = ">";
}

void writeCondition(const Condition * c, QString * s) {
    // Check condition is not null
    Q_ASSERT(c != 0);

    // Handle not
    if (c->isNot) {
        s->append("not(");
    }
    // Handle values condition
    if (c->isValues) {
        if (c->lhsIsAgentVariable) {
            s->append("a.");
            s->append(c->lhs);
        } else if (c->lhsIsMessageVariable) {
            s->append("m.");
            s->append(c->lhs);
        } else if (c->lhsIsValue) {
            s->append(QString().setNum(c->lhsDouble));
        }
        s->append(" ");
        s->append(c->op);
        s->append(" ");
        if (c->rhsIsAgentVariable) {
            s->append("a.");
            s->append(c->rhs);
        } else if (c->rhsIsMessageVariable) {
            s->append("m.");
            s->append(c->rhs);
        } else if (c->rhsIsValue) {
            s->append(QString().setNum(c->rhsDouble));
        }
    }
    // Handle nested condition
    if (c->isConditions) {
        s->append("(");
        writeCondition(c->lhsCondition, s);
        s->append(") ");
        s->append(c->op);
        s->append(" (");
        writeCondition(c->rhsCondition, s);
        s->append(")");
    }
    // Handle time condition
    if (c->isTime) {
        s->append("time(");
        s->append(c->timePeriod);
        s->append(", ");
        if (c->timePhaseIsVariable) s->append(c->timePhaseVariable);
        else
            s->append(QString::number(c->timePhaseValue));
        if (c->timeDuration > 0) {
            s->append(", ");
            s->append(QString::number(c->timeDuration));
        }
        s->append(")");
    }
    // Handle not
    if (c->isNot) {
        s->append(")");
    }
}

QString Condition::toString() {
    QString s;
    if (enabled) writeCondition(this, &s);
    return s;
}

void Condition::writeConditionValueXML(QString * hsString,
    bool isAgentVariable, bool isMessageVariable, bool isValue,
        QString hs, double hsDouble) {
    // Handle agent variable
    if (isAgentVariable) {
        hsString->append("a.");
        hsString->append(hs);
    // Handle message variable
    } else if (isMessageVariable) {
        hsString->append("m.");
        hsString->append(hs);
    // Handle value
    } else if (isValue) {
        hsString->append(QString().setNum(hsDouble));
    }
}

void Condition::writeConditionValuesXML(const Condition * c,
                                        QXmlStreamWriter * stream) {
    // Default values to write out
    QString lhsString = "";
    QString rhsString = "";
    QString opString;

    // Format lhsString
    writeConditionValueXML(&lhsString, lhsIsAgentVariable,
                           lhsIsMessageVariable, lhsIsValue, lhs, lhsDouble);

    // Format operator
    if (c->op == "==") opString = "EQ";
    else if (c->op == "!=") opString = "NEQ";
    else if (c->op == "<=") opString = "LEQ";
    else if (c->op == ">=") opString = "GEQ";
    else if (c->op == "<") opString = "LT";
    else if (c->op == ">") opString = "GT";

    // Format rhsString
    writeConditionValueXML(&rhsString, rhsIsAgentVariable,
                           rhsIsMessageVariable, rhsIsValue, rhs, rhsDouble);

    // Write out values to xml stream
    stream->writeStartElement("lhs");
    stream->writeTextElement("value", lhsString);
    stream->writeEndElement();  // lhs
    stream->writeTextElement("op", opString);
    stream->writeStartElement("rhs");
    stream->writeTextElement("value", rhsString);
    stream->writeEndElement();  // rhs
}

void Condition::writeConditionTimeXML(const Condition * c,
        QXmlStreamWriter * stream) {
    stream->writeStartElement("time");
    stream->writeTextElement("period", c->timePeriod);
    if (c->timePhaseIsVariable) {
        QString s = "a.";
        s.append(c->timePhaseVariable);
        stream->writeTextElement("phase", s);
    } else {
        stream->writeTextElement("phase",
                QString::number(c->timePhaseValue));
    }
    if (c->timeDuration > 0)
        stream->writeTextElement("duration",
                QString::number(c->timeDuration));
    stream->writeEndElement();  // time
}

void Condition::writeConditionXML(const Condition * c,
        QXmlStreamWriter * stream) {
    // Check condition is not null
    Q_ASSERT(c != 0);

    // Handle not
    if (c->isNot) {
        stream->writeStartElement("not");
    }
    // Handle condition
    if (c->isValues) {
        writeConditionValuesXML(c, stream);
    } else if (c->isConditions) {
        stream->writeStartElement("lhs");
        writeConditionXML(c->lhsCondition, stream);
        stream->writeEndElement();  // lhs
        stream->writeTextElement("op", c->op);
        stream->writeStartElement("rhs");
        writeConditionXML(c->rhsCondition, stream);
        stream->writeEndElement();  // rhs
    } else if (c->isTime) {
        writeConditionTimeXML(c, stream);
    }
    // Handle closing of not tag
    if (c->isNot) {
        stream->writeEndElement();  // not
    }
}

void Condition::toXML(QXmlStreamWriter * stream) {
    if (enabled) writeConditionXML(this, stream);
}

void Condition::paint(QPainter *painter, const QRect &rect,
                       const QPalette &/*palette*/, EditMode /*mode*/) const {
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    if (enabled) {
       QString s;
       writeCondition(this, &s);

       painter->drawText(rect, s);
    }

    painter->restore();
}
