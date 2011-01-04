time-machine-size
=================

**Determine the size of a Time Machine backup**

Usage: `sudo time-machine-size /Volumes/MyBackups/Backups.backupdb/MyVolumes/2012-01-01-000000`

Time Machine creates backups using directory hard links, so it's not obvious how much space will be saved when a backup is deleted. This tool prints out all the files that exist in only this particular backup, and finally the size in MB used up by the backup.

Note that finding the size of two backups individually and adding them together will *not* accurately determine the size you'll save by deleting them both. Some more space than expected may be saved, because of files present in only the two given backups and none others. If this matters to you, run time-machine-size again after deleting the first backup.
