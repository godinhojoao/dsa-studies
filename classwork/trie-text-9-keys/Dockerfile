FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY trie-text-9-keys.c .
COPY dicionario.txt .
COPY input.txt .

# Compile it
RUN gcc -g -o trie-text-9-keys trie-text-9-keys.c

# Default command runs Valgrind
CMD ["sh", "-c", "valgrind ./trie-text-9-keys < input.txt --track-origins=yes"]

