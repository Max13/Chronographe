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

Timer::Timer(const QIcon &icon, QObject *parent) : QObject(parent)
{
    qDebug() << "Checking settings...";
    this->m_settings = new QSettings(SETTINGS_PATH+"/"+SETTINGS_FILENAME, QSettings::IniFormat);
    QMap<QString,QString>   aSettings(Timer::availableSettings());
    foreach (const QString &key, aSettings.keys()) {
        if (!this->m_settings->contains(key)) {
            this->m_settings->setValue(key, aSettings.value(key));
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
            QString rotation(aSettings.value(key));
            if (rotation == "day") {
                this->m_timerFile.setFileName(this->m_timersPath+"/"
                                                  +QDateTime::currentDateTime().toString("yyyy-MM-dd.csv"));
            } else if (rotation == "month") {
                this->m_timerFile.setFileName(this->m_timersPath+"/"
                                                  +QDateTime::currentDateTime().toString("yyyy-MM.csv"));
            }
        }
    }
    qDebug() << "Settings OK.";

    qDebug() << "Init system tray menu...";
//    this->m_sysTray->contextMenu()->addAction(tr("Start"), this, SLOT(start()));
//    this->m_sysTray->contextMenu()->addAction(tr("Stop"), this, SLOT(stop()));
//    this->m_sysTray->contextMenu()->addSeparator();
//    this->m_sysTray->contextMenu()->addAction(tr("Quit"), this, SLOT(quit()));
//    this->m_sysTray->contextMenu()->actions().at(1)->setDisabled(true);
    qDebug() << "System tray OK";

    this->m_sysTray = new QSystemTrayIcon(icon, this);
}



// Methods
QMap<QString,QString>   Timer::availableSettings(void)
{
    QMap<QString,QString>   settings;

    settings["prompt_job"] = "start";
    settings["timers_dir"] = "./timers";
    settings["timers_rotation"] = "day";

    return (settings);
}

// /Methods

// Signals / Slots
void    Timer::show(void)
{
    this->m_sysTray->show();
}

void    Timer::hide(void)
{
    this->m_sysTray->hide();
}

void    Timer::start(void)
{
    QString     reason;
    QTextStream stream(&this->m_timerFile);

    if (this->m_timer.isActive()) {
        return;
    }
    if (this->m_settings->value("prompt_job", "start") == "start") {
        reason = QInputDialog::getText(0, tr("Reason"), tr("What are you going to start ?"));
    }

    if (!this->m_timerFile.open(QFile::Text | QFile::Append)) {
        qFatal("Can't write the timer...");
    }

    stream << QDateTime::currentDateTime().toString("\"yyyy-MMM-dd @ hh:mm:ss\"") << ";"
           << "" << ";"
           << "\""+reason+"\"" << ";"
           << "" << endl;

    this->m_timerFile.close();

    this->m_timer.start();
}

void    Timer::pause(void)
{
}

void    Timer::stop(void)
{
    QString     reason;
    QTextStream stream(&this->m_timerFile);

    if (!this->m_timer.isActive()) {
        return;
    }
    this->m_timer.stop();

    if (this->m_settings->value("prompt_job", "stop") == "stop") {
        reason = QInputDialog::getText(0, tr("Reason"), tr("What were you doing ?"));
    }

    if (!this->m_timerFile.open(QFile::Text | QFile::Append)) {
        qFatal("Can't write the timer...");
    }

    stream << "" << ";"
           << QDateTime::currentDateTime().toString("\"yyyy-MMM-dd @ hh:mm:ss\"") << ";"
           << "\""+reason+"\"" << ";"
           << "\""+QString::number(this->m_timer.interval())+" ms\"" << endl;

    this->m_timerFile.close();
}

void    Timer::quit(void)
{
    this->stop();
    QApplication::quit();
}
// /Signals / Slots
