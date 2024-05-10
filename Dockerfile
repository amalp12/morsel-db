root@b0d2c2d6cd2d:/app# apt-get install libnuma
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
E: Unable to locate package libnuma
root@b0d2c2d6cd2d:/app# apt-get update
Get:1 http://archive.ubuntu.com/ubuntu jammy InRelease [270 kB]
Get:2 http://security.ubuntu.com/ubuntu jammy-security InRelease [110 kB]
Get:3 http://archive.ubuntu.com/ubuntu jammy-updates InRelease [119 kB]
Get:4 http://archive.ubuntu.com/ubuntu jammy-backports InRelease [109 kB]
Get:5 http://security.ubuntu.com/ubuntu jammy-security/multiverse amd64 Packages [44.7 kB]
Get:6 http://archive.ubuntu.com/ubuntu jammy/multiverse amd64 Packages [266 kB]
Get:7 http://archive.ubuntu.com/ubuntu jammy/main amd64 Packages [1792 kB]
Get:8 http://security.ubuntu.com/ubuntu jammy-security/universe amd64 Packages [1082 kB]
Get:9 http://archive.ubuntu.com/ubuntu jammy/universe amd64 Packages [17.5 MB]
Get:10 http://security.ubuntu.com/ubuntu jammy-security/main amd64 Packages [1798 kB]
Get:11 http://security.ubuntu.com/ubuntu jammy-security/restricted amd64 Packages [2308 kB]
Get:12 http://archive.ubuntu.com/ubuntu jammy/restricted amd64 Packages [164 kB]
Get:13 http://archive.ubuntu.com/ubuntu jammy-updates/universe amd64 Packages [1374 kB]
Get:14 http://archive.ubuntu.com/ubuntu jammy-updates/restricted amd64 Packages [2382 kB]
Get:15 http://archive.ubuntu.com/ubuntu jammy-updates/main amd64 Packages [2069 kB]
Get:16 http://archive.ubuntu.com/ubuntu jammy-updates/multiverse amd64 Packages [51.1 kB]
Get:17 http://archive.ubuntu.com/ubuntu jammy-backports/main amd64 Packages [81.0 kB]
Get:18 http://archive.ubuntu.com/ubuntu jammy-backports/universe amd64 Packages [31.9 kB]
Fetched 31.5 MB in 6s (5416 kB/s)
Reading package lists... Done
root@b0d2c2d6cd2d:/app# apt-get install libnuma
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
E: Unable to locate package libnuma
root@b0d2c2d6cd2d:/app# apt-get install libnuma-de
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
E: Unable to locate package libnuma-de
root@b0d2c2d6cd2d:/app# apt-get install libnuma-dev
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following NEW packages will be installed:
  libnuma-dev
