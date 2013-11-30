#ifndef     TIMER_HPP
# define    TIMER_HPP

# include   <QFile>
# include   <QObject>
# include   <QMap>
# include   <QSettings>
# include   <QSystemTrayIcon>

class Timer : public QObject
{
    Q_OBJECT
    private:
        QFile                               m_timerFile;
        const QIcon                         &m_normalIcon;
        const QIcon                         &m_startedIcon;
        static QMap<QString,QString>        m_defaultSettings;
        QSettings                           *m_settings;
        QString                             m_timersPath;
        QSystemTrayIcon                     *m_sysTray;

    public:
        Timer(const QIcon &normalIcon, const QIcon &startedIcon, QObject *parent = 0);
        ~Timer(void);

        void                            init();
        void                            switchContextMenu(const QString &to);
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
