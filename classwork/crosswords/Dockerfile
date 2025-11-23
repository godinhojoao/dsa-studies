FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY crosswords.c .
COPY words.txt .
COPY input.txt .

# Compile it
RUN gcc -g -o crosswords crosswords.c

# Default command runs Valgrind
CMD ["sh", "-c", "valgrind ./crosswords < input.txt --track-origins=yes"]

