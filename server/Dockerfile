FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y build-essential cmake git valgrind && \
    apt-get autoclean && \
    apt-get autoremove && \
    apt-get clean && \
    rm -rf /tmp/* /var/lib/apt/lists/* /var/tmp/*

RUN git clone --depth 1 https://github.com/libuv/libuv.git /tmp/dep-libuv && \
    (cd /tmp/dep-libuv && cmake . -DLIBUV_BUILD_TESTS=OFF) && \
    cmake --build /tmp/dep-libuv && cmake --install /tmp/dep-libuv && rm -rf /tmp/dep-libuv
RUN git clone --depth 1 https://github.com/warmcat/libwebsockets.git /tmp/dep-libwebsockets && \
    (cd /tmp/dep-libwebsockets && cmake . -DLWS_WITH_LIBUV=ON -DLWS_WITH_SSL=OFF -DLWS_WITHOUT_TESTAPPS=ON) && \
    cmake --build /tmp/dep-libwebsockets && cmake --install /tmp/dep-libwebsockets && rm -rf /tmp/dep-libwebsockets
RUN git clone --branch v2.1.1 --depth 1 https://github.com/delasy/libduc.git /tmp/dep-libduc && \
    (cd /tmp/dep-libduc && cmake . -DLIBDUC_BUILD_TESTS=OFF -DLIBDUC_WITH_WS=ON) && \
    cmake --build /tmp/dep-libduc && cmake --install /tmp/dep-libduc && rm -rf /tmp/dep-libduc

RUN ldconfig

WORKDIR /app
COPY . .
RUN mkdir -p build
RUN (cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug)
RUN cmake --build build

ENTRYPOINT ["scripts/docker-entrypoint.sh"]
