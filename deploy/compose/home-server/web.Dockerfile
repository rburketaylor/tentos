FROM node:22-alpine AS build

WORKDIR /app

COPY package.json package.json
COPY tsconfig.base.json tsconfig.base.json
COPY apps/web/package.json apps/web/package.json
COPY apps/web apps/web

RUN npm install
RUN npm --workspace apps/web run build

FROM nginx:1.27-alpine

COPY --from=build /app/apps/web/dist /usr/share/nginx/html
COPY deploy/compose/home-server/nginx.conf /etc/nginx/conf.d/default.conf
