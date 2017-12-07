FROM node:carbon

WORKDIR /usr/src/app

COPY package*.json ./

RUN npm install
# RUN npm install -g node-red

COPY . .

EXPOSE 1880
CMD [ "/usr/src/app/node_modules/node-red/bin/node-red-pi", "flows.json" ]