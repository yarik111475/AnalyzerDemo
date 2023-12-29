#ifndef ANALYZERSHELL_H
#define ANALYZERSHELL_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QJsonObject>
#include <QSharedPointer>

class QLibrary;
class IAnalyzer;
using makeTypeFunc=const char*(*)();
using makeAnalyzerFunc=IAnalyzer*(*)();

class AnalyzerShell : public QThread
{
    Q_OBJECT
private:
    const QString typeResolveTag_ {"makeType"};
    const QString analyzerResolveTag_ {"makeAnalyzer"};

    QJsonObject paramsObject_ {};
    QSharedPointer<QLibrary> libPtr_ {nullptr};
    QSharedPointer<IAnalyzer> analyzerPtr_ {nullptr};
protected:
    virtual void run()override;
public:
    explicit AnalyzerShell(QSharedPointer<QLibrary> libPtr, const QJsonObject &paramsObject, QObject* parent=nullptr);
    AnalyzerShell(const AnalyzerShell& other)=delete;
    AnalyzerShell& operator=(const AnalyzerShell& other)=delete;
    virtual ~AnalyzerShell();
signals:
    void progressSignal(const QByteArray& data);
    void finishedSignal(const QString& lastError);
};

#endif // ANALYZERSHELL_H
