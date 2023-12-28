#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QMainWindow>
#include <QJsonObject>


class QMenu;
class QAction;
class QSettings;
class AnalyzerDialog;
class AnalyzerLoader;
class AnalyzerSettings;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    AnalyzerLoader& analyzerLoader_;
    AnalyzerSettings& analyzerSettings_;
    QSharedPointer<QSettings> appSettingsPtr_ {nullptr};
    QMap<QString,QString> analyzersMap_;

    void makeMenu();
public:
    explicit MainWindow(QSharedPointer<QSettings> appSettingsPtr,QWidget *parent = nullptr);

signals:

};

#endif // MAINWINDOW_H
