#ifndef ANALYZER_H
#define ANALYZER_H

#include "analyzers/IAnalyzer.h"

class Analyzer : public IAnalyzer
{
public:
    explicit Analyzer()=default;
    virtual ~Analyzer()=default;
    virtual void handle(const QJsonObject& paramsObject)override;
    virtual QWidget* settingsWidget(const QJsonObject& paramsObject)override;
    virtual QDialog* settingsDialog(const QJsonObject& paramsObject)override;
};

const char* makeType()
{
    return "TcpAnalyzer";
}

IAnalyzer* makeAnalyzer()
{
    return new Analyzer();
}

#endif // ANALYZER_H
