Chronographe
============

Description
-----------
This is a dead simple cross-platform task timer. Dead simple because it's a dead simple system-tray icon with a dead-simple context-menu which has only few actions (Start/Stop, Show timers, quit, ...).

You need to time your task? Nothing simplier, [download the latest release](https://github.com/Max13/Chronographe/releases/latest), start it, right-click on the system-tray icon and choose your action. Be sure to read the "known bugs" not to be surprised. The options are implemented as ini files, an right-click action will reveal it.

If you find a bug, want a feature or want to implement a feature yourself, don't hesitate to [open an issue](https://github.com/Max13/Chronographe/issues) on Github, I will see what I can do.

Finally, this app was an idea from [@r8e9](https://github.com/r8e9) who needed a dead simple task timer, like [Thyme](http://joaomoreno.github.io/thyme/) but cross-platform. So I made a fair-trade with [@r8e9](https://github.com/r8e9), I needed a scanned version of the physical monopoly game, we had a deal.

Supported settings
------------------
- **prompt_job**: When to prompt the user for the task name
    - `start` (Default): On start
    - `stop`: On stop
    - **Note**: The timer will start/stop AFTER prompting the reason.
- **timers_dir**: The directory where the timers will be saved
    - Absolute path
    - Relative path (default `./timers`)
    - **Note**: Beginning with a dot means *relative path*
- **timers_format**: The datetime format
    - [`Qt::QDateTime::toString()`](http://qt-project.org/doc/qt-5.1/qtcore/qdatetime.html#toString) format
    - `yyyy-MM-dd hh:mm:ss` (default)
- **timers_rotation**: When to rotate (change the file) the timers logs. For i.e.: 1 file for 1 rotation
    - `day` (Default): Every day
    - `month`: Every month

FAQ
---
**Q) What does "dead-simple" spirit means ?**

A) The "dead-simple app" spirit means this app aims to be dead simple and if you need more advanced options, try to ask it on github issues but not sure I will try to implement them. Usually, I do this because more advanced apps already exist on the market, and the idea is to simplify the needs not re-inventing the wheel ;)


Note:
-----
First version written in ~14 hours... Don't stop on bugs and typo immediately please ;)

Have fun !
