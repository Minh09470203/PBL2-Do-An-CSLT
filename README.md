# Shoe Store Management Application 👟

A comprehensive C++ console-based application for managing a shoe store business with role-based access control for administrators, staff, and customers.

## Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Data Structure](#data-structure)
- [Default Accounts](#default-accounts)
- [License](#license)

## Features

### Authentication System
- **Role-based access control** with three user types:
  - **Admin**: Full system access
  - **Staff**: Limited management capabilities
  - **Customer**: View and personal information management
- **Secure login** with username/password authentication
- **Password change** functionality for all users

### User Management
- **Customer Management**
  - Add new customers
  - View customer list
  - Search customers by name or phone
  - Edit personal information
- **Staff Management**
  - Add new staff members
  - View staff list
  - Delete staff accounts

### Product Management
- **Comprehensive Product Catalog**
  - Add new products with details (ID, name, color, size, price, quantity)
  - View all products in formatted table
  - Search products by ID
  - Delete products from inventory
  - Category and supplier association

### Category & Supplier Management
- **Category Management**
  - View all product categories
  - Add new categories
- **Supplier Management**
  - View all suppliers
  - Add new suppliers

### Invoice System
- **Complete Sales Management**
  - Create new invoices with multiple products
  - Automatic inventory deduction
  - View all invoices
  - View invoice details
  - Filter invoices by staff or customer

### Statistics & Reporting
- View comprehensive business statistics
- Total customers, staff, products, and invoices
- Total revenue calculation

## System Requirements

- **Operating System**: Windows, Linux, or macOS
- **Compiler**: C++11 compatible compiler (GCC, Clang, MSVC)
- **RAM**: Minimum 512MB
- **Storage**: 50MB free space

## Installation

### Method 1: Using GCC (Recommended)

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/shoe-store-management.git
   cd shoe-store-management
   ```

2. **Compile the application:**
   ```bash
   g++ -std=c++11 -o shoe_store main.cpp
   ```

3. **Create the Data directory:**
   ```bash
   mkdir Data
   ```

4. **Run the application:**
   ```bash
   ./shoe_store
   ```

### Method 2: Using Code::Blocks

1. Clone or download the repository
2. Open Code::Blocks
3. Create a new project and add all `.h` and `.cpp` files
4. Build and run the project
5. Ensure the `Data/` directory exists in the same location as the executable

### Method 3: Using Visual Studio

1. Clone or download the repository
2. Open Visual Studio
3. Create a new Console Application project
4. Add all source files to the project
5. Build and run
6. Create the `Data/` directory in the output folder

## Usage

### Initial Setup

When you first run the application, it will attempt to load data from text files in the `Data/` directory. If files don't exist, they will be created automatically when you add new records.

### Login System

The application starts with a login screen. Use one of the [default accounts](#default-accounts) or create new ones through the admin interface.

### Navigation

- **Main Menu**: Choose to login or exit
- **Role-specific Menus**: Different interfaces based on user role
- **Input Validation**: The system validates all inputs and provides error messages
- **File Persistence**: All data is automatically saved to text files

### Admin Workflow

1. Login with admin credentials
2. Access the admin menu with full system control
3. Manage customers, staff, products, invoices, categories, and suppliers
4. View comprehensive statistics
5. Create and manage user accounts

### Staff Workflow

1. Login with staff credentials
2. Access limited functionality appropriate for sales staff
3. View and search products
4. Create invoices for customers
5. View personal sales history

### Customer Workflow

1. Login with customer credentials
2. Browse available products
3. Search for specific items
4. View personal purchase history
5. Update personal information

## Data Structure

### Custom Data Structures

The application implements custom data structures from scratch:

- **MyVector<T>**: Dynamic array implementation similar to std::vector
- **MyMap<K,V>**: Binary search tree implementation similar to std::map

### File Format

All data is stored in pipe-delimited text files:

```
Data/
├── Accounts.txt     # User accounts (username|password|type|userID)
├── Customer.txt     # Customer information
├── Staff.txt        # Staff information  
├── Product.txt      # Product catalog
├── Category.txt     # Product categories
├── Supplier.txt     # Supplier information
└── Invoice.txt      # Sales invoices with details
```

## Default Accounts

### Administrator Account
- **Username**: `admin`
- **Password**: `admin0123`
- **Access**: Full system control

### Staff Accounts
- **Username**: `NV001` | **Password**: `staff123`
- **Username**: `NV002` | **Password**: `staff123`
- **Username**: `NV003` | **Password**: `staff123`

### Customer Accounts
- **Username**: `CUS001` | **Password**: `123456`
- **Username**: `CUS002` | **Password**: `123456`
- **Username**: `CUS003` | **Password**: `123456`

### Performance Notes

- The application loads all data into memory at startup
- Large datasets may require additional RAM
- File operations are performed synchronously

### Development Guidelines

- Follow existing code style and naming conventions
- Add comments for complex logic
- Test all new features thoroughly
- Update documentation as needed

## License

This project is open source and available under the [MIT License](LICENSE).
