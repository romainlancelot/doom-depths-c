FROM alpine:latest

# Install dependencies
RUN apk add --no-cache bash gcc make musl-dev

ENTRYPOINT tail -f
