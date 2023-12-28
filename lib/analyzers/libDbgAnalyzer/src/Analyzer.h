#ifndef ANALYZER_H
#define ANALYZER_H
#include "analyzers/IAnalyzer.h"

class Analyzer : public IAnalyzer
{
public:
    explicit Analyzer()=default;
    virtual ~Analyzer()=default;
    virtual void handle(const QJsonObject& paramsObject)override;
};

const char* makeType()
{
    return "DbgAnalyzer";
}

IAnalyzer* makeAnalyzer()
{
    return new Analyzer();
}

#endif // ANALYZER_H
