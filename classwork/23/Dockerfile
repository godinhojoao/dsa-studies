FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY last-version.c .

# Compile it
RUN gcc -g -o last-version last-version.c

# Default command runs Valgrind
CMD ["valgrind", "./last-version"]
