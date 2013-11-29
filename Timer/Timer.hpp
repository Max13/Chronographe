#ifndef     TIMER_HPP
# define    TIMER_HPP

# include   <QFile>
# include   <QObject>
# include   <QSettings>
# include   <QSystemTrayIcon>
# include   <QTime>

class Timer : public QObject
{
    Q_OBJECT
    private:
        QFile           m_timerFile;
        QSettings       *m_settings;
        QString         m_timersPath;
        QSystemTrayIcon *m_sysTray;
        QTime           *m_timer;

    public:
        Timer(const QIcon &icon, QObject *parent = 0);
        ~Timer(void);

        void                            init();
        static QMap<QString,QString>    availableSettings(void);

    signals:

    public slots:
        void    show(void);
        void    openAppData(void);
        void    hide(void);
        void    start(void);
        void    pause(void);
        void    stop(void);
        void    quit(void);
};

#endif // TIMER_HPP
