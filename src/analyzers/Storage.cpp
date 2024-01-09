#include "Storage.h"
#include "analyzers/IAnalyzer.h"

#include <QDir>
#include <QFile>
#include <QLibrary>
#include <QFileInfo>
#include <QSettings>
#include <QStringList>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>

analyzer::Storage::Storage()
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
    const QString homeDir {qgetenv("USERPROFILE")};
#endif
#ifdef Q_OS_LINUX
    const QString homeDir {qgetenv("HOME")};
#endif
    const auto settingsPath {QStringLiteral("%1/%2/%3").arg(homeDir,appDir_,settingsFilename_)};
    QSettings settings {settingsPath,QSettings::IniFormat};

    const auto instancesIdList {settings.value("analyzers").toStringList()};
    std::for_each(instancesIdList.begin(),instancesIdList.end(),[&](const QString& analyzerId){
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
                    instancesContainer_.push_back({analyzerId,analyzerPtr});
                }
            }
        }
        settings.endGroup();
    });
}

analyzer::Storage::~Storage()
{
}

analyzer::Instance analyzer::Storage::getInstance(const QString &analyzerId)
{
    const auto instanceIt {std::find_if(instancesContainer_.begin(),instancesContainer_.end(),
                                [&](const std::pair<const QString,Instance>& pair){
          return (pair.first==analyzerId);
      })};
    if(instanceIt!=instancesContainer_.end()){
        return instanceIt->second;
    }
    return nullptr;
}

bool analyzer::Storage::saveInstances(QString &lastError)
{
#ifdef Q_OS_WIN
    const QString homeDir {qgetenv("USERPROFILE")};
#endif
#ifdef Q_OS_LINUX
    const QString homeDir {qgetenv("HOME")};
#endif
    const auto settingsPath {QStringLiteral("%1/%2/%3").arg(homeDir,appDir_,settingsFilename_)};
    QFile{settingsPath}.remove();
    QSettings settings {settingsPath,QSettings::IniFormat};
    QStringList instancesIdList {};
    std::transform(qAsConst(instancesContainer_).begin(),qAsConst(instancesContainer_).end(),std::back_inserter(instancesIdList),
                   [&](const std::pair<const QString,Instance>& pair){
        return pair.first;
    });
    settings.setValue("analyzers",instancesIdList);
    std::for_each(qAsConst(instancesContainer_).begin(),qAsConst(instancesContainer_).end(),
                  [&](const std::pair<QString,Instance>& pair){
        settings.beginGroup(pair.first);
        {
            settings.setValue("params",QJsonDocument(pair.second->standardSettings()).toJson(QJsonDocument::Compact));
        }
        settings.endGroup();
    });
    return true;
}

bool analyzer::Storage::removeInstance(const QString &analyzerId)
{
    const auto instanceIt {std::find_if(instancesContainer_.begin(),instancesContainer_.end(),
                                [&](const std::pair<const QString,Instance>& pair){
          return (pair.first==analyzerId);
      })};
    if(instanceIt!=instancesContainer_.end()){
        instancesContainer_.erase(instanceIt);
        return true;
    }
    return false;
}

bool analyzer::Storage::addInstance(const QString &analyzerId, const QString &analyzerType, const QString &analyzerName)
{
    const auto libraryIt {librariesContainer_.find(analyzerType)};
    if(libraryIt!=librariesContainer_.end()){
        std::shared_ptr<QLibrary> libPtr {libraryIt->second};
        AnalyzerFunc analyzerFunc {(AnalyzerFunc)libraryIt->second->resolve(qPrintable(createResolveTag_))};
        if(analyzerFunc){
            std::shared_ptr<IAnalyzer> analyzerPtr {analyzerFunc()};
            const QJsonObject standardSettings {
                {"name",analyzerName},
                {"type",analyzerType}
            };
            analyzerPtr->initAnalyzer(standardSettings);
            instancesContainer_.push_back({analyzerId,analyzerPtr});
            return true;
        }
    }
    return false;
}

bool analyzer::Storage::editInstance(const QString &analyzerId, const QString &analyzerType, const QString &analyzerName)
{
    auto instanceIt {std::find_if(instancesContainer_.begin(),instancesContainer_.end(),
                                  [&](const std::pair<QString,Instance>& pair){
            return (pair.first==analyzerId);
        })};
    if(instanceIt!=instancesContainer_.end()){
        instancesContainer_.erase(instanceIt);
        const auto libraryIt {librariesContainer_.find(analyzerType)};
        if(libraryIt!=librariesContainer_.end()){
            std::shared_ptr<QLibrary> libPtr {libraryIt->second};
            AnalyzerFunc analyzerFunc {(AnalyzerFunc)libraryIt->second->resolve(qPrintable(createResolveTag_))};
            if(analyzerFunc){
                std::shared_ptr<IAnalyzer> analyzerPtr {analyzerFunc()};
                const QJsonObject standardSettings {
                    {"name",analyzerName},
                    {"type",analyzerType}
                };
                analyzerPtr->initAnalyzer(standardSettings);
                instancesContainer_.push_back({analyzerId,analyzerPtr});
                return true;
            }
        }
    }
    return false;
}

analyzer::TypesContainer analyzer::Storage::getTypes() const
{
    TypesContainer typesContainer {};
    typesContainer.reserve(librariesContainer_.size());
    std::transform(librariesContainer_.begin(),librariesContainer_.end(),std::back_inserter(typesContainer),
                   [](const std::pair<const QString,std::shared_ptr<QLibrary>>& pair){
        return pair.first;
    });
    return typesContainer;
}

analyzer::ViewsContainer analyzer::Storage::getViews() const
{
    ViewsContainer viewsContainer {};
    viewsContainer.reserve(instancesContainer_.size());
    std::transform(instancesContainer_.begin(),instancesContainer_.end(),std::back_inserter(viewsContainer),
                   [](const std::pair<const QString,Instance>& pair){
        const QJsonObject standardSettings {pair.second->standardSettings()};
        const QString analyzerName {standardSettings.value("name").toString()};
        const QString analyzerType {standardSettings.value("type").toString()};
        const QString analyzerState {pair.second->analyzerState()==State::Enabled ? "Enabled"  : "Disabled"};
        return std::make_tuple(analyzerName,analyzerType,analyzerState,pair.first);
    });
    return viewsContainer;
}

analyzer::InstancesContainer analyzer::Storage::getInstances() const
{
    return instancesContainer_;
}