0 upgraded, 1 newly installed, 0 to remove and 0 not upgraded.
Need to get 35.9 kB of archives.
After this operation, 161 kB of additional disk space will be used.
Get:1 http://archive.ubuntu.com/ubuntu jammy/main amd64 libnuma-dev amd64 2.0.14-3ubuntu2 [35.9 kB]
Fetched 35.9 kB in 1s (30.6 kB/s)
debconf: delaying package configuration, since apt-utils is not installed
Selecting previously unselected package libnuma-dev:amd64.
(Reading database ... 20961 files and directories currently installed.)
Preparing to unpack .../libnuma-dev_2.0.14-3ubuntu2_amd64.deb ...
Unpacking libnuma-dev:amd64 (2.0.14-3ubuntu2) ...
Setting up libnuma-dev:amd64 (2.0.14-3ubuntu2) ...
root@b0d2c2d6cd2d:/app# apt-get install libnuma
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
E: Unable to locate package libnuma
root@b0d2c2d6cd2d:/app# apt-get install libnuma-dev
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
libnuma-dev is already the newest version (2.0.14-3ubuntu2).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
root@b0d2c2d6cd2d:/app# ./compile.sh
-- Configuring done
-- Generating done
-- Build files have been written to: /app/build
[1/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/static.cpp.o -MF src/CMakeFiles/srclib.dir/static.cpp.o.d -o src/CMakeFiles/srclib.dir/static.cpp.o -c /app/src/static.cpp
[2/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/init.cpp.o -MF src/CMakeFiles/srclib.dir/init.cpp.o.d -o src/CMakeFiles/srclib.dir/init.cpp.o -c /app/src/init.cpp
[3/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/attribute.cpp.o -MF src/CMakeFiles/srclib.dir/attribute.cpp.o.d -o src/CMakeFiles/srclib.dir/attribute.cpp.o -c /app/src/attribute.cpp
[4/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/loop_functions.cpp.o -MF src/CMakeFiles/srclib.dir/loop_functions.cpp.o.d -o src/CMakeFiles/srclib.dir/loop_functions.cpp.o -c /app/src/loop_functions.cpp
[5/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -MF tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o.d -o tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -c /app/tests/morsel_test.cpp
[6/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -MF src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o.d -o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -c /app/src/b_plus_tree.cpp
[7/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/operators.cpp.o -MF src/CMakeFiles/srclib.dir/operators.cpp.o.d -o src/CMakeFiles/srclib.dir/operators.cpp.o -c /app/src/operators.cpp
[8/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/dbapp.dir/app.cpp.o -MF src/CMakeFiles/dbapp.dir/app.cpp.o.d -o src/CMakeFiles/dbapp.dir/app.cpp.o -c /app/src/app.cpp
[9/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/tuple.cpp.o -MF src/CMakeFiles/srclib.dir/tuple.cpp.o.d -o src/CMakeFiles/srclib.dir/tuple.cpp.o -c /app/src/tuple.cpp
[10/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/relcat.cpp.o -MF src/CMakeFiles/srclib.dir/relcat.cpp.o.d -o src/CMakeFiles/srclib.dir/relcat.cpp.o -c /app/src/relcat.cpp
[11/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/dispatcher.cpp.o -MF src/CMakeFiles/srclib.dir/dispatcher.cpp.o.d -o src/CMakeFiles/srclib.dir/dispatcher.cpp.o -c /app/src/dispatcher.cpp
[12/15] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/test.cpp.o -MF src/CMakeFiles/srclib.dir/test.cpp.o.d -o src/CMakeFiles/srclib.dir/test.cpp.o -c /app/src/test.cpp
[13/15] : && /usr/bin/cmake -E rm -f src/libsrclib.a && /usr/bin/ar qc src/libsrclib.a  src/CMakeFiles/srclib.dir/morsel.cpp.o src/CMakeFiles/srclib.dir/operators.cpp.o src/CMakeFiles/srclib.dir/relcat.cpp.o src/CMakeFiles/srclib.dir/parser.cpp.o src/CMakeFiles/srclib.dir/attribute.cpp.o src/CMakeFiles/srclib.dir/dispatcher.cpp.o src/CMakeFiles/srclib.dir/init.cpp.o src/CMakeFiles/srclib.dir/lib.cpp.o src/CMakeFiles/srclib.dir/loop_functions.cpp.o src/CMakeFiles/srclib.dir/tuple.cpp.o src/CMakeFiles/srclib.dir/test.cpp.o src/CMakeFiles/srclib.dir/static.cpp.o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o && /usr/bin/ranlib src/libsrclib.a && :
[14/15] : && /usr/bin/g++ -g  src/CMakeFiles/dbapp.dir/app.cpp.o -o src/dbapp -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
[15/15] : && /usr/bin/g++ -g  tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -o tests/morsel_test -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
root@b0d2c2d6cd2d:/app# exit
exit
❯ nvim Dockerfile
❯ docker build -t my-docker-image .

[+] Building 85.7s (13/13) FINISHED                                                                                                                                                          docker:default
 => [internal] load build definition from Dockerfile                                                                                                                                                   0.1s
 => => transferring dockerfile: 1.32kB                                                                                                                                                                 0.0s
 => [internal] load metadata for docker.io/library/ubuntu:22.04                                                                                                                                        1.8s
 => [internal] load .dockerignore                                                                                                                                                                      0.0s
 => => transferring context: 2B                                                                                                                                                                        0.0s
 => CACHED [1/9] FROM docker.io/library/ubuntu:22.04@sha256:a6d2b38300ce017add71440577d5b0a90460d0e57fd7aec21dd0d1b0761bbfb2                                                                           0.0s
 => [2/9] RUN apt-get update &&     apt-get install -y     git     cmake     ninja-build     vim     ccache     libnuma-dev     curl     build-essential    && rm -rf /var/lib/apt/lists/*            55.6s
 => [3/9] RUN git clone https://github.com/hyrise/sql-parser.git                                                                                                                                       2.7s
 => [4/9] WORKDIR /sql-parser                                                                                                                                                                          0.0s
 => [5/9] RUN make clean &&     make &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_release.so &&     make clean                                             8.0s
 => [6/9] RUN make clean &&     make mode=debug &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_debug.so &&     make clean                                    6.7s
 => [7/9] RUN rm -rf /sql-parser                                                                                                                                                                       0.4s
 => [8/9] WORKDIR /app                                                                                                                                                                                 0.1s
 => [9/9] RUN git clone -b demo --single-branch https://github.com/amalp12/morsel-db.git  .                                                                                                            6.5s
 => exporting to image                                                                                                                                                                                 3.6s
 => => exporting layers                                                                                                                                                                                3.6s
 => => writing image sha256:a926afa54df5e2c7cdd30873ed0b76c24ed55420c7fc5203629c43c6fc1c361a                                                                                                           0.0s
 => => naming to docker.io/library/my-docker-image                                                                                                                                                     0.0s
❯ docker run -it my-docker-image

root@37f5112a61cf:/app# ./compile.sh
-- The CXX compiler identification is GNU 11.4.0
-- The C compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    CMAKE_VERBOSE_MAKEFILE


-- Build files have been written to: /app/build
[1/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/static.cpp.o -MF src/CMakeFiles/srclib.dir/static.cpp.o.d -o src/CMakeFiles/srclib.dir/static.cpp.o -c /app/src/static.cpp
[2/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/morsel.cpp.o -MF src/CMakeFiles/srclib.dir/morsel.cpp.o.d -o src/CMakeFiles/srclib.dir/morsel.cpp.o -c /app/src/morsel.cpp
/app/src/morsel.cpp: In member function 'int Morsel::setNthMorselEntry(int, void*, std::__cxx11::list<Attribute>)':
/app/src/morsel.cpp:57:31: warning: pointer of type 'void *' used in arithmetic [-Wpointer-arith]
   57 |     memcpy((char *)(nthMorsel + offset), (char *)(ptr + attr.offset),
      |                     ~~~~~~~~~~^~~~~~~~
/app/src/morsel.cpp:57:55: warning: pointer of type 'void *' used in arithmetic [-Wpointer-arith]
   57 |     memcpy((char *)(nthMorsel + offset), (char *)(ptr + attr.offset),
      |                                                   ~~~~^~~~~~~~~~~~~
[3/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/parser.cpp.o -MF src/CMakeFiles/srclib.dir/parser.cpp.o.d -o src/CMakeFiles/srclib.dir/parser.cpp.o -c /app/src/parser.cpp
[4/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/init.cpp.o -MF src/CMakeFiles/srclib.dir/init.cpp.o.d -o src/CMakeFiles/srclib.dir/init.cpp.o -c /app/src/init.cpp
[5/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/lib.cpp.o -MF src/CMakeFiles/srclib.dir/lib.cpp.o.d -o src/CMakeFiles/srclib.dir/lib.cpp.o -c /app/src/lib.cpp
[6/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/attribute.cpp.o -MF src/CMakeFiles/srclib.dir/attribute.cpp.o.d -o src/CMakeFiles/srclib.dir/attribute.cpp.o -c /app/src/attribute.cpp
[7/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/loop_functions.cpp.o -MF src/CMakeFiles/srclib.dir/loop_functions.cpp.o.d -o src/CMakeFiles/srclib.dir/loop_functions.cpp.o -c /app/src/loop_functions.cpp
[8/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -MF tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o.d -o tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -c /app/tests/morsel_test.cpp
[9/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/operators.cpp.o -MF src/CMakeFiles/srclib.dir/operators.cpp.o.d -o src/CMakeFiles/srclib.dir/operators.cpp.o -c /app/src/operators.cpp
[10/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -MF src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o.d -o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -c /app/src/b_plus_tree.cpp
[11/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/tuple.cpp.o -MF src/CMakeFiles/srclib.dir/tuple.cpp.o.d -o src/CMakeFiles/srclib.dir/tuple.cpp.o -c /app/src/tuple.cpp
[12/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/dbapp.dir/app.cpp.o -MF src/CMakeFiles/dbapp.dir/app.cpp.o.d -o src/CMakeFiles/dbapp.dir/app.cpp.o -c /app/src/app.cpp
[13/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/relcat.cpp.o -MF src/CMakeFiles/srclib.dir/relcat.cpp.o.d -o src/CMakeFiles/srclib.dir/relcat.cpp.o -c /app/src/relcat.cpp
[14/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/dispatcher.cpp.o -MF src/CMakeFiles/srclib.dir/dispatcher.cpp.o.d -o src/CMakeFiles/srclib.dir/dispatcher.cpp.o -c /app/src/dispatcher.cpp
[15/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/test.cpp.o -MF src/CMakeFiles/srclib.dir/test.cpp.o.d -o src/CMakeFiles/srclib.dir/test.cpp.o -c /app/src/test.cpp
[16/18] : && /usr/bin/cmake -E rm -f src/libsrclib.a && /usr/bin/ar qc src/libsrclib.a  src/CMakeFiles/srclib.dir/morsel.cpp.o src/CMakeFiles/srclib.dir/operators.cpp.o src/CMakeFiles/srclib.dir/relcat.cpp.o src/CMakeFiles/srclib.dir/parser.cpp.o src/CMakeFiles/srclib.dir/attribute.cpp.o src/CMakeFiles/srclib.dir/dispatcher.cpp.o src/CMakeFiles/srclib.dir/init.cpp.o src/CMakeFiles/srclib.dir/lib.cpp.o src/CMakeFiles/srclib.dir/loop_functions.cpp.o src/CMakeFiles/srclib.dir/tuple.cpp.o src/CMakeFiles/srclib.dir/test.cpp.o src/CMakeFiles/srclib.dir/static.cpp.o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o && /usr/bin/ranlib src/libsrclib.a && :
[17/18] : && /usr/bin/g++ -g  src/CMakeFiles/dbapp.dir/app.cpp.o -o src/dbapp -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
[18/18] : && /usr/bin/g++ -g  tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -o tests/morsel_test -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
root@37f5112a61cf:/app# git branch
* demo
root@37f5112a61cf:/app# q
bash: q: command not found
root@37f5112a61cf:/app# exit
exit
❯ docker build -t my-docker-image .

[+] Building 1.7s (13/13) FINISHED                                                                                                                                                           docker:default
 => [internal] load build definition from Dockerfile                                                                                                                                                   0.0s
 => => transferring dockerfile: 1.32kB                                                                                                                                                                 0.0s
 => [internal] load metadata for docker.io/library/ubuntu:22.04                                                                                                                                        1.7s
 => [internal] load .dockerignore                                                                                                                                                                      0.0s
 => => transferring context: 2B                                                                                                                                                                        0.0s
 => [1/9] FROM docker.io/library/ubuntu:22.04@sha256:a6d2b38300ce017add71440577d5b0a90460d0e57fd7aec21dd0d1b0761bbfb2                                                                                  0.0s
 => CACHED [2/9] RUN apt-get update &&     apt-get install -y     git     cmake     ninja-build     vim     ccache     libnuma-dev     curl     build-essential    && rm -rf /var/lib/apt/lists/*      0.0s
 => CACHED [3/9] RUN git clone https://github.com/hyrise/sql-parser.git                                                                                                                                0.0s
 => CACHED [4/9] WORKDIR /sql-parser                                                                                                                                                                   0.0s
 => CACHED [5/9] RUN make clean &&     make &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_release.so &&     make clean                                      0.0s
 => CACHED [6/9] RUN make clean &&     make mode=debug &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_debug.so &&     make clean                             0.0s
 => CACHED [7/9] RUN rm -rf /sql-parser                                                                                                                                                                0.0s
 => CACHED [8/9] WORKDIR /app                                                                                                                                                                          0.0s
 => CACHED [9/9] RUN git clone -b demo --single-branch https://github.com/amalp12/morsel-db.git  .                                                                                                     0.0s
 => exporting to image                                                                                                                                                                                 0.0s
 => => exporting layers                                                                                                                                                                                0.0s
 => => writing image sha256:a926afa54df5e2c7cdd30873ed0b76c24ed55420c7fc5203629c43c6fc1c361a                                                                                                           0.0s
 => => naming to docker.io/library/my-docker-image                                                                                                                                                     0.0s
❯ docker build -t my-docker-image .

[+] Building 0.9s (13/13) FINISHED                                                                                                                                                           docker:default
 => [internal] load build definition from Dockerfile                                                                                                                                                   0.0s
 => => transferring dockerfile: 1.32kB                                                                                                                                                                 0.0s
 => [internal] load metadata for docker.io/library/ubuntu:22.04                                                                                                                                        0.8s
 => [internal] load .dockerignore                                                                                                                                                                      0.0s
 => => transferring context: 2B                                                                                                                                                                        0.0s
 => [1/9] FROM docker.io/library/ubuntu:22.04@sha256:a6d2b38300ce017add71440577d5b0a90460d0e57fd7aec21dd0d1b0761bbfb2                                                                                  0.0s
 => CACHED [2/9] RUN apt-get update &&     apt-get install -y     git     cmake     ninja-build     vim     ccache     libnuma-dev     curl     build-essential    && rm -rf /var/lib/apt/lists/*      0.0s
 => CACHED [3/9] RUN git clone https://github.com/hyrise/sql-parser.git                                                                                                                                0.0s
 => CACHED [4/9] WORKDIR /sql-parser                                                                                                                                                                   0.0s
 => CACHED [5/9] RUN make clean &&     make &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_release.so &&     make clean                                      0.0s
 => CACHED [6/9] RUN make clean &&     make mode=debug &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_debug.so &&     make clean                             0.0s
 => CACHED [7/9] RUN rm -rf /sql-parser                                                                                                                                                                0.0s
 => CACHED [8/9] WORKDIR /app                                                                                                                                                                          0.0s
 => CACHED [9/9] RUN git clone -b demo --single-branch https://github.com/amalp12/morsel-db.git  .                                                                                                     0.0s
 => exporting to image                                                                                                                                                                                 0.0s
 => => exporting layers                                                                                                                                                                                0.0s
 => => writing image sha256:a926afa54df5e2c7cdd30873ed0b76c24ed55420c7fc5203629c43c6fc1c361a                                                                                                           0.0s
 => => naming to docker.io/library/my-docker-image                                                                                                                                                     0.0s
❯ docker run -it my-docker-image

root@2c0cf2627d3a:/app# ./compile.sh
-- The CXX compiler identification is GNU 11.4.0
-- The C compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    CMAKE_VERBOSE_MAKEFILE


-- Build files have been written to: /app/build
[1/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/static.cpp.o -MF src/CMakeFiles/srclib.dir/static.cpp.o.d -o src/CMakeFiles/srclib.dir/static.cpp.o -c /app/src/static.cpp
[2/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/morsel.cpp.o -MF src/CMakeFiles/srclib.dir/morsel.cpp.o.d -o src/CMakeFiles/srclib.dir/morsel.cpp.o -c /app/src/morsel.cpp
/app/src/morsel.cpp: In member function 'int Morsel::setNthMorselEntry(int, void*, std::__cxx11::list<Attribute>)':
/app/src/morsel.cpp:57:31: warning: pointer of type 'void *' used in arithmetic [-Wpointer-arith]
   57 |     memcpy((char *)(nthMorsel + offset), (char *)(ptr + attr.offset),
      |                     ~~~~~~~~~~^~~~~~~~
/app/src/morsel.cpp:57:55: warning: pointer of type 'void *' used in arithmetic [-Wpointer-arith]
   57 |     memcpy((char *)(nthMorsel + offset), (char *)(ptr + attr.offset),
      |                                                   ~~~~^~~~~~~~~~~~~
[3/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/init.cpp.o -MF src/CMakeFiles/srclib.dir/init.cpp.o.d -o src/CMakeFiles/srclib.dir/init.cpp.o -c /app/src/init.cpp
[4/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/lib.cpp.o -MF src/CMakeFiles/srclib.dir/lib.cpp.o.d -o src/CMakeFiles/srclib.dir/lib.cpp.o -c /app/src/lib.cpp
[5/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/parser.cpp.o -MF src/CMakeFiles/srclib.dir/parser.cpp.o.d -o src/CMakeFiles/srclib.dir/parser.cpp.o -c /app/src/parser.cpp
[6/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/attribute.cpp.o -MF src/CMakeFiles/srclib.dir/attribute.cpp.o.d -o src/CMakeFiles/srclib.dir/attribute.cpp.o -c /app/src/attribute.cpp
[7/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/loop_functions.cpp.o -MF src/CMakeFiles/srclib.dir/loop_functions.cpp.o.d -o src/CMakeFiles/srclib.dir/loop_functions.cpp.o -c /app/src/loop_functions.cpp
[8/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -MF tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o.d -o tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -c /app/tests/morsel_test.cpp
[9/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/dbapp.dir/app.cpp.o -MF src/CMakeFiles/dbapp.dir/app.cpp.o.d -o src/CMakeFiles/dbapp.dir/app.cpp.o -c /app/src/app.cpp
[10/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -MF src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o.d -o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o -c /app/src/b_plus_tree.cpp
[11/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/operators.cpp.o -MF src/CMakeFiles/srclib.dir/operators.cpp.o.d -o src/CMakeFiles/srclib.dir/operators.cpp.o -c /app/src/operators.cpp
[12/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/tuple.cpp.o -MF src/CMakeFiles/srclib.dir/tuple.cpp.o.d -o src/CMakeFiles/srclib.dir/tuple.cpp.o -c /app/src/tuple.cpp
[13/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/dispatcher.cpp.o -MF src/CMakeFiles/srclib.dir/dispatcher.cpp.o.d -o src/CMakeFiles/srclib.dir/dispatcher.cpp.o -c /app/src/dispatcher.cpp
[14/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/relcat.cpp.o -MF src/CMakeFiles/srclib.dir/relcat.cpp.o.d -o src/CMakeFiles/srclib.dir/relcat.cpp.o -c /app/src/relcat.cpp
[15/18] ccache /usr/bin/g++  -I/app/./include -g -MD -MT src/CMakeFiles/srclib.dir/test.cpp.o -MF src/CMakeFiles/srclib.dir/test.cpp.o.d -o src/CMakeFiles/srclib.dir/test.cpp.o -c /app/src/test.cpp
[16/18] : && /usr/bin/cmake -E rm -f src/libsrclib.a && /usr/bin/ar qc src/libsrclib.a  src/CMakeFiles/srclib.dir/morsel.cpp.o src/CMakeFiles/srclib.dir/operators.cpp.o src/CMakeFiles/srclib.dir/relcat.cpp.o src/CMakeFiles/srclib.dir/parser.cpp.o src/CMakeFiles/srclib.dir/attribute.cpp.o src/CMakeFiles/srclib.dir/dispatcher.cpp.o src/CMakeFiles/srclib.dir/init.cpp.o src/CMakeFiles/srclib.dir/lib.cpp.o src/CMakeFiles/srclib.dir/loop_functions.cpp.o src/CMakeFiles/srclib.dir/tuple.cpp.o src/CMakeFiles/srclib.dir/test.cpp.o src/CMakeFiles/srclib.dir/static.cpp.o src/CMakeFiles/srclib.dir/b_plus_tree.cpp.o && /usr/bin/ranlib src/libsrclib.a && :
[17/18] : && /usr/bin/g++ -g  src/CMakeFiles/dbapp.dir/app.cpp.o -o src/dbapp -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
[18/18] : && /usr/bin/g++ -g  tests/CMakeFiles/morsel_test.dir/morsel_test.cpp.o -o tests/morsel_test -L/usr/local/lib -Wl,-rpath,/usr/local/lib  -lsqlparser_debug  src/libsrclib.a  -lnuma && :
root@2c0cf2627d3a:/app# :q
bash: :q: command not found
root@2c0cf2627d3a:/app# exit
exit
❯ docker build -t my-docker-image .

[+] Building 0.9s (13/13) FINISHED                                                                                                                                                           docker:default
 => [internal] load build definition from Dockerfile                                                                                                                                                   0.0s
 => => transferring dockerfile: 1.32kB                                                                                                                                                                 0.0s
 => [internal] load metadata for docker.io/library/ubuntu:22.04                                                                                                                                        0.8s
 => [internal] load .dockerignore                                                                                                                                                                      0.0s
 => => transferring context: 2B                                                                                                                                                                        0.0s
 => [1/9] FROM docker.io/library/ubuntu:22.04@sha256:a6d2b38300ce017add71440577d5b0a90460d0e57fd7aec21dd0d1b0761bbfb2                                                                                  0.0s
 => CACHED [2/9] RUN apt-get update &&     apt-get install -y     git     cmake     ninja-build     vim     ccache     libnuma-dev     curl     build-essential    && rm -rf /var/lib/apt/lists/*      0.0s
 => CACHED [3/9] RUN git clone https://github.com/hyrise/sql-parser.git                                                                                                                                0.0s
 => CACHED [4/9] WORKDIR /sql-parser                                                                                                                                                                   0.0s
 => CACHED [5/9] RUN make clean &&     make &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_release.so &&     make clean                                      0.0s
 => CACHED [6/9] RUN make clean &&     make mode=debug &&     make install &&     mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_debug.so &&     make clean                             0.0s
 => CACHED [7/9] RUN rm -rf /sql-parser                                                                                                                                                                0.0s
 => CACHED [8/9] WORKDIR /app                                                                                                                                                                          0.0s
 => CACHED [9/9] RUN git clone -b demo --single-branch https://github.com/amalp12/morsel-db.git  .                                                                                                     0.0s
 => exporting to image                                                                                                                                                                                 0.0s
 => => exporting layers                                                                                                                                                                                0.0s
 => => writing image sha256:a926afa54df5e2c7cdd30873ed0b76c24ed55420c7fc5203629c43c6fc1c361a                                                                                                           0.0s
 => => naming to docker.io/library/my-docker-image                                                                                                                                                     0.0s
❯ docker run -it my-docker-image

root@76e14ea4e756:/app# git pull
remote: Enumerating objects: 27, done.
remote: Counting objects: 100% (27/27), done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 14 (delta 5), reused 14 (delta 5), pack-reused 0
Unpacking objects: 100% (14/14), 2.65 MiB | 3.34 MiB/s, done.
From https://github.com/amalp12/morsel-db
   f3e9d1a..45223f9  demo       -> origin/demo
Updating f3e9d1a..45223f9
Fast-forward
 Tables/Table_3.csv   | 95960 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++--------------------------------------------------------------------------------------
 Tables/Table_4.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 Tables/Table_5.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 Tables/Table_6.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 Tables/Table_7.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 Tables/Table_8.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 Tables/Table_9.csv   | 96000 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------------------
 outputlog_select.csv | 20654 ++++++++++++++++++++++++++++----------
 select_test.sh       |     2 +-
 src/morsel.cpp       |     2 +-
 10 files changed, 351113 insertions(+), 341505 deletions(-)
root@76e14ea4e756:/app# ./compile.sh
-- The CXX compiler identification is GNU 11.4.0
-- The C compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
# Use the official Ubuntu base image
FROM ubuntu:22.04

# Update the package repository and install necessary packages
 RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    ninja-build \
    vim \
    ccache \
    libnuma-dev \
    curl \
    build-essential \
   && rm -rf /var/lib/apt/lists/*

# Clone the repository
RUN git clone https://github.com/hyrise/sql-parser.git

# Set working directory
WORKDIR /sql-parser

# Build dependency in release and debug
RUN make clean && \
    make && \
    make install && \
    mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_release.so && \
    make clean

RUN make clean && \
    make mode=debug && \
    make install && \
    mv /usr/local/lib/libsqlparser.so /usr/local/lib/libsqlparser_debug.so && \
    make clean


# Cleanup
RUN rm -rf /sql-parser

# Set any additional configurations or commands as needed

# Set the working directory inside the container
WORKDIR /app

# Clone your code repository
RUN git clone -b demo --single-branch https://github.com/amalp12/morsel-db.git  .

# Run your tests (replace the command with your actual test command)
# RUN ./run_tests.sh
Dockerfile                                                                                                                                                                                1,1            Top
