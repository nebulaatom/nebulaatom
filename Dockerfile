FROM alpine:3.15 AS build
RUN apk --no-cache add \
    make \
    binutils \
    g++ \
    cmake \
    poco-dev \
    mariadb-dev

WORKDIR /usr/src/nebula-atom

COPY . /usr/src/nebula-atom

RUN mkdir ./build-app \
    && cd ./build-app \
    && cmake ../ -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build ./ \
    && cmake --build ./ --target install

FROM alpine:3.15

RUN apk --no-cache add poco

WORKDIR /usr/local/bin

COPY --from=build /usr/local/bin/nebula-atom ./nebula-atom

EXPOSE 8080

CMD ["./nebula-atom"]
