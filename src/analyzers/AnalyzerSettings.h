#ifndef ANALYZERSETTINGS_H
#define ANALYZERSETTINGS_H

#include <QMap>
#include <QString>
#include <QJsonObject>
#include <map>

class AnalyzerSettings
{
private:
    QString homeDir_ {};
    const QString appDir_ {"spectrumhero"};
    const QString settingsFilename_ {"analyzers.ini"};
    std::map<QString,QJsonObject> analyzersMap_ {};
    explicit AnalyzerSettings();
public:
    static AnalyzerSettings& instance(){
        static AnalyzerSettings instance_;
        return instance_;
    }
    AnalyzerSettings(const AnalyzerSettings& other)=delete;
    AnalyzerSettings& operator=(const AnalyzerSettings& other)=delete;

    std::map<QString,QJsonObject> getAnalyzers()const;
    QJsonObject getParams(const QString& id)const;
    void setParams(const QString& id,const QJsonObject& paramsObject);
};

#endif // ANALYZERSETTINGS_H
