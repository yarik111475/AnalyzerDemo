#ifndef ANALYZERLOADER_H
#define ANALYZERLOADER_H

#include <QMap>
#include <QString>
#include <QSharedPointer>
#include <QCoreApplication>
#include <map>

class QLibrary;
class IAnalyzer;
using makeTypeFunc=const char*(*)();
using makeAnalyzerFunc=IAnalyzer*(*)();

class AnalyzerLoader
{
private:
    const QString typeResolveTag_ {"makeType"};
    const QString analyzerResolveTag_ {"makeAnalyzer"};

    const QString analyzersPath_ {QStringLiteral("%1/../lib/analyzers").arg(qApp->applicationDirPath())};
    std::map<QString,QSharedPointer<QLibrary>> librariesMap_ {};
    explicit AnalyzerLoader();
public:
    static AnalyzerLoader& instance(){
        static AnalyzerLoader instance_ {};
        return instance_;
    }
    AnalyzerLoader(const AnalyzerLoader& other)=delete;
    AnalyzerLoader& operator=(const AnalyzerLoader& other)=delete;
    QSharedPointer<QLibrary> getLibrary(const QString key);
};

#endif // ANALYZERLOADER_H
