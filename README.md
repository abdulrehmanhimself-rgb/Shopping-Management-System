# Shopping Management System

A simple console-based **inventory and sales management program** written in **C++**.  
It allows managing products across different categories (Electronics, Grocery, Medicines, Sports, etc.), tracking stock quantities, prices, adding/updating items, searching, and simulating sales — with all data saved persistently in a text file.

## Features

- View all products with category, quantity and price
- Add new products
- Update existing product information
- Search products by name or category
- Sell items (automatically reduces stock quantity)
- Data is saved/loaded from `Inventory.txt` file
- Basic input validation and user-friendly menu
- Receipt saved after every checkout in 'Receipt.txt' file
- User cannot Logout until cart is empty

## Technologies / Concepts Used

- C++ (console application)
- File handling (`fstream` – reading/writing text file)
- String parsing (splitting `|` separated values)
- Vectors or arrays to store products in memory
- Switch-case / functions for menu navigation
- Basic error checking (invalid input, out-of-stock, etc.)
- Input validation

## How to Run (Visual Studio)

1. Open the project in **Visual Studio** (2019 / 2022 / 2025 recommended)
2. Make sure `Inventory.txt` is in the same folder as your `.cpp` file (or in the project directory)
3. Set configuration to **Release** (recommended) or **Debug**
4. Build the solution → `Ctrl + Shift + B`
5. Run → `Ctrl + F5` (Start Without Debugging)

After building, the executable (`.exe`) will appear in:
- `x64\Release\` folder (if using 64-bit)
- or `Debug\` / `Release\` depending on your settings

**Important:** Keep `Inventory.txt` in the **same folder as the .exe** so the program can read/write it.

## Login Information

The program includes a simple **admin mode** with hardcoded credentials (for demo purposes only – not secure for real apps).

- Username: `ar`
- Password: `ar123`

After logging in as admin, you get extra options like:
- Add new products
- Update/delete items
- Other admin functions

## Sample Output Preview
<img width="1346" height="686" alt="Screenshot (174)" src="https://github.com/user-attachments/assets/3729124c-a3a8-4503-82db-c3598e80497a" />
<img width="1352" height="688" alt="Screenshot (175)" src="https://github.com/user-attachments/assets/b872577b-a5eb-4b67-b50a-f08e88489b35" />
<img width="1350" height="678" alt="Screenshot (176)" src="https://github.com/user-attachments/assets/afe54606-e3d6-49ac-8041-82b0da83a604" />
<img width="1348" height="686" alt="Screenshot (177)" src="https://github.com/user-attachments/assets/6d4f52e0-7401-4cc5-aea5-8138c3a59cf9" />


## Project Files
- main.cpp
- Inventory.txt
