FROM alpine:latest

# Install dependencies
RUN apk update && apk upgrade
RUN apk add --no-cache bash gcc make musl-dev sqlite sqlite-dev

ENTRYPOINT tail -f
