# Blockchain From Scratch in C

A simple educational implementation of blockchain fundamentals using the C programming language and OpenSSL.

This project explores how blockchains work internally, from cryptographic hashing to consensus mechanisms, without relying on blockchain frameworks.


## Project Structure

```text
Blockchain-From-Scratch-in-C/
│
├── blockchain/
│   ├── blockchain.c
│   ├── merkle-tree.c
│   ├── proof-of-stake.c
│   └── proof-of-work.c
│
├── README.md
└── .gitignore
```

## Features

### 1. Blockchain Implementation
**File:** `blockchain.c`

A basic blockchain implementation integrating block creation, hash linking, and consensus mechanisms.

- Block and Blockchain data structures.
- SHA-256 hash calculation.
- Block linking through previous hashes.
- Proof of Work and Proof of Stake.
- Blockchain integrity verification.

### 2. Merkle Tree
**File:** `merkle-tree.c`

Implementation of a Merkle Tree to ensure data integrity.

- SHA-256 hashing of individual data items.
- Recursive hash combination.
- Handling an odd number of elements.
- Merkle Root generation.

### 3. Proof of Work (PoW)
**File:** `proof-of-work.c`

Simulation of the Proof of Work consensus mechanism.

- Nonce generation.
- SHA-256 hash calculation.
- Mining process.
- Configurable mining difficulty.
- Execution time measurement.

### 4. Proof of Stake (PoS)
**File:** `proof-of-stake.c`

Simulation of a Proof of Stake consensus mechanism.

- Validator representation.
- Stake-based validator selection.
- Random selection proportional to stake.
- PoW and PoS execution time comparison.

## Technologies

- **Programming Language:** C
- **Cryptographic Library:** OpenSSL
- **Hashing Algorithm:** SHA-256
- **Compiler:** GCC

## Requirements

- GCC compiler
- OpenSSL development libraries

## Compilation and Execution

Compile each source file independently:

**Blockchain**
```bash
gcc blockchain/blockchain.c -o blockchain -lcrypto
```

**Merkle Tree**
```bash
gcc blockchain/merkle-tree.c -o merkle-tree -lcrypto
```

**Proof of Work**
```bash
gcc blockchain/proof-of-work.c -o proof-of-work -lcrypto
```

**Proof of Stake**
```bash
gcc blockchain/proof-of-stake.c -o proof-of-stake -lcrypto
```

Run an executable:

```bash
./blockchain
```

On Windows:

```bash
blockchain.exe
```


## Testing

The implementation can be tested by:

- Creating blocks using PoW and PoS.
- Modifying block data and checking hash integrity.
- Modifying previous hashes and checking chain consistency.
- Observing the effect of mining difficulty.
- Comparing PoW and PoS execution times.

## Learning Objectives

Through this project, I explored:

- Cryptographic hashing with SHA-256.
- Merkle Trees and data integrity.
- Blockchain structure and hash linking.
- Proof of Work and Proof of Stake.
- Nonce generation and mining.
- Basic blockchain validation.

## Limitations

This is an educational simulation, not a production-ready blockchain.

The implementation does not yet include:

- Digital signatures and transaction authentication.
- Distributed networking and peer-to-peer communication.
- Persistent blockchain storage.
- A complete economic incentive and slashing mechanism.
- Production-grade consensus security.

## Future Improvements

- Integrate Merkle Trees directly into blocks containing multiple transactions.
- Implement blockchain attack simulations.
- Explore chain reorganization and double spending.
- Add malicious-node detection and mitigation.

---
Developed as part of a university Blockchain practical workshop.

**Language:** C  
**Academic Year:** 2026–2027

**Author:** [Nour El Houda OUARAGHOUANE](https://github.com/nourmiyy03)

*Learning blockchain by building it from scratch.*
---
