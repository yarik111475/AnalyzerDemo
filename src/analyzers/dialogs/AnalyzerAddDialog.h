#ifndef ANALYZERNEWDIALOG_H
#define ANALYZERNEWDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class AnalyzerStorage;
class AnalyzerAddDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerStorage& analyzerStorage_;
    QComboBox* typesComboBoxPtr_ {nullptr};
    QLineEdit* nameLineEditPtr_  {nullptr};
public:
    explicit AnalyzerAddDialog(AnalyzerStorage& analyzerStorage,QWidget* parent=nullptr);
    virtual ~AnalyzerAddDialog()=default;
signals:
    void addSignal(const QString& analyzerType,const QString& analyzerName);
};

#endif // ANALYZERNEWDIALOG_H
