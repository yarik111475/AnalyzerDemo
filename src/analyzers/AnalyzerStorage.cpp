#include "AnalyzerStorage.h"
#include "analyzers/IAnalyzer.h"

#include <QDir>
#include <QLibrary>
#include <QFileInfo>
#include <QSettings>
#include <QStringList>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>

AnalyzerStorage::AnalyzerStorage()
{
    const QStringList nameFilters {"*.dll","*.so"};
    const auto librariesList {QDir{analyzersLibrariesPath_}.entryInfoList(nameFilters,QDir::Files)};
    std::for_each(librariesList.begin(),librariesList.end(),[&](const QFileInfo& fileInfo){
        std::shared_ptr<QLibrary> libPtr {new QLibrary(fileInfo.absoluteFilePath())};
        if(libPtr->load()){
            TypeFunc typeFunc {(TypeFunc)libPtr->resolve(qPrintable(typeResolveTag_))};
            AnalyzerFunc analyzerFunc {(AnalyzerFunc)libPtr->resolve(qPrintable(createResolveTag_))};
            if(typeFunc && analyzerFunc){
                const QString analyzerType {typeFunc()};
                librariesContainer_.emplace(analyzerType,libPtr);
            }
        }
    });

#ifdef Q_OS_WIN
    const QString homeDir=qgetenv("USERPROFILE");
#endif
#ifdef Q_OS_LINUX
    const QString homeDir {qgetenv("HOME")};
#endif
    const auto settingsPath {QStringLiteral("%1/%2/%3").arg(homeDir,appDir_,settingsFilename_)};
    QSettings settings {settingsPath,QSettings::IniFormat};

    const auto analyzersIdList {settings.value("analyzers").toStringList()};
    std::for_each(analyzersIdList.begin(),analyzersIdList.end(),[&](const QString& analyzerId){
        settings.beginGroup(analyzerId);
        {
            const QJsonObject standardSettings {QJsonDocument::fromJson(settings.value("params").toByteArray()).object()};
            const QString analyzerName {standardSettings.value("name").toString()};
            const QString analyzerType {standardSettings.value("type").toString()};

            const auto it {librariesContainer_.find(analyzerType)};
            if(it!=librariesContainer_.end()){
                AnalyzerFunc analyzerFunc {(AnalyzerFunc)it->second->resolve(qPrintable(createResolveTag_))};
                if(analyzerFunc){
                    std::shared_ptr<IAnalyzer> analyzerPtr {analyzerFunc()};
                    analyzerPtr->initAnalyzer(standardSettings);
                    instancesContainer_.emplace(analyzerId,analyzerPtr);
                }
            }
        }
        settings.endGroup();
    });
}

AnalyzerStorage::~AnalyzerStorage()
{

}

Instance AnalyzerStorage::getAnalyzerInstance(const QString &analyzerId)
{
    const auto it {instancesContainer_.find(analyzerId)};
    if(it!=instancesContainer_.end()){
        return it->second;
    }
    return nullptr;
}

TypesContainer AnalyzerStorage::getAnalyserTypes() const
{
    TypesContainer typesContainer {};
    typesContainer.reserve(librariesContainer_.size());
    std::transform(librariesContainer_.begin(),librariesContainer_.end(),std::back_inserter(typesContainer),
                   [](const std::pair<QString,std::shared_ptr<QLibrary>>& pair){
        return pair.first;
    });
    return typesContainer;
}

ViewsContainer AnalyzerStorage::getAnalyzerViews() const
{
    ViewsContainer viewsContainer {};
    viewsContainer.reserve(instancesContainer_.size());
    std::transform(instancesContainer_.begin(),instancesContainer_.end(),std::back_inserter(viewsContainer),
                   [](const std::pair<QString,Instance>& pair){
        const QJsonObject standardSettings {pair.second->standardSettings()};
        const QString analyzerName {standardSettings.value("name").toString()};
        const QString analyzerType {standardSettings.value("type").toString()};
        const QString analyzerState {pair.second->analyzerState()==State::Enabled ? "Enabled"  : "Disabled"};
        return std::make_tuple(analyzerName,analyzerType,analyzerState,pair.first);
    });
    return viewsContainer;
}

InstancesContainer AnalyzerStorage::getAnalyzerInstances() const
{
    return instancesContainer_;
}

