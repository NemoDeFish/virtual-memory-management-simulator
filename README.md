# Virtual Memory Management Simulator

## Project Overview

This project simulates a **Virtual Memory Manager** for a single process. It handles translation of logical addresses to physical addresses using a **Page Table** and **Translation Lookaside Buffer (TLB)**, and includes a **FIFO page replacement algorithm** when memory is full.

The simulator processes read-only logical memory accesses from an input file and outputs a detailed execution transcript for each memory access.

## Features by Task

### Task 1: Logical Address Parser

* Extracts **page number** and **offset** from each 32-bit logical address (using only the rightmost 22 bits).
* Outputs address parsing results.

### Task 2: Page Table & Frame Allocation

* Implements a **page table** with 1024 entries.
* Allocates pages to frames (max 256) in increasing order.
* Assumes no more than 256 unique pages are accessed (no page replacement required).
* Outputs physical address, frame number, and page fault information.

### Task 3: FIFO Page Replacement

* Implements **FIFO** strategy when all frames are used.
* Evicts oldest page and reuses frame.
* Tracks and logs **evictions** and **frame reassignments**.

### Task 4: TLB with LRU Replacement

* Uses a 32-entry **Translation Lookaside Buffer (TLB)**.
* Implements **LRU replacement** for TLB updates.
* Flushes TLB entries on page eviction.
* Reports **TLB hits**, **misses**, **updates**, and **flushes**.

## System Specifications

| Component             | Specification   |
| --------------------- | --------------- |
| Logical Address Space | 4MB (2²² bytes) |
| Physical Memory Space | 1MB (2²⁰ bytes) |
| Page/Frame Size       | 4KB (2¹² bytes) |
| Logical Pages         | 1024            |
| Physical Frames       | 256             |
| TLB Entries           | 32              |

## Usage

### Build

```bash
make
```

Compiles and generates the `translate` executable in the root directory.

### Run

```bash
./translate -f <input_file> -t <task>
```

#### Parameters:

* `-f <filename>`: Path to input file containing 32-bit logical addresses (one per line).
* `-t <task>`: One of `{task1, task2, task3, task4}`.

**Examples:**

```bash
./translate -f addresses.txt -t task1
./translate -t task3 -f data/mem_trace.dat
```

## Input Format

The input file must contain one 32-bit unsigned integer (decimal) per line. These represent memory addresses accessed by the process. For example:

```
30023
10001
20007
```

## Output Format

Output is printed to **standard output** and follows a strict, task-specific format.

### Task 1:

```
logical-address=30023,page-number=7,offset=1351
```

### Task 2:

```
logical-address=30023,page-number=7,offset=1351
page-number=7,page-fault=1,frame-number=0,physical-address=1351
```

### Task 3 (with page eviction):

```
logical-address=90048,page-number=21,offset=4032
evicted-page=7,freed-frame=0
page-number=21,page-fault=1,frame-number=0,physical-address=4032
```

### Task 4 (with TLB operations):

```
logical-address=30023,page-number=7,offset=1351
tlb-hit=0,page-number=7,frame=none,physical-address=none
tlb-remove=none,tlb-add=7
page-number=7,page-fault=1,frame-number=0,physical-address=1351
```

### Output Ordering

Output follows this strict order:

1. `logical-address=...`
2. `tlb-hit=...` (Task 4)
3. `evicted-page=...` (Task 3)
4. `tlb-flush=...` (Task 4)
5. `tlb-remove=...`, `tlb-add=...` (Task 4)
6. `page-number=...,page-fault=...,frame-number=...,physical-address=...` (Task 2)

Empty input files produce **no output**.

## Build Requirements

* Include a `Makefile` that compiles all necessary source files into an executable named `translate`.
* The `translate` executable should reside in the root directory after `make` is run.

## Repository Structure

```
/ (root)
├── cases/             # Test case input and response files
├── main.c             # Program entry point
├── Makefile           # Build script
├── .clang-format      # Code formatting rules
├── .gitignore         # Ignored files
├── bit_utils.*        # Bit manipulation utilities
├── file_io.*          # File reading/writing helpers
├── linked_list.*      # Custom linked list implementation
├── constants.h        # Global constants and macros
├── task1.* - task4.*  # Task-specific modules
└── README.md
```

## Example Test Cases

You can run test cases using `diff` to compare outputs:

### Task 1

```bash
./translate -f cases/task1/spec.txt -t task1 | diff - cases/task1/spec.out
./translate -f cases/task1/spec-mask.txt -t task1 | diff - cases/task1/spec-mask.out
./translate -f cases/task1/bound.txt -t task1 | diff - cases/task1/bound.out
```

### Task 2

```bash
./translate -f cases/task1/spec.txt -t task2 | diff - cases/task2/spec.out
./translate -f cases/task1/spec-mask.txt -t task2 | diff - cases/task2/spec-mask.out
./translate -f cases/task1/bound.txt -t task2 | diff - cases/task2/bound.out
./translate -f cases/task2/same.txt -t task2 | diff - cases/task2/same.out
./translate -f cases/task2/3page.txt -t task2 | diff - cases/task2/3page.out
./translate -f cases/task2/255.txt -t task2 | diff - cases/task2/255.out
./translate -f cases/task2/255-more.txt -t task2 | diff - cases/task2/255-more.out
```

### Task 3

```bash
./translate -f cases/task3/spec2.txt -t task3 | diff - cases/task3/spec2.out
./translate -f cases/task3/512.txt -t task3 | diff - cases/task3/512.out
./translate -f cases/task3/loop.txt -t task3 | diff - cases/task3/loop.out
```

### Task 4

```bash
./translate -f cases/task4/basic.txt -t task4 | diff - cases/task4/basic.out
./translate -f cases/task2/3page.txt -t task4 | diff - cases/task4/3page.out
./translate -f cases/task4/lru.txt -t task4 | diff - cases/task4/lru.out
./translate -f cases/task3/spec2.txt -t task4 | diff - cases/task4/spec2.out
```

---

## Code Style

Format using:

```bash
clang-format -i *.c *.h
```
