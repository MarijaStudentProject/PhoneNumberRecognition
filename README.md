# 📞  Phone Number Recognition & Contact Manager

## Overview

A C++17 library and Qt6 application for parsing, normalizing, and managing contacts from vCard (.vcf) files, with a fully functional phone dialer UI.<br>
The core library handles phone number normalization across international and local formats, enabling reliable contact matching and deduplication. 
The Qt6 frontend provides a native-feeling contacts browser and dial pad.

### Example

```
+38160123456 ≡ 060123456 (Serbia)
00491701234567 ≡ +491701234567 (Germany)
```


## ⚙️ Core Features

### 📂 vCard Processing

* Parses .vcf files using the ivanenko/vCard library
* Extracts name, phone numbers, email, and address fields
* Normalizes all phone numbers on import using the configured region

### 📱 Phone Number Normalization

* Parses E.164, local, and international prefix formats (+381..., 00381..., 060...)
* Detects country codes from a JSON metadata file (supports countries from EU, ASIA, NA)
* Region-aware parsing — knows which national prefix to strip per country
* Special-case handling (e.g. Italy keeps national prefix)

### 🔍 Contact Matching

* Exact match — compares normalized E.164 values
* Suffix match — fuzzy fallback for partial number overlap 
* Name search — Levenshtein edit distance 
* Number prefix search — live filtering as you type

### 🖥️ Qt Application

* Contacts list with search (name or number)
* Contact detail view with inline editing — changes are held in memory for the session
* Dial pad with contact matching
* Active call screen and incoming call simulation
* Import contacts from .vcf file at startup


## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| Build | CMake ≥ 3.16 |
| UI Framework | Qt6 (Quick, QuickControls2, Multimedia) |
| Testing | Catch2 v3 (auto-fetched via CMake) |
| Static Analysis | clang-tidy |
| Formatting | clang-format |


## 📦 External Dependencies

### 📚 vCard Parser

This project uses the following library:  [https://github.com/ivanenko/vCard](https://github.com/ivanenko/vCard)

#### Installation

```bash
git clone https://github.com/ivanenko/vCard.git
```

Build and install per the repository instructions. The library is linked as vCard via find_library.

### 📄 Json Parser

This project uses the following header only library:  [https://github.com/zserge/jsmn](https://github.com/zserge/jsmn)


## 🧪 Testing

* Unit testing framework: **Catch2 v3**
* Automatically fetched via CMake if not installed locally
* Covers normalization, parsing, and comparison logic


## 🔧 Build Instructions

### Requirements

* C++17 compatible compiler (GCC / Clang)
* CMake ≥ 3.16
* Qt6 (with required modules):
  * QtCore
  * QtGui
  * QtQml
  * QtQuick
  * QtQuickControls2
  * QtMultimedia
  * Qt6::Network


## 🔧 Build Steps
### 0. Install dependencies
   - [Ivanenko vCard library](#-vcard-parser)
   
   - Required Qt modules
      ```bash
      sudo apt install \
       qml6-module-qtquick-controls \
       qml6-module-qtmultimedia
      ```
      
### 1. Clone the repository:
     ```bash
     git clone https://github.com/MarijaStudentProject/PhoneNumberRecognition.git
     ```

### 2. Build from the command line:
    
    ```bash
    cd PhoneNumberRecognition
    mkdir build && cd build
    cmake ..
    make 
    ./ContactsApp
    ```
#### Build tests

   ```bash
   make tests 
   ./tests/tests
   ```

## Or build with Qt Creator

1. Open **Qt Creator**

2. Click **Open Project**

3. Navigate to the cloned folder and select: **CMakeLists.txt**

4. Configure the project (choose Qt6 kit if prompted)

6. Click **Configure Project**

7. Run the application: Click **Run (▶)**

## Configuration
Resources are copied to the `build` folder during installation. There you can change the metadata path as well as the parsing region.
