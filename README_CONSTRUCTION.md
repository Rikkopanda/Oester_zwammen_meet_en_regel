nodered:
https://www.youtube.com/watch?v=3QgK4IAAqcQ
https://stevesnoderedguide.com/using-the-node-red-chart-node

zie flows op rikverhoeven usr!!


echo ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIA3+tTzOZ5t6a2yy9EGTDxdIqSRMAT/t3O0oZpbxIWvp node-red@rikverhoeven-N56VM > /usr/src/node-red/.ssh/id_ed25519.pub

echo -----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACAN/rU8zmebemtssvRBkw8XSKkkTAE/7dztKGaW8SFr6QAAAKCsmUxGrJlM
RgAAAAtzc2gtZWQyNTUxOQAAACAN/rU8zmebemtssvRBkw8XSKkkTAE/7dztKGaW8SFr6Q
AAAED3+OzW6EKRpqLzkt3I4J0BqKbStmBI+oEjBK5SX/u02Q3+tTzOZ5t6a2yy9EGTDxdI
qSRMAT/t3O0oZpbxIWvpAAAAG25vZGUtcmVkQHJpa3ZlcmhvZXZlbi1ONTZWTQEC
-----END OPENSSH PRIVATE KEY----- > /usr/src/node-red/.ssh/id_ed25519

chmod 600 /usr/src/node-red/.ssh/id_ed25519


cd /data/projects
git clone git@github.com:Rikkopanda/NODE-RED-OESTERZWAM.git 


# debug nodered;


find / -name "package.json" -not -path "*/node_modules/*" -exec 
ls -la {} \; 2>/dev/null

enviroment variabelen;
cat /proc/6/environ | sed 's/:/\n/g'

Show current directory for PIDs
/data/projects/NODE-RED-OESTERZWAM $ pwdx 6
6: /usr/src/node-red


zie settings.js met line nummers;
cat /data/settings.js -n
zie snel specifiek iets;
cat /data/settings.js | grep -n userDir


   editorTheme: {
       projects: {
           enabled: true
       }
   },





docker logs node-red-oesterzwam

alias dockerrestart = "docker compose down && docker compose up -d"



# config & aanzetten CAN

sudo /sbin/ip link set can0 up type can bitrate 25000



candump can0

cansend can0 123#12ABA

# todo

setting.js in git
automatic add git mail user in compose

add personal aliases in git project


# calibration co2 meter

zero point calibration:
als de ppm 400 is in de omgeving( netzoals buitenlucht )
voor op z'n minst 20 min.

FF 01 87 00 00 00 00 00 78
HD pin van de module naar 0v verbindeen  7sec`





