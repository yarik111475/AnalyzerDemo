#ifndef ANALYZERSTORAGE_H
#define ANALYZERSTORAGE_H

#include <QString>
#include <QCoreApplication>
#include <map>
#include <vector>
#include <tuple>
#include <memory>

class QLibrary;
namespace analyzer
{
class IAnalyzer;
using TypeFunc=const char*(*)();
using AnalyzerFunc=IAnalyzer*(*)();

using Instance=std::shared_ptr<IAnalyzer>;
using LibrariesContainer=std::map<QString,std::shared_ptr<QLibrary>>;
using TypesContainer=std::vector<QString>;
using ViewsContainer=std::vector<std::tuple<QString,QString,QString,QString>>;
using InstancesContainer=std::vector<std::pair<QString,Instance>>;

class Storage
{
private:
    const QString settingsFilename_       {"analyzers.ini"};
    const QString appDir_                 {"spectrumhero"};
    const QString typeResolveTag_         {"createType"};
    const QString createResolveTag_       {"createAnalyzer"};
    const QString analyzersLibrariesPath_ {QStringLiteral("%1/../lib/analyzers").arg(qApp->applicationDirPath())};

    LibrariesContainer librariesContainer_ {};
    InstancesContainer instancesContainer_;

    explicit Storage();
public:
    static Storage& instance(){
        static Storage instance_;
        return instance_;
    }
    Storage(const Storage& other)=delete;
    Storage& operator=(const Storage& other)=delete;
    ~Storage();


    Instance getInstance(const QString& analyzerId);
    bool saveInstances(QString& lastError);
    bool removeInstance(const QString& analyzerId);
    bool addInstance(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName);
    bool editInstance(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName);

    TypesContainer getTypes()const;
    ViewsContainer getViews()const;
    InstancesContainer getInstances()const;
};
}


#endif // ANALYZERSTORAGE_H
