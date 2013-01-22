#include "cevariable.h"

CEVariable::CEVariable()
{
}

double CEVariable::getValue()
{
    if(type == "int") return (double)(ivalue);
    if(type == "double") return dvalue;
    else return 0.0;
}

void CEVariable::setValue(double v)
{
    ivalue = (int)v;
    dvalue = v;
}
