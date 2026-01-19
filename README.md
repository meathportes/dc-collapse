# Structured Divide-and-Conquer Collapse for Barrett Inversion

This repository contains a reference implementation of the **Structured Divide-and-Conquer Collapse algorithm for Barrett inversion**, as described in the accompanying paper.

The core contribution is a divide-and-conquer reciprocal algorithm that:

* uses **only one recursive descent per level**,
* requires **five tight half-width multiplications per level**,
* supports **odd bit sizes**,
* uses only **constant bounded corrections**, and
* runs in **Θ(M(n)) time** with a smaller leading constant than classical D&C reciprocal schemes.

The implementation here is intended as:

* a **correctness reference**,
* a **testbed for experimentation**, and
* a foundation for **big-integer and cryptographic integration**.

It is written in C and focuses on machine-word regimes, where all arithmetic widths and overflows are explicit and controlled.

---

## Repository layout


```
config.h
dc-collapse.pdf
inv_machine_word.c
fprintu128.h
inv_bits.h
inv_bits_test.c
inv_machine_word.c
inv_machine_word.h
inv_test.c
LICENSE
Makefile
README.md
```

---

## What this code does

Given an odd integer ( d \in [2^{b-1}, 2^b) ), the core routine computes:

[
(q, r) = \left(\left\lfloor \frac{2^{2b}}{d} \right\rfloor,; 2^{2b} \bmod d \right)
]

using the collapsed divide-and-conquer algorithm.

This produces a Barrett reciprocal and remainder suitable for fast division, reduction, and modular arithmetic.

---

## Building

The code is standard C (C99 or later).

Typical build:

```
make
```

---

## Running tests

The repository includes both deterministic and randomized tests. Bit sizes greater than 32 are tested with random denominators. Otherwise, all denominators for each specified bit size are tested exhaustively.

Typical usage:

```
./inv_test [BIT SIZES TO TEST]
```

To test sentinels exhaustively, use:
```
./inv_bits_test
```

Tests include:

* exhaustive verification at small bit-widths,
* randomized fuzzing at larger widths,
* cross-checking against widened reference arithmetic.

A successful run produces no output or a short confirmation message.

---

## Notes on correctness

The implementation explicitly tracks and controls:

* word widths,
* carry/borrow propagation,
* truncated products,
* and bounded correction loops.

All critical invariants from the paper are instrumented in the test harness.

This is **not** written as a production big-integer library; clarity and structural correspondence to the algorithm take precedence.

---

## Relationship to the paper

This code implements the algorithms described in:

> *Structured Divide-and-Conquer Collapse for Barrett Inversion*
> Heath P. Schultz, 2026.

The paper contains:

* derivation of the collapse,
* complexity analysis,
* correction bounds,
* and algorithmic structure.

This repository provides the working reference core.

---

## Intended audience

This code is aimed at readers interested in:

* fast division and reciprocal computation
* Barrett and Montgomery-style reduction
* high-performance big-integer arithmetic
* cryptographic arithmetic kernels

It assumes comfort with:

* multi-precision arithmetic
* truncated multiplication
* word-level overflow behavior

---

## License

```
Public domain / CC0
```

---

## Contact

Heath P. Schultz
meathportes@gmail.com

---
