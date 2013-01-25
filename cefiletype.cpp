#include "cefiletype.h"

CEFileType::CEFileType(QByteArray *stream, QString FunctionName) : _stream(stream), sFunctionName(FunctionName)
{
}

CEFileType::~CEFileType()
{
}
