#ifndef IANALYZER_H
#define IANALYZER_H
#include <QtGlobal>

#if defined(CORE_LIBRARY)
#  define CORE_EXPORT Q_DECL_EXPORT
#else
#  define CORE_EXPORT Q_DECL_IMPORT
#endif

#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <string>
#include <functional>

class IAnalyzer
{
protected:
    QJsonObject paramsObject_ {};
public:
    explicit IAnalyzer()=default;
    virtual ~IAnalyzer()=default;
    virtual void handle(const QJsonObject& parameObject)=0;
    std::function<void(const QString& obj,const QString& msg)> logFunc_ {nullptr};
    std::function<void(const QByteArray& data)> progressFunc_ {nullptr};
};

extern "C" CORE_EXPORT const char* makeType();
extern "C" CORE_EXPORT IAnalyzer* makeAnalyzer();

#endif // IANALYZER_H
