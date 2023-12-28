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
        QSharedPointer<QLibrary> libPtr {new QLibrary(fileInfo.absoluteFilePath())};
        if(libPtr->load()){
            makeTypeFunc typeMaker {(makeTypeFunc)libPtr->resolve(qPrintable(typeResolveTag_))};
            makeAnalyzerFunc analyzerMaker {(makeAnalyzerFunc)libPtr->resolve(qPrintable(analyzerResolveTag_))};
            if(typeMaker && analyzerMaker){
                const QString type {typeMaker()};
                librariesMap_.emplace(type,libPtr);
            }
        }
    });
}

QSharedPointer<QLibrary> AnalyzerLoader::getLibrary(const QString key)
{
    auto it {librariesMap_.find(key)};
    if(it!=librariesMap_.end()){
        return it->second;
    }
    return nullptr;
}
