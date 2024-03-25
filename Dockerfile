FROM debian:12 AS build
RUN apt-get update && apt-get install -y \
      make \
      binutils \
      g++ \
      git \
      cmake \
      libssl-dev \
      libpoco-dev \
      libmariadb-dev \
      libyaml-cpp-dev

WORKDIR /usr/src/nebula-atom

COPY . /usr/src/nebula-atom

RUN mkdir ./build-app \
    && cd ./build-app \
    && cmake ../ \
    && cmake --build ./ \
    && cpack --config CPackSourceConfig.cmake && cpack --config CPackConfig.cmake

FROM debian:12

RUN apt-get update && apt-get install -y \
      libssl-dev \
      libpoco-dev \
      libmariadb-dev \
      libyaml-cpp-dev

COPY --from=build /usr/src/nebula-atom/build-app/cpack/libnebulaatom-*-Linux.deb ./

RUN dpkg -i libnebulaatom-*-Linux.deb && apt-get install -f