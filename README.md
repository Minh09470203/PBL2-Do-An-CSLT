# SHOE STORE MANAGEMENT - PBL2 (Basic Programming Project)
---

Description:  
A command-line shoe store management application written in C++.  
This project combines knowledge from Object-Oriented Programming and Data Structures.  
The application provides functions to manage customers, staff, products, suppliers, categories, invoices, login/change password and basic statistics.  
Data is stored and loaded from text files in the `DataFile/` directory.

## Main features
- Login for Staff / Customer
- Role-based menus: Manager has full features, regular staff have limited permissions
- Customer management: view, add, search, edit information (customers can edit their own info)
- Staff management: view, add, (delete)
- Product management: view, add, delete, search
- Category / Supplier management: view, add
- Invoice management: view, create invoices, view invoice details
- In-memory runtime storage using MyVector and lookup using MyMap
- All data persists in text files (loaded on startup; saved when changes occur)

---

## Project structure (overview)
- `main.cpp` — entry point, main menu
- `Models/` — object classes (Customer, Staff, Product, Invoice, ...)
- `DAO/` — Data Access Objects, responsible for file IO
- `DataFile/` — contains data files (Accounts.txt, Product.txt, ...)
- `AppSession/` — SessionManager (singleton) that manages the current session and DAOs
- `DataStructure/` — MyVector, MyMap 

## Sample data
Sample data files are included in `DataFile/`. Some example accounts (from `Accounts.txt`):
- Staff (Manager):
  - Username: `NV001`  Password: `staff123`  → linked to Staff ID `S001` (Manager)
- Staff (other):
  - Username: `NV002`  Password: `staff123`  (S002)
- Customer:
  - Username: `KH001`  Password: `123456`  → linked to Customer ID `C001`

## Build & run (Linux / macOS / Windows WSL)
Requirements: C++ compiler (g++ 7+), make (optional). 

Build (from repo root):
- Quick way (compile all `.cpp` files in the repo):
  ```
  g++ -o main.exe main.cpp Models/Person.cpp Models/Customer.cpp Models/Staff.cpp Models/Product.cpp Models/Category.cpp Models/Supplier.cpp Models/Invoice.cpp Models/Account.cpp
  AppSession/SessionManager.cpp DAO/IDAO.cpp DAO/AccountDAO.cpp DAO/CustomerDAO.cpp DAO/StaffDAO.cpp DAO/ProductDAO.cpp DAO/CategoryDAO.cpp DAO/SupplierDAO.cpp DAO/InvoiceDAO.cpp
  ```

- After building:
  ```
  ./main.exe
  ```

The program will load data from the `DataFile/` folder (text files). If a file is missing, the program will output "Cannot open file ...".

---

## Short usage guide (flow)
1. Run the program → main menu
2. Choose `1. Login` → enter `username` and `password`
3. If logged in as Staff:
   - Manager: will see the full menu (Customer Management, Product Management, Invoice Management, Category, Supplier, Statistics, Change Password)
   - Regular staff: limited menu (View customers/products, Create invoice, Change password)
4. If logged in as Customer:
   - View products, search products, view own invoices, update personal information, change password
5. Perform operations from the menus; after changes, the application saves data back to the corresponding files (handled by DAOs).

---

## License & Contact
- This project is a coursework assignment.
---
