#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QMainWindow>
#include <QJsonObject>
#include <map>
#include <memory>

class QMenu;
class QAction;
class QSettings;
class QMdiArea;
class AnalyzerDialog;
class AnalyzerLoader;
class AnalyzerSettings;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QMdiArea* mdiAreaPtr_ {nullptr};
    AnalyzerLoader& analyzerLoader_;
    AnalyzerSettings& analyzerSettings_;
    std::shared_ptr<QSettings> appSettingsPtr_ {nullptr};
    std::map<QString,QString> analyzersMap_;

    void makeMenu();
public:
    explicit MainWindow(std::shared_ptr<QSettings> appSettingsPtr,QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
