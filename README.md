# 🚆 Train Management System

![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20WSL-lightgrey?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)
![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square)

A command-line based **Train Reservation and Management System** written in C, built using structured file handling as a persistent storage backend. It supports searching for trains between stations, booking tickets for multiple passengers, and cancelling existing reservations.

---

## 📋 Table of Contents

- [Features](#-features)
- [Project Structure](#-project-structure)
- [Prerequisites](#-prerequisites)
- [Installation & Setup](#-installation--setup)
- [Data File Format](#-data-file-format)
- [Usage](#-usage)
- [Implementation Details](#-implementation-details)
- [Known Limitations](#-known-limitations)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## ✨ Features

- 🔍 **Search Trains** — Find available trains between any two stations (case-insensitive)
- 🎟️ **Book Tickets** — Book multiple tickets in one transaction with per-passenger details
- ❌ **Cancel Tickets** — Cancel a specific passenger's booking by name and train number
- 💾 **File-Based Persistence** — All data stored in plain text files, no database required
- 🔄 **Real-Time Seat Update** — Available seats are updated immediately after each booking or cancellation

---

## 📁 Project Structure

```
train-management-system/
│
├── main.c               # Main source file (all logic)
├── train_data.txt       # Master train database (manually populated)
├── bookings.txt         # Auto-generated booking records
├── cancelled.txt        # Auto-generated cancellation log
├── temp.txt             # Temporary file used during seat update (auto-managed)
├── temp_book.txt        # Temporary file used during cancellation (auto-managed)
├── .gitignore           # Git ignore rules
└── README.md            # Project documentation
```

---

## 🔧 Prerequisites

- GCC compiler (`gcc`)
- Linux / Windows (with WSL or MinGW) / macOS terminal
- Basic knowledge of C compilation

---

## 🚀 Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/train-management-system.git
cd train-management-system
```

### 2. Create the Train Data File

Before running the program, you must create `train_data.txt` in the project root with the following format:

```
<train_no> <from> <to> <name> <seats> <fare>
```

**Example `train_data.txt`:**

```
12345 Bengaluru Chennai RajdhaniExpress 100 450
67890 Bengaluru Mumbai Deccan_Queen 80 850
11223 Chennai Mumbai CoastalExpress 60 700
```

> ⚠️ **Important:** Station names and train names must NOT contain spaces. Use underscores (e.g., `New_Delhi`).

### 3. Compile

```bash
gcc main.c -o train
```

### 4. Run

```bash
./train
```

On Windows (CMD or PowerShell with MinGW):
```bash
gcc main.c -o train.exe
train.exe
```

---

## 📄 Data File Format

### `train_data.txt` (Master Train Database)
| Field      | Type   | Description                        |
|------------|--------|------------------------------------|
| train_no   | int    | Unique train number                |
| from       | string | Source station (no spaces)         |
| to         | string | Destination station (no spaces)    |
| name       | string | Train name (no spaces)             |
| seats      | int    | Available seats                    |
| fare       | int    | Fare per ticket (in ₹)             |

### `bookings.txt` (Auto-generated)
```
<train_no> <from> <to> <train_name> <fare> <date> <passenger_name> <gender> <age>
```

### `cancelled.txt` (Auto-generated)
Same format as `bookings.txt` — logs all cancelled records.

---

## 🖥️ Usage

When you run the program, you'll see:

```
--- Train Management System ---
1.Search
2.Book
3.Cancel
4.Exit
Enter choice:
```

### Option 1 — Search Trains
```
Enter choice: 1
From: Bengaluru
To: Chennai

Available Trains:
Train No:12345 | Name:RajdhaniExpress | Seats:100 | Fare:450
```

### Option 2 — Book Ticket
```
Enter choice: 2
Train No: 12345
Date: 25-12-2025
Number of Tickets: 2

Passenger 1
Name: Shreevathsa
Gender (M/F): M
Age: 21

Passenger 2
Name: Koushik
Gender (M/F): M
Age: 22

 BOOKING SUCCESSFUL
Train: RajdhaniExpress (12345)
Tickets: 2
Date: 25-12-2025
Passenger 1: Shreevathsa | M | 21
Passenger 2: Koushik | M | 22
Fare per ticket: 450
 Total Amount: 900
```

### Option 3 — Cancel Ticket
```
Enter choice: 3
Train No: 12345
Enter Passenger Name: Shreevathsa

 Ticket cancelled for Shreevathsa
```

---

## ⚙️ Implementation Details

### Core Data Structures

```c
struct Train {
    int train_no;
    char from[50], to[50];
    char name[50];
    int seats;
    int fare;
};

struct Passenger {
    char name[50];
    char gender[10];
    int age;
};
```

### File I/O Strategy

The system uses a **"temp file replace"** pattern for safe record updates:

1. Open original file for reading and a `temp` file for writing
2. Copy all records, modifying the target record in-place
3. Delete the original file
4. Rename `temp` to the original filename

This ensures no data is lost if the program exits mid-operation.

### Search Logic

Station names are normalized to lowercase before comparison using a custom `toLower()` function, making the search **case-insensitive**.

```c
void toLower(char str[]) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}
```

### Booking Flow

```
User inputs train_no + date
    → Input passenger details (up to 10)
        → Scan train_data.txt for matching train_no with sufficient seats
            → Deduct seats, write to bookings.txt
                → Rewrite train_data.txt via temp file
```

### Cancellation Flow

```
User inputs train_no + passenger_name
    → Scan bookings.txt for first matching record
        → Write matched record to cancelled.txt
            → Skip it when rewriting bookings.txt via temp file
```

---

## ⚠️ Known Limitations

- Names and station names **cannot contain spaces** (scanf limitation)
- Cancellation matches only the **first occurrence** of a passenger name per train
- No login or authentication system
- No PNR (Passenger Name Record) number generation
- Max 10 passengers per booking transaction

---

## 🔮 Future Improvements

- [ ] Add PNR number generation using `rand()` or timestamp-based IDs
- [ ] Support multi-word names using `fgets()` instead of `scanf()`
- [ ] Admin panel to add/remove trains from the database
- [ ] Input validation (invalid dates, negative ticket count, etc.)
- [ ] Refund calculation on cancellation
- [ ] Waitlist functionality when seats are 0

---

## 📜 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

> Built as part of a C programming project | File Handling & Structures
