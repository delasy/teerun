# trn-server
TeeRun Server

## Building
> To build successfully, you need to have [libuv](https://github.com/libuv/libuv), [libwebsockets](https://github.com/warmcat/libwebsockets) (with -DLWS_WITH_LIBUV=ON) and [libduc](https://github.com/delasy/libuv) (with -DLIBDUC_WITH_WS=ON) installed.

To build with [CMake](https://cmake.org):
```bash
$ mkdir -p build
$ (cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug)
$ cmake --build build
```

To build with [Docker](https://www.docker.com):
```bash
$ docker build -t trn-server .
```

## Testing
To test your build with [CMake](https://cmake.org):
```bash
$ (cd build && ctest -T memcheck --output-on-failure)
```

To test your build with [Docker](https://www.docker.com):
```bash
$ docker run trn-server
```

## Contributing
Before doing any commit, mount git hooks by:

```bash
$ scripts/mount-git-hooks.sh
```

## Version
Starting from version 1.0.0 trn-server follows [Semantic Versioning](https://semver.org) rules.
