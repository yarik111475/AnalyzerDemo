#include <QSharedPointer>
#include <QApplication>
#include <QSettings>
#include <QString>
#include <QDir>
#include <memory>

#include "MainWindow.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/rotating_file_sink.h>

const QString appName {"SpectrumHero"};
const QString orgName  {"Gammatech"};
std::shared_ptr<spdlog::logger> loggerPtr_ {nullptr};
std::shared_ptr<QSettings> appSettingsPtr_ {nullptr};

void initLogger();
void initSettings();
void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication app{argc,argv};
    app.setApplicationName(appName);
    app.setOrganizationName(orgName);
    initLogger();
    initSettings();
    qInstallMessageHandler(msgHandler);
    MainWindow mainWindow {appSettingsPtr_};
    mainWindow.show();
    return app.exec();
}

void initLogger()
{
    const QString appDir {QCoreApplication::applicationDirPath()};
    const QString varLogAppDir {QString("%1/../.var/log/spectrumhero").arg(appDir)};
    const bool isLogDirOk {QDir{}.mkpath(varLogAppDir)};
    if(!isLogDirOk){
        qWarning("Fail to create log directory!");
        return;
    }
    const int logFilescount {5};
    const int logFilesize {1024 * 1024 * 50};
    const QString logName {"SpectrumHero"};
    const QString logfilenamePath {varLogAppDir + "/spectrumhero.log"};
    const spdlog::level::level_enum logLevel {spdlog::level::debug};
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#ifdef Q_OS_WINDOWS
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logfilenamePath.toStdWString(), logFilesize, logFilescount));
#endif
#ifdef Q_OS_LINUX
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logfilenamePath.toStdString(), logFilesize, logFilescount));
#endif
    loggerPtr_.reset(new spdlog::logger(logName.toStdString(), sinks.begin(),sinks.end()));
    spdlog::register_logger(loggerPtr_);
    loggerPtr_->set_level(logLevel);
    loggerPtr_->flush_on(logLevel);
}

void initSettings()
{
}

void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const qint32 logLine {context.line};
    const char *logFile {context.file ? context.file : ""};
    const char *logFunction {context.function ? context.function : ""};
    if(!loggerPtr_){
        qDebug("File: %s; Line: %d; Function: %s; Message: %s",
               logFile,logLine,logFunction,qPrintable(msg));
        return;
    }
    switch(type){
    case QtMsgType::QtDebugMsg:
        loggerPtr_->debug("{}; {}",logFunction,msg.toStdString());
        break;
    case QtMsgType::QtInfoMsg:
        loggerPtr_->info("{}; {}",logFunction,msg.toStdString());
        break;
    case QtMsgType::QtWarningMsg:
        loggerPtr_->warn("{}; {}",logFunction,msg.toStdString());
        break;
    case QtMsgType::QtCriticalMsg:
        loggerPtr_->error("{}; {}",logFunction,msg.toStdString());
        break;
    case QtMsgType::QtFatalMsg:
        loggerPtr_->error("File: {}; Line: {}; Function: {}; Message: {}",logFile,logLine,logFunction,msg.toStdString());
        break;
    }
}
