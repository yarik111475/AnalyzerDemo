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

class QWidget;
class IAnalyzer
{
protected:
    QJsonObject standardSettings_ {};
    QJsonObject extendedSettings_ {};
public:
    explicit IAnalyzer()=default;
    virtual ~IAnalyzer()=default;

    virtual QWidget* standardWidget()=0;
    virtual QWidget* extendedWidget()=0;
    virtual void handle()=0;

    inline void initStandard(const QJsonObject& standardSettings){
        standardSettings_=standardSettings;
    }

    std::function<void(const QString& obj,const QString& msg)> logFunc_ {nullptr};
    std::function<void(const QByteArray& data)> progressFunc_ {nullptr};
};

extern "C" CORE_EXPORT const char* makeType();
extern "C" CORE_EXPORT IAnalyzer* makeAnalyzer();

#endif // IANALYZER_H
