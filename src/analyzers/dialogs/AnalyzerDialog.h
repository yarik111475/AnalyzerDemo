#ifndef ANALYZERDIALOG_H
#define ANALYZERDIALOG_H

#include <QMap>
#include <QDialog>
#include <QJsonObject>
#include <QSharedPointer>
#include <map>

class QWidget;
class QComboBox;
class QLineEdit;
class QStackedWidget;
class QVBoxLayout;
class QPushButton;
class AnalyzerModel;
class AnalyzerStorage;

class AnalyzerDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerModel* analyzerModelPtr_ {nullptr};
    AnalyzerStorage& analyzerStorage_;

    QComboBox* analyzersComboBoxPtr_     {nullptr};
    QPushButton* analyzersViewButtonPtr_ {nullptr};
    QLineEdit* matrixLineEditPtr_        {nullptr};
    QPushButton* matrixAddButtonPtr_     {nullptr};
    QVBoxLayout* settingsVBoxLayoutPtr_  {nullptr};
    QStackedWidget* stackedWidgetPtr_    {nullptr};
    void resetSettingsWidget(int index);
public:
    explicit AnalyzerDialog(QWidget *parent = nullptr);
    virtual ~AnalyzerDialog()=default;

private slots:
    void analyzersViewSlot();
    void matrixAddSlot();
    void modelResetSlot();

signals:

};

#endif // ANALYZERDIALOG_H
