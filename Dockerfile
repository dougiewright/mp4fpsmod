FROM ubuntu:18.04
RUN apt-get update && apt-get -y --no-install-recommends install autoconf libtool automake g++ make
COPY . /build
WORKDIR /build
RUN ./bootstrap.sh
RUN ./configure && make && strip mp4fpsmod
RUN ./mp4fpsmod
