#include "AnalyzerLoader.h"
#include "analyzers/IAnalyzer.h"

#include <QDir>
#include <QLibrary>
#include <QFileInfo>
#include <QStringList>
#include <algorithm>

AnalyzerLoader::AnalyzerLoader()
{
    const QStringList nameFilters {"*.dll","*.so"};
    const auto librariesList {QDir{analyzersPath_}.entryInfoList(nameFilters,QDir::Files)};
    std::for_each(librariesList.begin(),librariesList.end(),[&](const QFileInfo& fileInfo){
        std::shared_ptr<QLibrary> libPtr {new QLibrary(fileInfo.absoluteFilePath())};
        if(libPtr->load()){
            typeFunc typeMaker {(typeFunc)libPtr->resolve(qPrintable(typeResolveTag_))};
            analyzerFunc analyzerMaker {(analyzerFunc)libPtr->resolve(qPrintable(analyzerResolveTag_))};
            if(typeMaker && analyzerMaker){
                const QString type {typeMaker()};
                librariesMap_.emplace(type,libPtr);
            }
        }
    });
}

std::shared_ptr<QLibrary> AnalyzerLoader::getLibrary(const QString &key)
{
    auto it {librariesMap_.find(key)};
    if(it!=librariesMap_.end()){
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<IAnalyzer> AnalyzerLoader::getAnalyzer(const QString &key)
{
    auto it {librariesMap_.find(key)};
    if(it!=librariesMap_.end()){
        analyzerFunc analyzerMaker {(analyzerFunc)it->second->resolve(qPrintable(analyzerResolveTag_))};
        if(analyzerMaker){
            return std::shared_ptr<IAnalyzer>(analyzerMaker());
        }
    }
    return nullptr;
}
