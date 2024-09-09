#!/bin/bash


FILE_TO_WATCH="platformio.ini"
BACKUP="platformio_ini_backup"



echo  ""  >> $BACKUP && diff $BACKUP $FILE_TO_WATCH | grep '^>' | sed 's/^> //' >> $BACKUP

echo "Changes appended to file"


