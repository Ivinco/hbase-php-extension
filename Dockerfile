FROM harbor.sgdctroy.net/ivinco/ivinco-openjdk13-mvn:2021-06-11

#ARG CC=/usr/bin/gcc-5
#ARG CXX=/usr/bin/g++-5
ARG CFLAGS="-fPIC -g -fno-omit-frame-pointer -O2 -pthread"
ARG CXXFLAGS="-fPIC -g -fno-omit-frame-pointer -O2 -pthread"

RUN set -xe \
 && dnf install -y --nogpgcheck \
        hostname cmake3 openssl-devel libevent-devel libsodium-devel double-conversion-devel \
        yum-utils centos-release-scl sudo \
        vim python-pip doxygen graphviz valgrind \
        dh-autoreconf unzip \
 && dnf install -y --nogpgcheck --enablerepo=centos-sclo-rh-testing \
        devtoolset-7-gcc devtoolset-7-gcc-c++* \
 && pip install yapf \
 && dnf clean all \
 && rm -rf /tmp/*

# gflags
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /tmp \
 && git clone https://github.com/gflags/gflags.git \
 && cd gflags \
 && mkdir build \
 && cd build \
 && cmake3 -DBUILD_SHARED_LIBS=1 -DGFLAGS_INSTALL_SHARED_LIBS=1 .. \
 && make install

# glog
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /tmp \
 && wget https://github.com/google/glog/archive/v0.4.0.tar.gz \
 && tar zxvf v0.4.0.tar.gz \
 && cd glog-0.4.0 \
 && cmake3 -H. -Bbuild -G "Unix Makefiles" \
 && cmake3 --build build \
 && cmake3 --build build --target install

# gtest
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /tmp \
 && wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz \
 && tar zxf release-1.8.0.tar.gz \
 && rm -f release-1.8.0.tar.gz \
 && mv googletest-release-1.8.0 gtest \
 && cd gtest \
 && cmake3 . \
 && make -j4 \
 && make install

# cyrus-sasl
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /tmp \
 && wget https://github.com/cyrusimap/cyrus-sasl/releases/download/cyrus-sasl-2.1.27/cyrus-sasl-2.1.27.tar.gz \
 && tar zxf cyrus-sasl-2.1.27.tar.gz \
 && cd cyrus-sasl-2.1.27 \
 && ./configure \
 && make \
 && make install \
 && mkdir /usr/lib/sasl2/ \
 && cp /usr/local/lib/sasl2/* /usr/lib/sasl2/ \
 && cd /tmp \
 && rm -rf /tmp/*

# hbase
RUN set -xe \
 && mkdir -p /usr/src/hbase/ \
 && cd /usr/src/hbase/ \
 && git clone https://github.com/apache/hbase.git \
 && cd hbase \
 && mvn -DskipTests package

# hbase-native-client
RUN set -xe \
 && cd /usr/src/hbase/ \
 #&& git clone https://github.com/apache/hbase-native-client.git \
 # until https://github.com/apache/hbase-native-client/pull/15 is merged
 && git clone https://github.com/julia-emelyanova/hbase-native-client.git \
 && source scl_source enable devtoolset-7 \
 && cd /usr/src/hbase/hbase-native-client \
 && cmake3 -DHBASE_HOME=/usr/src/hbase/hbase/ -DDOWNLOAD_DEPENDENCIES=ON . \
 && export LD_LIBRARY_PATH="/usr/local/lib" \
 && make \
 && make install

#INSTALL PHP - copy-paste from our base php image
RUN set -xe \
 && dnf install  -y --enablerepo=ivinco --nogpgcheck \
        git wget epel-release http://rpms.famillecollet.com/enterprise/remi-release-7.rpm \
 && dnf install -y --enablerepo=remi-php74 --nogpgcheck \
        php \
        php-opcache \
        php-xmlrpc \
        php-xml \
        php-pdo \
        php-mbstring \
        php-bcmath \
        php-gd \
        php-mysqlnd \
        php-fpm \
        php-intl \
        php-process \
        php-imap \
 && dnf clean all

RUN set -xe \
 && dnf install -y --enablerepo=remi-php74,ivinco --nogpgcheck \
        php-pear \
        php-pecl-memcache \
        php-pecl-memcached \
        php-pecl-mcrypt \
        php-pecl-igbinary \
        php-pecl-psr \
        php-pecl-zip \
        php-pecl-msgpack \
        php-pecl-xhprof \
        php-symfony-event-dispatcher \
        php-symfony-finder \
        php-symfony-debug \
        php-symfony-polyfill \
        php-symfony-console \
        php-symfony-process \
        php-symfony-css-selector \
        php-symfony-expression-language \
        php-symfony-dependency-injection \
        php-symfony-browser-kit \
        php-symfony-common \
        php-symfony-config \
        php-symfony-dom-crawler \
        php-symfony-class-loader \
        php-symfony-var-dumper \
        php-symfony-http-kernel \
        php-symfony-filesystem \
        php-symfony-yaml \
        php-symfony-http-foundation \
        php-composer-semver \
        php-composer-spdx-licenses \
        php-composer-xdebug-handler \
        php-composer-ca-bundle \
        php-phalcon4 \
        php-justinrainbow-json-schema5 \
        php-seld-phar-utils \
        php-jsonlint \
        php-pecl-thrift \
        php-pecl-rdkafka4 \
        php-devel \
 && dnf clean all

#INSTALL PHP_CPP
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /usr/src/hbase/ \
 && git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP.git \
 && cd PHP-CPP \
 && make \
 && make install

# Add config files
ADD php-conf/php.d/* /etc/php.d/
ADD php-conf/php.ini /etc/php.ini
COPY src /usr/src/hbase/hbase-php-extension/src/
COPY include /usr/src/hbase/hbase-php-extension/include/
ADD CMakeLists.txt /usr/src/hbase/hbase-php-extension/
ADD demo.php /usr/src/hbase/hbase-php-extension/
ADD Dockerfile /usr/src/hbase/hbase-php-extension/
ADD main.cpp /usr/src/hbase/hbase-php-extension/
ADD phphbase.ini /usr/src/hbase/hbase-php-extension/

# Build hbase-php extension
RUN set -xe \
 && source scl_source enable devtoolset-7 \
 && cd /usr/src/hbase/hbase-php-extension \
 && cmake3 . \
 && make

# Add users
RUN set -xe \
 && adduser manticore_prod_blogs_search \
 && adduser manticore_prod_video_search
