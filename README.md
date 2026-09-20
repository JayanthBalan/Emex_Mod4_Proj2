# Inverted Search

An inverted index implementation in C for storing and searching words across multiple text files.

## Features

- Hash table with 26 alphabet-based buckets.
- Stores each word along with the files containing it.
- Tracks word occurrence count for each file.
- Supports indexing multiple `.txt` files.
- Re-indexes a file by removing its old entries before indexing the current contents.
- Saves the database to `target-files/archive.dat`.
- Loads the saved database when the program starts.
- Supports case-insensitive search.
- Interactive database display and search.

## Build

```bash
make
```

To clean the generated object files and executable:

```bash
make clean
```

The project uses:

```text
gcc -Wall -Wextra -Werror -std=c11 -O3 -g
```

## Run

Index a single file:

```bash
./inverted-search target-files/file1.txt
```

Index all `.txt` files in `target-files`:

```bash
./inverted-search target-files/*.txt
```

After indexing, the program provides:

```text
1. Display Database
2. Search Database
3. Close Database
```

The database is saved when the program is closed after indexing.

## Test Script

`run-proj.sh` can be used for quick testing.

```bash
./run-proj.sh file1.txt
```

Index every `.txt` file:

```bash
./run-proj.sh all
```

Run the default search test:

```bash
./run-proj.sh
```

## Project Structure

```text
.
├── include/
│   ├── inverted-search.h
│   └── ll-ops.h
├── src/
│   ├── main.c
│   ├── db-ops.c
│   └── ll-ops.c
├── target-files/
│   ├── *.txt
│   └── archive.dat
├── Makefile
└── run-proj.sh
```

## Database

The runtime database uses a hash table where each bucket contains a linked list of words. Each word has a linked list of files and the number of occurrences in each file.

`archive.dat` stores the logical database contents so the index can be reconstructed on the next run.
