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

void Condition::processSymbols() {
    if (lhs.startsWith("a.")) {
        lhsIsAgentVariable = true;
        lhsIsValue = false;
        lhs.remove(0, 2);
    } else if (lhs.startsWith("m.")) {
        lhsIsMessageVariable = true;
        lhsIsValue = false;
        lhs.remove(0, 2);
    } else {
        lhsIsValue = true;
        lhsDouble = lhs.toDouble();
    }

    if (rhs.startsWith("a.")) {
        rhsIsAgentVariable = true;
        rhsIsValue = false;
        rhs.remove(0, 2);
    } else if (rhs.startsWith("m.")) {
        rhsIsMessageVariable = true;
        rhsIsValue = false;
        rhs.remove(0, 2);
    } else {
        rhsIsValue = true;
        rhsDouble = rhs.toDouble();
    }

    if (isTime) {
        if (timePhaseVariable.startsWith("a.")) {
            timePhaseVariable.remove(0, 2);
            timePhaseIsVariable = true;
        } else {
            timePhaseValue = timePhaseVariable.toInt();
            timePhaseIsVariable = false;
        }
    }

    if (op == "EQ") op = "==";
    else if (op == "NEQ") op = "!=";
    else if (op == "LEQ") op = "<=";
    else if (op == "GEQ") op = ">=";
    else if (op == "LT") op = "<";
    else if (op == "GT") op = ">";
}

void writeCondition(const Condition * c, QString * s) {
    Q_ASSERT(c != 0);

    if (c->isNot) {
        s->append("not(");
    }
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
    if (c->isConditions) {
        s->append("(");
        writeCondition(c->lhsCondition, s);
        s->append(") ");
        s->append(c->op);
        s->append(" (");
        writeCondition(c->rhsCondition, s);
        s->append(")");
    }
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
    if (c->isNot) {
        s->append(")");
    }
}

QString Condition::toString() {
    QString s;
    if (enabled) writeCondition(this, &s);
    return s;
}

void Condition::writeConditionXML(const Condition * c,
        QXmlStreamWriter * stream) {
    Q_ASSERT(c != 0);

    if (c->isNot) {
        stream->writeStartElement("not");
    }
    if (c->isValues) {
        QString lhsString = "";
        QString rhsString = "";
        QString opString;

        if (c->lhsIsAgentVariable) {
            lhsString.append("a.");
            lhsString.append(c->lhs);
        } else if (c->lhsIsMessageVariable) {
            lhsString.append("m.");
            lhsString.append(c->lhs);
        } else if (c->lhsIsValue) {
                lhsString.append(QString().setNum(c->lhsDouble));
        }

        if (c->op == "==") opString = "EQ";
        else if (c->op == "!=") opString = "NEQ";
        else if (c->op == "<=") opString = "LEQ";
        else if (c->op == ">=") opString = "GEQ";
        else if (c->op == "<") opString = "LT";
        else if (c->op == ">") opString = "GT";

        if (c->rhsIsAgentVariable) {
            rhsString.append("a.");
            rhsString.append(c->rhs);
        } else if (c->rhsIsMessageVariable) {
            rhsString.append("m.");
            rhsString.append(c->rhs);
        } else if (c->rhsIsValue) {
            rhsString.append(QString().setNum(c->rhsDouble));
        }

        stream->writeStartElement("lhs");
        stream->writeTextElement("value", lhsString);
        stream->writeEndElement();  // lhs
        stream->writeTextElement("op", opString);
        stream->writeStartElement("rhs");
        stream->writeTextElement("value", rhsString);
        stream->writeEndElement();  // rhs
    }
    if (c->isConditions) {
        stream->writeStartElement("lhs");
        writeConditionXML(c->lhsCondition, stream);
        stream->writeEndElement();  // lhs
        stream->writeTextElement("op", c->op);
        stream->writeStartElement("rhs");
        writeConditionXML(c->rhsCondition, stream);
        stream->writeEndElement();  // rhs
    }
    if (c->isTime) {
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
