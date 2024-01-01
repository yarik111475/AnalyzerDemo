#ifndef ANALYZERNEWDIALOG_H
#define ANALYZERNEWDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class AnalyzerStorage;
class AnalyzerNewDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerStorage& analyzerStorage_;
    QComboBox* typesComboBoxPtr_ {nullptr};
    QLineEdit* nameLineEditPtr_  {nullptr};
public:
    explicit AnalyzerNewDialog(AnalyzerStorage& analyzerStorage,QWidget* parent=nullptr);
    virtual ~AnalyzerNewDialog()=default;
signals:
    void addSignal(const QString& analyzerType,const QString& analyzerName);
};

#endif // ANALYZERNEWDIALOG_H
