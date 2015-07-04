FROM ubuntu:14.04
MAINTAINER Yoshikawa Ryota <yoshikawa@heartbeats.jp>

RUN apt-get update -y && apt-get install -y \
      gcc \
      make \
      git \
      rake \
      bison \
      libzmq3-dev \
      && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/mruby/mruby.git /usr/local/mruby
WORKDIR /usr/local/mruby
ADD build_config.rb /tmp/build_config.rb
RUN mkdir /tmp/mruby-zmq
ADD mrbgem.rake /tmp/mruby-zmq/mrbgem.rake
ADD src /tmp/mruby-zmq/src

RUN MRUBY_CONFIG=/tmp/build_config.rb rake
