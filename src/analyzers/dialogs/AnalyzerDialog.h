#ifndef ANALYZERDIALOG_H
#define ANALYZERDIALOG_H

#include <QMap>
#include <QDialog>
#include <QJsonObject>
#include <QSharedPointer>

class QWidget;
class QComboBox;
class QLineEdit;
class QVBoxLayout;
class QPushButton;
class AnalyzerLoader;
class AnalyzerSettings;

class AnalyzerDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerLoader& analyzerLoader_;
    AnalyzerSettings& analyzerSettings_;

    QComboBox* analyzersComboBoxPtr_    {nullptr};
    QPushButton* analyzersAddButtonPtr_ {nullptr};
    QLineEdit* matrixLineEditPtr_       {nullptr};
    QPushButton* matrixAddButtonPtr_    {nullptr};
    QVBoxLayout* settingsVBoxLayoutPtr_ {nullptr};

    void resetSettings(const QString& test);
public:
    explicit AnalyzerDialog(QWidget *parent = nullptr);
    virtual ~AnalyzerDialog()=default;

signals:

};

#endif // ANALYZERDIALOG_H
