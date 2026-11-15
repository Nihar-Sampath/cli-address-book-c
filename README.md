# CLI Address Book in C

A terminal-based contact manager written in C using structs, pointers, custom string validation, and persistent CSV file handling.

## Features
* **Full CRUD Operations:** Create, search, edit, delete, and list contacts directly in the terminal.
* **Custom Input Validation (No Regex):**
  * **Name:** Accepts letters and spaces only.
  * **Phone:** Requires exactly 10 digits and preserves leading zeros by storing numbers as strings.
  * **Email:** Enforces lowercase letters, a single `@`, valid dot placement, and no trailing `.` or `@`.
* **Duplicate Prevention:** Blocks duplicate phone numbers and email addresses before saving.
* **Partial Search & Selection:** Searches across names, phone numbers, or emails, and lets you select the exact match when multiple contacts appear.
* **Persistent CSV Storage:** Loads contacts from `AddressBook.csv` on startup and saves changes only when you select **Save and Exit**.

## Project Structure
* `main.c` — Runs the interactive `do...while` menu loop.
* `contact.c` / `contact.h` — Defines `Contact` and `AddressBook` structs, input validation, sorting (`strcasecmp`), and CRUD logic.
* `file.c` / `file.h` — Handles reading from and writing to `AddressBook.csv`.

## How to Compile & Run
Make sure you have `gcc` installed (Linux, macOS, or Windows with MinGW/WSL):

```bash
gcc main.c contact.c file.c -o addressbook
./addressbook
```

## Program Flow & Architecture
## Program Flow & Architecture

![Overview](assets/slide1.png)
![Main Program Flow](assets/slide2.png)
![Creating a Contact](assets/slide3.png)
![Search, Edit, and Delete Flow](assets/slide4.png)


## Future Upgrades
* Replace the fixed 100-contact array with dynamic memory allocation (`malloc` and `realloc`).
* Migrate storage from CSV to SQLite for faster querying.
* Support punctuation in names (e.g., `"K. Nihar"` or `"O'Brien"`).
