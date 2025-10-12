FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
  apt-get install -y build-essential valgrind

# Create working directory
WORKDIR /app

# Copy your source file into the container
COPY hashtable-oplexicon.c .
COPY OpLexicon.txt .
COPY input.txt .

# Compile it
RUN gcc -g -o hashtable-oplexicon hashtable-oplexicon.c

# Default command runs Valgrind
CMD ["sh", "-c", "valgrind ./hashtable-oplexicon < input.txt --track-origins=yes"]

