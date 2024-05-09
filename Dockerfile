# Use a base image
FROM ubuntu:latest

# Install required packages
RUN apt-get update && \
    apt-get install -y \
    git \
    build-essential \
    cmake \
    ctest \
    ccache \
    clang \

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


#Copy codebase
WORKDIR /morsel-db
COPY . .




# Cleanup
RUN rm -rf /sql-parser

# Set any additional configurations or commands as needed
