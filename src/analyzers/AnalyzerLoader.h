#ifndef ANALYZERLOADER_H
#define ANALYZERLOADER_H

#include <QMap>
#include <QString>
#include <QSharedPointer>
#include <QCoreApplication>
#include <map>
#include <memory>

class QLibrary;
class IAnalyzer;

using typeFunc=const char*(*)();
using analyzerFunc=IAnalyzer*(*)();

class AnalyzerLoader
{
private:
    const QString typeResolveTag_ {"makeType"};
    const QString analyzerResolveTag_ {"makeAnalyzer"};

    const QString analyzersPath_ {QStringLiteral("%1/../lib/analyzers").arg(qApp->applicationDirPath())};
    std::map<QString,std::shared_ptr<QLibrary>> librariesMap_ {};
    explicit AnalyzerLoader();
public:
    static AnalyzerLoader& instance(){
        static AnalyzerLoader instance_ {};
        return instance_;
    }
    AnalyzerLoader(const AnalyzerLoader& other)=delete;
    AnalyzerLoader& operator=(const AnalyzerLoader& other)=delete;
    std::shared_ptr<QLibrary> getLibrary(const QString& key);
    std::shared_ptr<IAnalyzer> getAnalyzer(const QString& key);
};

#endif // ANALYZERLOADER_H
