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

# Start a shell session to show the terminal (you can replace "/bin/bash" with any shell you prefer)
CMD ["/bin/bash"]

