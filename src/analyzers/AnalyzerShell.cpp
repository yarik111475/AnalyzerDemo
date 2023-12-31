#include "AnalyzerShell.h"
#include "analyzers/IAnalyzer.h"

#include <QDebug>
#include <QLibrary>

void AnalyzerShell::run()
{
    try{
        typeFunc typeMaker {(typeFunc)libPtr_->resolve(qPrintable(typeResolveTag_))};
        analyzerFunc analyzerMaker {(analyzerFunc)libPtr_->resolve(qPrintable(analyzerResolveTag_))};
        if(typeMaker && analyzerMaker){
            analyzerPtr_.reset(analyzerMaker());
            analyzerPtr_->logFunc_=[&](const QString& obj,const QString& msg){
                qDebug("object: %s; message: %s",qPrintable(obj),qPrintable(msg));
            };
            analyzerPtr_->progressFunc_=[&](const QByteArray& data){
                emit progressSignal(data);
            };
            //analyzerPtr_->handle(paramsObject_);
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

AnalyzerShell::AnalyzerShell(std::shared_ptr<QLibrary> libPtr, const QJsonObject& paramsObject, QObject *parent)
    :QThread{parent},libPtr_{libPtr},paramsObject_{paramsObject}
{
}

AnalyzerShell::~AnalyzerShell()
{
    QThread::quit();
    QThread::wait();
}
