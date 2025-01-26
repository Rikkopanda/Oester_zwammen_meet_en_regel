#!/bin/sh
git config --global user.name "${GIT_USER_NAME}"
git config --global user.email "${GIT_USER_EMAIL}"

# Don't forget any other configs you need
git config --global --add safe.directory /data/projects/NODE-RED-OESTERZWAM

# Start Node-RED
exec "$@"