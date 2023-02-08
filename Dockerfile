FROM ghcr.io/armmbed/mbed-os-env:mbed-os-6.15.1-latest

ARG UNAME=bloop-hacker
ARG UID=1000
ARG GID=1000

RUN ["apt", "update"]
RUN ["apt", "install", "-y", "vim", "lsusb"] # Add necessary packages here

RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME
RUN mkdir /src /bloop-dev

RUN chown -R bloop-hacker:bloop-hacker /src

RUN mbed config root /src --global

USER $UNAME

WORKDIR /src

