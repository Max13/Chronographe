Chronographe
============

Note:
-----
First version written in 17 hours... Don't stop on bugs and typo immediately please ;)

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
- **timers_rotation**: When to rotate (change the file) the timers logs. For i.e.: 1 file for 1 rotation
	- `day` (Default): Every day
	- `month`: Every month

Have fun !