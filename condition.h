/*!
 * \file condition.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for model condition
 */
#ifndef CONDITION_H_
#define CONDITION_H_

#include <QString>
#include <QMetaType>
#include <QPainter>
#include <QXmlStreamWriter>

class Condition {
  public:
    enum EditMode { Editable, ReadOnly };

    Condition();
    ~Condition();

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;
    void processConditionSide(QString * hs,
            bool * isAgentVariable,
            bool * isMessageVariable,
            bool * isValue,
            double * hsDouble);
    void processSymbols();
    QString toString();
    void writeConditionValueXML(QString * hsString,
                               bool isAgentVariable,
                               bool isMessageVariable,
                               bool isValue,
                               QString hs,
                               double hsDouble);
    void writeConditionValuesXML(
            const Condition * c, QXmlStreamWriter * stream);
    void writeConditionTimeXML(
            const Condition * c, QXmlStreamWriter * stream);
    void writeConditionXML(const Condition * c, QXmlStreamWriter * stream);
    void toXML(QXmlStreamWriter * stream);

    bool enabled;
    bool isNot;
    bool isTime;
    bool isValues;
    bool isConditions;
    bool lhsIsAgentVariable;
    bool rhsIsAgentVariable;
    bool lhsIsValue;
    bool rhsIsValue;
    bool lhsIsMessageVariable;
    bool rhsIsMessageVariable;
    double lhsDouble;
    double rhsDouble;
    QString lhs;
    QString op;
    QString rhs;
    QString timePeriod;
    QString timePhaseVariable;
    int timePhaseValue;
    bool timePhaseIsVariable;
    int timeDuration;
    Condition * lhsCondition;
    Condition * rhsCondition;
    Condition * parentCondition;
};

Q_DECLARE_METATYPE(Condition)

#endif  // CONDITION_H_
