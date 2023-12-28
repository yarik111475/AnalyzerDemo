#include "AnalyzerSettings.h"
#include <QUuid>
#include <QDebug>
#include <QtGlobal>
#include <QSettings>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>

AnalyzerSettings::AnalyzerSettings()
{
#ifdef Q_OS_WIN
    homeDir_=qgetenv("USERPROFILE");
#endif
#ifdef Q_OS_LINUX
    homeDir_=qgetenv("HOME");
#endif
    const auto settingsPath {QStringLiteral("%1/%2/%3").arg(homeDir_,appDir_,settingsFilename_)};
    QSettings settings {settingsPath,QSettings::IniFormat};

#if 0
    const QStringList testIdList {QUuid::createUuid().toString(QUuid::WithoutBraces),
                QUuid::createUuid().toString(QUuid::WithoutBraces),
                QUuid::createUuid().toString(QUuid::WithoutBraces)};
    settings.setValue("analyzers",testIdList);
    std::for_each(testIdList.begin(),testIdList.end(),[&](const QString& id){
        settings.beginGroup(id);
        {
            const QJsonObject paramsObject{
                {"name","test_name"},
                {"type","test_type"}
            };
            settings.setValue("params",QJsonDocument(paramsObject).toJson(QJsonDocument::Compact));
        }
        settings.endGroup();
    });
#endif

    const auto idList {settings.value("analyzers").toStringList()};
    std::for_each(idList.begin(),idList.end(),[&](const QString& id){
        settings.beginGroup(id);
        {
            const QJsonObject paramsObject {QJsonDocument::fromJson(settings.value("params").toByteArray()).object()};
            analyzersMap_.emplace(id,paramsObject);
        }
        settings.endGroup();
    });
}

std::map<QString, QJsonObject> AnalyzerSettings::getAnalyzers() const
{
    return analyzersMap_;
}

QJsonObject AnalyzerSettings::getParams(const QString &id) const
{
    auto it {analyzersMap_.find(id)};
    if(it!=analyzersMap_.end()){
        return it->second;
    }
    return QJsonObject{};
}

void AnalyzerSettings::setParams(const QString &id, const QJsonObject &paramsObject)
{
    const auto settingsPath {QStringLiteral("%1/%2/%3").arg(homeDir_,appDir_,settingsFilename_)};
    QSettings settings {settingsPath,QSettings::IniFormat};
    auto idList {settings.value("analyzers").toStringList()};
    idList.push_back(id);
    settings.setValue("analyzers",idList);
    settings.beginGroup(id);
    {
        settings.setValue("params",QJsonDocument(paramsObject).toJson(QJsonDocument::Compact));
    }
    settings.endGroup();
}
