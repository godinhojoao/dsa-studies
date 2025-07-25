FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY valgrind-test.c .
COPY input.txt .

# Compile it
RUN gcc -g -o valgrind-test valgrind-test.c

# Default command runs Valgrind
CMD ["sh", "-c", "valgrind ./valgrind-test < input.txt"]

