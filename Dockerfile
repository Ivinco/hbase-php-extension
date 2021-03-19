FROM ubuntu:16.04

ARG CC=/usr/bin/gcc-5
ARG CXX=/usr/bin/g++-5
ARG CFLAGS="-fPIC -g -fno-omit-frame-pointer -O2 -pthread"
ARG CXXFLAGS="-fPIC -g -fno-omit-frame-pointer -O2 -pthread"

ENV JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64/"

RUN apt-get update && \
    apt-get install -y vim maven inetutils-ping python-pip doxygen graphviz clang-format valgrind \
        wget libgflags-dev libgoogle-glog-dev dh-autoreconf pkg-config libssl-dev build-essential \
        libevent-dev cmake libkrb5-dev git openjdk-8-jdk curl unzip google-mock libsodium-dev libdouble-conversion-dev && \
    pip install yapf && \
    apt-get -qq clean && \
    apt-get -y -qq autoremove && \
    rm -rf /tmp/*

#Build HBase

RUN cd /usr/src/ && \
#    wget https://downloads.apache.org/hbase/2.4.2/hbase-2.4.2-src.tar.gz && \
#    tar xzvf hbase-2.4.2-src.tar.gz && \
#    mv hbase-2.4.2 hbase && \
    git clone https://github.com/apache/hbase.git && \
    cd hbase && \
    mvn -DskipTests package

RUN wget https://github.com/cyrusimap/cyrus-sasl/releases/download/cyrus-sasl-2.1.26/cyrus-sasl-2.1.26.tar.gz && \
    tar zxf cyrus-sasl-2.1.26.tar.gz && \
    cd cyrus-sasl-2.1.26 && \
    ./configure && \
    make -j4&& \
    make install &&\
    cp /usr/local/lib/sasl2/* /usr/lib/sasl2/

RUN wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz && \
    tar zxf release-1.8.0.tar.gz && \
    rm -f release-1.8.0.tar.gz && \
    mv googletest-release-1.8.0 gtest && \
    cd gtest && \
    cmake . && \
    make -j4 && \
    make install

#Build HBase native client

RUN  cd /usr/src/ && \
    git clone https://github.com/apache/hbase-native-client && \
    cd hbase-native-client && \
    cmake -DHBASE_HOME=/usr/src/hbase -DDOWNLOAD_DEPENDENCIES=ON . && \
    make && \
    make install


#Install php

#RUN update-locale LANG="C.UTF-8"

RUN apt-get install -y software-properties-common python-software-properties && \
    export LC_ALL="C.UTF-8" && add-apt-repository -y ppa:ondrej/php && \
    apt-get update && \
    apt-get install -y php7.4 php7.4-cli php7.4-common libapache2-mod-php7.4 php7.4-fpm \
        libapache2-mod-fcgid php7.4-curl php7.4-dev php7.4-gd php7.4-mbstring php7.4-zip php7.4-mysql php7.4-xml

#Build PHP-CPP

RUN cd /usr/src/ && \
    git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP  && \
    cd PHP-CPP  && \
    make && \
    make install