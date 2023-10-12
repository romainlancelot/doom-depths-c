FROM debian:latest

# Install dependencies
RUN apt update && apt upgrade -y && apt install build-essential -y

ENTRYPOINT tail -f
