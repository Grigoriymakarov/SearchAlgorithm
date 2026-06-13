# INFO-H304 — Protein Sequence Alignment with Smith-Waterman
---

## Project Overview

This project implements the **Smith-Waterman algorithm** for local protein sequence alignment, as part of the INFO-H304 course, it also implements parallel computing(whitch significantly incresed the computing time).

The program takes a query protein sequence and aligns it against a large protein database (Swiss-Prot), returning the top 20 most similar proteins ranked by similarity score.

---

## Repository Structure

```
.
├── Makefile
├── README.md
├── ProjetINFO-H304.pdf          # Project specification
├── code/
│   ├── main.cpp                 # Entry point for final version
│   ├── mainprelim.cpp           # Entry point for preliminary version
│   ├── mainopt.cpp              # Entry point for optimized version
│   ├── AlgoSW.cpp / .h          # Smith-Waterman algorithm
│   ├── AlgoSWopt.cpp / .h       # Optimized Smith-Waterman (OpenMP)
│   ├── SeqfileHandler.cpp / .h  # Reads .psq binary sequence file
│   ├── SeqfileHandleropt.cpp/.h # Optimized sequence reader
│   ├── PinfileHandler.cpp / .h  # Reads .pin binary index file
│   ├── PhrfileHandler.cpp / .h  # Reads .phr binary header file
│   └── dict.cpp / .h            # Amino acid encoding dictionary
├── query_testfinal/             # Test query FASTA files
└── solution_testfinal/          # Expected output for test queries
```

> **Important:** Do not commit compiled binaries, database files (`.pin`, `.psq`, `.phr`, `.fasta`), or external tools (Swipe, makeblastdb). Only source files, the Makefile, and the report should be on the repository.

---

## Requirements

- **Compiler:** g++ with C++17 support
- **Standard library only** (no third-party dependencies for `projet` and `projetprelim`)
- **Optional:** OpenMP for the optimized version (`projetopt`)

---

## Build Instructions

```bash
# Build the final version
make projet

# Build the preliminary version
make projetprelim

# Build the optimized version (requires OpenMP)
make projetopt

# Clean all build artifacts
make clean
```

---

## Usage

### Preliminary version — exact sequence match
```bash
./projetprelim query/P00533.fasta database/uniprot_sprot.fasta
```
Output: a single line with the unique ID of the matching protein, e.g.:
```
sp|P00533|EGFR_HUMAN
```

### Final version — Smith-Waterman alignment
```bash
./projet query/P00533.fasta database/uniprot_sprot.fasta blosum/BLOSUM62 11 1
```

Parameters (in order):
| Parameter | Description |
|---|---|
| `query/P00533.fasta` | Query protein in FASTA format |
| `database/uniprot_sprot.fasta` | Protein database (reads `.pin`, `.psq`, `.phr` binary files) |
| `blosum/BLOSUM62` | BLOSUM scoring matrix |
| `11` | Gap open penalty |
| `1` | Gap extension penalty |

Output: top 20 most similar proteins, ranked by score:
```
sp|P00533|EGFR_HUMAN 6525
sp|P55245|EGFR_MACMU 6487
sp|Q01279|EGFR_MOUSE 5957
...
```

To save output to a file:
```bash
./projet query/P00533.fasta database/uniprot_sprot.fasta blosum/BLOSUM62 11 1 > results.txt
```

### Optimized version
```bash
./projetopt query/P00533.fasta database/uniprot_sprot.fasta blosum/BLOSUM62 11 1
```
Uses OpenMP for multi-threaded parallelism.

---

## Setup: Database and Files

Before running the program, you need to prepare the following files:

1. **Download the Swiss-Prot database** from [UniProt](https://ftp.uniprot.org/pub/databases/uniprot/current_release/knowledgebase/complete/uniprot_sprot.fasta.gz), decompress it, and place `uniprot_sprot.fasta` in `database/`.

2. **Convert to BLAST binary format** using `makeblastdb`:
   ```bash
   ./makeblastdb -in database/uniprot_sprot.fasta -dbtype prot -blastdb_version 4
   ```
   This generates `.pin`, `.psq`, and `.phr` files in `database/`.

3. **Download a query protein** from [UniProt](https://www.uniprot.org/) in FASTA format and place it in `query/`.

4. **Download BLOSUM62** from [NCBI](https://ftp.ncbi.nih.gov/blast/matrices/) and place it in `blosum/`.

---

## Testing

Test scripts are provided at the root of the repository:

```bash
# Test preliminary version
./testprelim

# Test final version
./testfinal
```

Test queries are in `query_testfinal/` and expected outputs are in `solution_testfinal/`.

---

## References

- Smith & Waterman (1981) — *Identification of common molecular subsequences*
- Gotoh (1982) — *An improved algorithm for matching biological sequences*
- Rognes (2011) — *Faster Smith-Waterman database searches with inter-sequence SIMD parallelisation*
- Farrar (2010) — *NCBI BLAST Database Format*
