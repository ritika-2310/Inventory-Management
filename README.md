# Inventory Management System (C++ with cJSON)
<br>
📌 Overview
This project is a console-based Inventory Management System built in C++ using the cJSON library for JSON file handling. It is designed for ease of use so that even less technically skilled people can operate it with minimal training. The system provides a menu-driven interface with clear, color-coded instructions for intuitive navigation. The program stores all inventory data in a product.json file so that the inventory is persistent even after closing the application.
<br>
🎯 Purpose
<br>
This project was developed with simplicity and accessibility in mind:
<br>
- Suitable for small shops, local vendors, NGOs, rural markets, or low-tech workplaces.
- Minimal technical jargon — simple prompts for adding, viewing, searching, and updating products.
- Color-coded terminal messages to guide the user clearly (e.g., green for success, red for errors).
- No complex commands — fully menu-driven.
<br>
✨ Features
<br>
1. Add New Item – Create a new inventory record with auto-generated unique ID.
2. View All Items – Display all stored items with details (ID, Name, Quantity, Price).
3. Search Item – Search by ID or by Name.
4. Update Item – Modify product details by ID or by Name.
5. Display Out-of-Stock Items – List all items with quantity = 0.
6. Exit Program – Gracefully close the application.
<br>
🗂 Data Storage Format
<br>
All inventory data is stored in a JSON file (product.json) in the following structure:
<br>
[
    <br>
    {
        <br>
        "id": 1,
        <br>
        "name": "Pen",
        <br>
        "price": 10.5,
        <br>
        "quantity": 50
        <br>
    },
    <br>
    {
        <br>
        "id": 2,
        <br>
        "name": "Notebook",
        <br>
        "price": 45,
        <br>
        "quantity": 0
        <br>
    }
    <br>
]
<br>
- The file is automatically created if it doesn’t exist.
- Data is loaded at runtime and updated whenever you make changes.
<br>
🛠 Installation & Compilation
<br>
Prerequisites:
<br>
- A C++ compiler (e.g., g++).
- The cJSON library files (cJSON.c and cJSON.h) in the same directory.
<br>
Compilation Command:
<br>
g++ main.cpp cJSON.c -o main
<br>
Run Command (Windows PowerShell / CMD):
<br>
.\main
<br>
Run Command (Linux/Mac):
<br>
./main
<br>
🖥 Usage Flow
<br>
When you run the program:
<br>
1. The main menu is displayed with options from 1 to 6.
2. You enter the number corresponding to your action.
3. The program processes your choice, shows relevant results or prompts, then asks if you want to continue (y or n).
<br>
📜 Function-by-Function Documentation
<br>
1. menu()
<br>
- Displays the main menu with ANSI color codes.
- Shows available operations: Add, View, Search, Update, Display Out-of-Stock, Exit.
<br>
2. read()
<br>
- Opens product.json and reads its contents.
- Parses JSON data into a cJSON object.
- Handles cases where:
  - File does not exist.
  - File is empty.
  - JSON is malformed.
- Returns a pointer to the parsed cJSON object for use in other functions.
<br>
3. generateId()
<br>
- Reads the existing JSON file.
- Finds the highest current ID in the list.
- Returns maxId + 1 to ensure unique IDs for new items.
<br>
4. saveToFile(item newitem)
<br>
- Reads the existing JSON array (or creates one if file is empty).
- Converts the item struct into a JSON object.
- Appends the new object to the JSON array.
- Writes the updated array back to product.json.
- Frees memory after saving.
<br>
5. additem(vector<item> &items)
<br>
- Prompts user for product name, quantity, price.
- Automatically assigns a unique ID via generateId().
- Saves the new item to the vector and file via saveToFile().
- Shows a success message.
<br>
6. display()
<br>
- Reads JSON data.
- Displays all items in a formatted and color-coded way.
- Safely checks if data fields exist before accessing.
<br>
7. menu_search()
<br>
- Displays search options:
  1. Search by ID.
  2. Search by Name.
<br>
8. searchById(cJSON *root)
<br>
- Asks user for an ID to search.
- Loops through JSON array to find a match.
- Displays item details if found, otherwise returns false.
<br>
9. searchByName(cJSON *root)
<br>
- Asks user for a Name to search.
- Performs exact match search.
- Displays item details if found.
<br>
10. search()
<br>
- Calls menu_search().
- Reads JSON data and delegates to searchById() or searchByName() depending on user choice.
- Handles case where no item is found.
<br>
11. menu_update()
<br>
- Displays update options:
  1. Update by ID.
  2. Update by Name.
<br>
12. updateById(cJSON *root)
<br>
- Prompts for the ID of the item to update.
- Asks for new Name, Quantity, and Price.
- Replaces existing JSON object values.
- Saves updated JSON back to file.
<br>
13. updateByName(cJSON *root)
<br>
- Prompts for the Name of the item to update.
- Asks for new Quantity and Price.
- Updates matching record in the JSON file.
<br>
14. update()
<br>
- Calls menu_update().
- Reads data and executes the chosen update function.
- Shows success or error messages.
<br>
15. display_outofstock()
<br>
- Reads JSON data.
- Loops through all items and checks if quantity == 0.
- Displays out-of-stock items in a dedicated section.
<br>
16. main()
<br>
- Contains the main program loop.
- Displays menu, takes user choice, and calls relevant functions.
- Asks the user if they want to continue (y to loop again, n to exit).
<br>
🛡 Error Handling
<br>
- File Not Found: Gracefully informs the user and skips the operation.
- Empty File: Creates a new JSON array if file is empty.
- Invalid Input: Shows error messages for wrong menu choices.
- Null JSON Objects: Uses safety checks before accessing object fields.
<br>
📌 Design Choices
<br>
- cJSON Library: Lightweight, fast JSON parsing.
- Vector for Temporary Storage: Keeps added items in memory for quick access during runtime.
- Menu-Driven System: User-friendly for low-tech users.
- Color-Coded Output: Improves readability and user experience.
<br>
📝 Example Run
<br>
======================= Welcome!!!! ===========================
<br>
================= Inventory Management System =================
<br>
1. Add New Item
    <br>
2. View All Item
     <br>
3. Search Item
     <br>
4. Update Item
     <br>
5. Display Out Of Stocks Item
     <br>
6. Exit
<br>
enter your choice : 1
<br>
enter name of the product : Pen
<br>
enter quantity of the product : 50
<br>
enter price of the product : 10.5
<br>
items added successfully
<br>
-----------------------------
<br>
enter y or n if want to continue : y
