FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY agenda-buffer.c .
COPY input.txt .

# Compile it
RUN gcc -g -o agenda-buffer agenda-buffer.c

# Default command runs Valgrind
CMD ["sh", "-c", "valgrind ./agenda-buffer < input.txt"]

