FROM ghcr.io/armmbed/mbed-os-env:mbed-os-6.15.1-latest

ARG UNAME=bloop-hacker
ARG UID=1000
ARG GID=1000

RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME
RUN mkdir /src /bloop-dev

COPY bloop-dev-tools/ /bloop-dev

RUN chown -R bloop-hacker:bloop-hacker /src

USER $UNAME

WORKDIR /src
