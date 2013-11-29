#ifndef     TIMER_HPP
# define    TIMER_HPP

# include   <QFile>
# include   <QObject>
# include   <QMap>
# include   <QSettings>
# include   <QSystemTrayIcon>
# include   <QTime>
# include   <QTimer>

class Timer : public QObject
{
    Q_OBJECT
    private:
        QFile                               m_timerFile;
        static QMap<QString,QString>        m_defaultSettings;
        QSettings                           *m_settings;
        QString                             m_timersPath;
        QSystemTrayIcon                     *m_sysTray;
//        QTime                               *m_timer;
        QTimer                              m_autoTimer;

    public:
        Timer(const QIcon &icon, QObject *parent = 0);
        ~Timer(void);

        void                            init();
        static QMap<QString,QString>    defaultSettings(void);

    signals:

    public slots:
        void    show(void);
        void    openAppData(void);
        void    hide(void);
        void    start(const bool faked = false);
        void    stop(void);
        void    quit(void);
};

#endif // TIMER_HPP
