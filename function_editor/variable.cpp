#include "variable.h"

Variable::Variable()
{
}

double Variable::getValue()
{
    if(type == "int") return (double)(ivalue);
    if(type == "double") return dvalue;
    else return 0.0;
}

void Variable::setValue(double v)
{
    ivalue = (int)v;
    dvalue = v;
}
