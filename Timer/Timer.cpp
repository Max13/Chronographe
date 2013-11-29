#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QStandardPaths>
#include <QtDebug>
#include <QTextStream>

#include "Timer.hpp"

#define SETTINGS_FILENAME   QString("settings.ini")
#define SETTINGS_PATH       QStandardPaths::writableLocation(QStandardPaths::DataLocation)

QMap<QString,QString>   Timer::m_defaultSettings = Timer::defaultSettings();

Timer::Timer(const QIcon &icon, QObject *parent) : QObject(parent), m_timer(NULL)
{
    this->m_settings = new QSettings(SETTINGS_PATH+"/"+SETTINGS_FILENAME, QSettings::IniFormat);
    this->m_sysTray = new QSystemTrayIcon(icon, this);
}

Timer::~Timer(void)
{
    delete this->m_settings;
    this->m_settings = NULL;
    delete this->m_sysTray;
    this->m_sysTray = NULL;

}

// Methods
void                    Timer::init(void)
{
    qDebug() << "Checking settings...";
    foreach (const QString &key, this->m_defaultSettings.keys()) {
        if (!this->m_settings->contains(key)) {
            this->m_settings->setValue(key, this->m_defaultSettings.value(key));
        }
        if (key == "timers_dir") {
            this->m_timersPath = this->m_settings->value(key).toString();
            if (this->m_timersPath.at(0) == '.') {
                this->m_timersPath = QDir(SETTINGS_PATH+"/"+this->m_timersPath).absolutePath();
            }
            if (!QDir().mkpath(this->m_timersPath)) {
                qFatal("Critical: Couldn't create the whole path to log directory (%s).",
                       this->m_timersPath.toStdString().c_str());
            }
        } else if (key == "timers_rotation") {
            QString rotation(this->m_defaultSettings.value(key));
            if (rotation == "day") {
                this->m_timerFile.setFileName(this->m_timersPath+"/"
                                              +QDateTime::currentDateTime().toString("yyyy-MM-dd'.csv'"));
            } else if (rotation == "month") {
                this->m_timerFile.setFileName(this->m_timersPath+"/"
                                              +QDateTime::currentDateTime().toString("yyyy-MM'.csv'"));
            }
        }
    }
    qDebug() << "Settings OK.";

    qDebug() << "Init system tray menu...";
    QMenu   *contextMenu = new QMenu;
    contextMenu->addAction(tr("Start"), this, SLOT(start()));
    contextMenu->addAction(tr("Stop"), this, SLOT(stop()));
    contextMenu->addSeparator();
    contextMenu->addAction(tr("Show app data"), this, SLOT(openAppData()));
    contextMenu->addSeparator();
    contextMenu->addAction(tr("Quit"), this, SLOT(quit()));
    contextMenu->actions().at(1)->setDisabled(true);
    this->m_sysTray->setContextMenu(contextMenu);
    qDebug() << "System tray OK";

    qDebug() << qApp->applicationName() << "ready !";
}

QMap<QString,QString>   Timer::defaultSettings(void)
{
    QMap<QString,QString>   settings;

    settings["prompt_job"] = "start";
    settings["timers_dir"] = "./timers";
    settings["timers_format"] = "yyyy-MM-dd hh:mm:ss";
    settings["timers_rotation"] = "day";

    return (settings);
}

// /Methods

// Signals / Slots
void    Timer::show(void)
{
    this->m_sysTray->show();
    this->m_sysTray->showMessage(QApplication::applicationDisplayName(),
                                 tr("Right-click on the system-tray icon when you want to start !"),
                                 QSystemTrayIcon::Information,
                                 2500);
}

void    Timer::openAppData(void)
{
    qDebug() << "Opening settings path:" << SETTINGS_PATH;
#ifdef  Q_OS_WIN
    system(QString("explorer \""+QDir::toNativeSeparators(SETTINGS_PATH)+"\"").toStdString().c_str());
#elif   defined(Q_OS_MACX)
    system(QString("open \""+SETTINGS_PATH+"\"").toStdString().c_str());
#endif
}

void    Timer::hide(void)
{
    this->m_sysTray->hide();
}

void    Timer::start(void)
{
    QString     dateTimeFormat;
    QString     reason;
    QTextStream stream(&this->m_timerFile);

    if (this->m_timer != NULL) {
        return;
    }
    qDebug() << "Starting timer...";
    if (this->m_settings->value(
                "prompt_job",
                this->defaultSettings().key("prompt_job")
        ) == "start") {
        reason = QInputDialog::getText(0, tr("Reason"), tr("What are you going to start ?"));
        if (!reason.isEmpty() && !reason.isNull()){
            reason.prepend("\"").append("\"");
        }
    }

    dateTimeFormat = this->m_settings
                     ->value("timers_format", this->defaultSettings().key("timers_format"))
                     .toString().prepend("\"").append("\"");

    if (!this->m_timerFile.open(QFile::Text | QFile::Append)) {
        qFatal("Can't write the timer...");
    }

    stream << QDateTime::currentDateTime().toString(dateTimeFormat) << ";"
           << "" << ";"
           << reason << ";"
           << "" << endl;

    this->m_timerFile.close();

    this->m_timer = new QTime;
    this->m_timer->start();

    this->m_sysTray->showMessage(tr("Started"), tr("Timer Started !"), QSystemTrayIcon::Information, 2000);
    this->m_sysTray->contextMenu()->actions().at(0)->setDisabled(true);
    this->m_sysTray->contextMenu()->actions().at(1)->setEnabled(true);
}

void    Timer::pause(void)
{
}

void    Timer::stop(void)
{
    QString     dateTimeFormat;
    QString     reason;
    QTextStream stream(&this->m_timerFile);

    if (this->m_timer == NULL) {
        return;
    }

    if (this->m_settings->value(
                "prompt_job",
                this->defaultSettings().key("prompt_job")
        ) == "stop") {
        reason = QInputDialog::getText(0, tr("Reason"), tr("What were you doing ?"));
        if (!reason.isEmpty() && !reason.isNull()){
            reason.prepend("\"").append("\"");
        }
    }

    dateTimeFormat = this->m_settings
                     ->value("timers_format", this->defaultSettings().key("timers_format"))
                     .toString().prepend("\"").append("\"");

    if (!this->m_timerFile.open(QFile::Text | QFile::Append)) {
        qFatal("Can't write the timer...");
    }

    stream << "" << ";"
           << QDateTime::currentDateTime().toString(dateTimeFormat) << ";"
           << reason << ";"
           << QTime(0,0).addMSecs(this->m_timer->elapsed()).toString("\"hh:mm:ss\"") << endl;

    this->m_timerFile.close();

    qDebug() << "Stopping timer...";
    delete this->m_timer;
    this->m_timer = NULL;

    this->m_sysTray->showMessage(tr("Stopped"), tr("Timer Stopped !"), QSystemTrayIcon::Information, 2000);
    this->m_sysTray->contextMenu()->actions().at(0)->setEnabled(true);
    this->m_sysTray->contextMenu()->actions().at(1)->setDisabled(true);
}

void    Timer::quit(void)
{
    this->stop();
    this->hide();
    QApplication::quit();
}
// /Signals / Slots
