#ifndef ANALYZER_H
#define ANALYZER_H

#include "analyzers/IAnalyzer.h"

class Analyzer : public IAnalyzer
{
public:
    explicit Analyzer()=default;
    virtual ~Analyzer()=default;
    virtual QWidget* standardWidget()override;
    virtual QWidget* extendedWidget()override;
    virtual void handle()override;
};

const char* createType()
{
    return "TcpAnalyzer";
}

IAnalyzer* createAnalyzer()
{
    return new Analyzer();
}

#endif // ANALYZER_H
