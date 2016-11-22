/*!
 * \file mpre.h
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Header file for mpre
*/
#ifndef MPRE_H_
#define MPRE_H_

#include <QMetaType>
#include <QPainter>

class Mpre {
  public:
    enum EditMode { Editable, ReadOnly };

    Mpre(QString n = "", QString o = "==", int v = 0,
        bool enabled = false, bool neg = false);

    void paint(QPainter *painter, const QRect &rect,
                const QPalette &palette, EditMode mode) const;

    QString name() const { return myName; }
    QString op() const { return myOp; }
    int value() const { return myValue; }
    void setName(QString n) { myName = n; }
    void setOp(QString o) { myOp = o; }
    void setValue(int v) { myValue = v; }
    bool enabled() const { return myEnabled; }
    void setEnabled(bool enabled) { myEnabled = enabled; }
    void setNot(bool n) { myNot = n; }
    bool isNot() const { return myNot; }

  private:
    bool myEnabled;
    bool myNot;
    QString myName;
    QString myOp;
    int myValue;
};

Q_DECLARE_METATYPE(Mpre)

#endif  // MPRE_H_
