#ifndef DIALOG_H
#define DIALOG_H

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

namespace analyzer
{
class Model;
class Storage;

class Dialog : public QDialog
{
    Q_OBJECT
private:
    analyzer::Model* analyzerModelPtr_   {nullptr};
    analyzer::Storage& analyzerStorage_;

    QComboBox* analyzersComboBoxPtr_     {nullptr};
    QPushButton* analyzersViewButtonPtr_ {nullptr};
    QLineEdit* matrixLineEditPtr_        {nullptr};
    QPushButton* matrixAddButtonPtr_     {nullptr};
    QVBoxLayout* settingsVBoxLayoutPtr_  {nullptr};
    QStackedWidget* stackedWidgetPtr_    {nullptr};
    void resetSettingsWidget(int index);
public:
    explicit Dialog(QWidget *parent = nullptr);
    virtual ~Dialog()=default;

private slots:
    void analyzersViewSlot();
    void matrixAddSlot();
    void modelResetSlot();

signals:

};
}

#endif // DIALOG_H
