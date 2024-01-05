#ifndef ANALYZERSTORAGE_H
#define ANALYZERSTORAGE_H

#include <QString>
#include <QCoreApplication>
#include <map>
#include <vector>
#include <tuple>
#include <memory>

class IAnalyzer;
class QLibrary;

using TypeFunc=const char*(*)();
using AnalyzerFunc=IAnalyzer*(*)();

using Instance=std::shared_ptr<IAnalyzer>;
using LibrariesContainer=std::map<QString,std::shared_ptr<QLibrary>>;
using TypesContainer=std::vector<QString>;
using ViewsContainer=std::vector<std::tuple<QString,QString,QString,QString>>;
using InstancesContainer=std::vector<std::pair<QString,Instance>>;

class AnalyzerStorage
{
private:
    const QString settingsFilename_       {"analyzers.ini"};
    const QString appDir_                 {"spectrumhero"};
    const QString typeResolveTag_         {"createType"};
    const QString createResolveTag_       {"createAnalyzer"};
    const QString analyzersLibrariesPath_ {QStringLiteral("%1/../lib/analyzers").arg(qApp->applicationDirPath())};

    LibrariesContainer librariesContainer_ {};
    InstancesContainer instancesContainer_;

    explicit AnalyzerStorage();
public:
    static AnalyzerStorage& instance(){
        static AnalyzerStorage instance_;
        return instance_;
    }
    AnalyzerStorage(const AnalyzerStorage& other)=delete;
    AnalyzerStorage& operator=(const AnalyzerStorage& other)=delete;
    ~AnalyzerStorage();


    Instance getInstance(const QString& analyzerId);
    bool saveInstances(QString& lastError);
    bool removeInstance(const QString& analyzerId);
    bool addInstance(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName);
    bool editInstance(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName);

    TypesContainer getTypes()const;
    ViewsContainer getViews()const;
    InstancesContainer getInstances()const;
};

#endif // ANALYZERSTORAGE_H
