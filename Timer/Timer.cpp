#include <QAction>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>
#include <QtDebug>
#include <QTextStream>

#include "Timer.hpp"

#define SETTINGS_FILENAME   QString("settings.ini")
#define SETTINGS_PATH       QStandardPaths::writableLocation(QStandardPaths::DataLocation)

QMap<QString,QString>   Timer::m_defaultSettings;

Timer::Timer(const QIcon &icon, QObject *parent) : QObject(parent)
{
    Timer::m_defaultSettings = Timer::defaultSettings();
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
    contextMenu->addSeparator();
    contextMenu->addAction(tr("Show app data"), this, SLOT(openAppData()));
    contextMenu->addSeparator();
    contextMenu->addAction(tr("Quit"), this, SLOT(quit()));
    this->m_sysTray->setContextMenu(contextMenu);
    qDebug() << "System tray OK";

    qDebug() << "Looking for autosave data...";
    QDateTime   lastStart(this->m_settings->value("last/start").toDateTime());
    if (!lastStart.isNull()) {
        qDebug() << "Previous timer found !";
        QMessageBox::StandardButton b;
        b = QMessageBox::question(0,
                                  qApp->applicationDisplayName(),
                                  tr("A started timer was found at %1.\nDo you want to recover it?")
                                    .arg(lastStart.toString(Qt::DefaultLocaleShortDate)));
        if (b == QMessageBox::No) {
            this->m_settings->remove("last");
        } else {
            this->start(true);
        }
    }
    qDebug() << "Autosave checked !";

    qDebug() << qApp->applicationName() << "ready !";
}

QMap<QString,QString>   Timer::defaultSettings(void)
{
    if (!Timer::m_defaultSettings.isEmpty()) {
        return (Timer::m_defaultSettings);
    }
    QMap<QString,QString>   settings;

    settings["autosave"] = "5";
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

void    Timer::start(const bool faked)
{
    QString     reason;

    if (!faked
        && this->m_settings->value(
                "prompt_job",
                this->defaultSettings().key("prompt_job")
        ) == "start") {
        reason = QInputDialog::getText(0,
                                       qApp->applicationDisplayName(),
                                       tr("What are you going to start ?"));
        if (!reason.isEmpty() && !reason.isNull()) {
            this->m_settings->setValue("last/reason", reason);
        }
    }

    if (!faked) {
        qDebug() << "Starting timer...";
        this->m_settings->setValue("last/start", QDateTime::currentDateTime());
        this->m_sysTray->showMessage(tr("Started"), tr("Timer Started !"), QSystemTrayIcon::Information, 2000);
    }

    QAction *action(this->m_sysTray->contextMenu()->actions().takeFirst());
    action->setText(tr("Stop"));
    disconnect(this, SLOT(start())); // TODO
    connect(action, SIGNAL(triggered()), SLOT(stop()));
}

void    Timer::stop(void)
{
    QString     dateTimeFormat;
    QString     reason(this->m_settings->value("last/reason").toString());
    QTextStream stream(&this->m_timerFile);

    if (this->m_settings->value(
                "prompt_job",
                this->defaultSettings().key("prompt_job")
        ) == "stop") {
        reason = QInputDialog::getText(0,
                                       qApp->applicationDisplayName(),
                                       tr("What are you going to start ?"),
                                       QLineEdit::Normal,
                                       reason);
    }

    dateTimeFormat = this->m_settings
                     ->value("timers_format", this->defaultSettings().key("timers_format"))
                     .toString().prepend("\"").append("\"");
    reason.prepend("\"").append("\"");

    if (!this->m_timerFile.open(QFile::Text | QFile::Append)) {
        qFatal("Can't open the timer file...");
    }

    stream << this->m_settings->value("last/start").toDateTime().toString(dateTimeFormat) << ";"// Start
           << QDateTime::currentDateTime().toString(dateTimeFormat) << ";"                      // Stop
           << reason << ";"
           << QTime(0,0).addMSecs(this->m_timer->elapsed()).toString("\"hh:mm:ss\"") << endl; // TODO

    this->m_timerFile.close();

    qDebug() << "Timer stopped.";
    this->m_sysTray->showMessage(tr("Stopped"), tr("Timer Stopped !"), QSystemTrayIcon::Information, 2000);

    QAction *action(this->m_sysTray->contextMenu()->actions().takeFirst());
    action->setText(tr("Stop"));
    disconnect(this, SLOT(stop())); // TODO
    connect(action, SIGNAL(triggered()), SLOT(start()));
}

void    Timer::quit(void)
{
    this->stop();
    this->hide();
    QApplication::quit();
}
// /Signals / Slots
