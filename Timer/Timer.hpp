#ifndef     TIMER_HPP
# define    TIMER_HPP

# include   <QFile>
# include   <QObject>
# include   <QSettings>
# include   <QSystemTrayIcon>
# include   <QTimer>

class Timer : public QObject
{
    Q_OBJECT
    private:
        QFile           m_timerFile;
        QSettings       *m_settings;
        QString         m_timersPath;
        QSystemTrayIcon *m_sysTray;
        QTimer          m_timer;

    public:
        Timer(const QIcon &icon, QObject *parent = 0);

        static QMap<QString,QString>    availableSettings(void);

    signals:

    public slots:
        void    show(void);
        void    hide(void);
        void    start(void);
        void    pause(void);
        void    stop(void);
};

#endif // TIMER_HPP
