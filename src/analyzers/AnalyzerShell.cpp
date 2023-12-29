#include "AnalyzerShell.h"
#include "analyzers/IAnalyzer.h"

#include <QDebug>
#include <QLibrary>

void AnalyzerShell::run()
{
    try{
        makeTypeFunc typeMaker {(makeTypeFunc)libPtr_->resolve(qPrintable(typeResolveTag_))};
        makeAnalyzerFunc analyzerMaker {(makeAnalyzerFunc)libPtr_->resolve(qPrintable(analyzerResolveTag_))};
        if(typeMaker && analyzerMaker){
            analyzerPtr_.reset(analyzerMaker());
            analyzerPtr_->logFunc_=[&](const QString& obj,const QString& msg){
                qDebug("object: %s; message: %s",qPrintable(obj),qPrintable(msg));
            };
            analyzerPtr_->progressFunc_=[&](const QByteArray& data){
                emit progressSignal(data);
            };
            analyzerPtr_->handle(paramsObject_);
        }
    }
    catch(const std::exception& ex){
        emit finishedSignal(QString{ex.what()});
    }
    catch(...){
        emit finishedSignal(QString{"unknown exception"});
    }
    QThread::exec();
}

AnalyzerShell::AnalyzerShell(QSharedPointer<QLibrary> libPtr, const QJsonObject& paramsObject, QObject *parent)
    :QThread{parent},libPtr_{libPtr},paramsObject_{paramsObject}
{
}

AnalyzerShell::~AnalyzerShell()
{
    QThread::quit();
    QThread::wait();
}
