FROM ghcr.io/armmbed/mbed-os-env:latest

RUN useradd -r --shell /bin/bash bloop-hacker
RUN mkdir /src
RUN chown -R bloop-hacker:bloop-hacker /src
RUN chmod 755 /src

USER bloop-hacker

WORKDIR /src
