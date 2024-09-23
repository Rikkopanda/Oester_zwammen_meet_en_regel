FROM nodered/node-red

WORKDIR /data
COPY package.json /data/package.json

RUN npm install --unsafe-perm --no-update-notifier --no-fund
WORKDIR /usr/src/node-red

RUN npm audit fix --force || true

COPY test_file /data/test_file
COPY settings.js /data/settings.js
COPY oesterzwam_flows.json /data/flows.json
# volume is mapped to /data so all previous files will be wiped and overwritten
COPY test_file ./test_file
