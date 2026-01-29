#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cstdlib>  


using namespace std;

struct User
{
    string username;
    string password;
};

struct Inventory
{
    string name;
    string category;
    int stock;
    int price;
    int cartQty;
};

vector <Inventory> items;

string role;

bool isValidAdmin(const string& username, const string& password);

void loadInventoryFromFile();

void saveInventoryToFile();

void checkoutCart();

void restockProduct();

void updateProductPrice();

void lowStockWarning();

void showCategories();

string getCategoryByChoice(int ch);

void userMenu();

void buyProduct();

void addToCart();

void viewCart();

void deleteCartItems();

void searchProduct();

void deleteProduct();

void addNewProduct();

void viewInventory();

bool userlogin();

bool adminlogin();

void clearScreen();

void pressEnterToContinue();

void showCategory(const string& cat);

bool paymentoptions(double bill, double& codCharges);

void generateReceiptDetailed(const vector<Inventory>& purchasedItems, double totalBill, double codCharges);


bool isValidAdmin(const string& username, const string& password)
{
    return (username == "ar" && password == "ar123");
}


void clearScreen()
{
#ifdef _WIN32
    system("cls");

#else
    system("clear");

#endif
}


void pressEnterToContinue()
{
    cout << "\nPress Enter to continue...";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cin.get();

    clearScreen();
}


int getIntInput()
{
    int x;
    cin >> x;

    while (cin.fail())
    {
        cin.clear();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid input! Enter a number: ";
        cin >> x;
    }

    return x;
}


bool adminlogin()
{
    string username, password;

    cout << "\n=============== ADMIN LOGIN ===============\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);

    if (isValidAdmin(username, password))
    {
        role = "ADMIN";
        cout << "[SUCCESS] Login Successful!\n";

        return true;
    }

    else
    {
        cout << "[ERROR] Invalid Credentials! Please try again.\n\n";

        return false;
    }
}

bool userlogin()
{
    User u;
    string username, password;
    ifstream file("Users.txt");

    if (!file)
    {
        cout << "[ERROR] User database not found!\n";

        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n=============== USER LOGIN ===============\n";

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);

    while (getline(file, u.username, '|') && getline(file, u.password))
    {
        if (u.username == username && u.password == password)
        {
            role = "USER";

            cout << "[SUCCESS] Login Successful!\n";

            file.close();

            return true;
        }
    }

    file.close();

    cout << "[ERROR] Invalid username or password!\n";

    return false;
}

bool registerUser()
{
    User u, temp;
    bool exists = false;
    bool digit, special_char;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n============= USER REGISTRATION =============\n";

    cout << "Enter Username: ";
    getline(cin, u.username);

    while (true)
    {
        cout << "Enter Password: ";

        getline(cin, u.password);

        digit = false;

        special_char = false;

        if (u.password.length() < 8)
        {

            cout << "Password must be least 8 characters long.\n";

            continue;
        }

        for (int i = 0; i < u.password.length(); i++)
        {
            if (u.password[i] >= '0' && u.password[i] <= '9')
                digit = true;

            else if (!(u.password[i] >= 'a' && u.password[i] <= 'z') &&
                !(u.password[i] >= 'A' && u.password[i] <= 'Z'))

                special_char = true;
        }

        if (digit && special_char)
            break;

        else
            cout << "Password must contain at least 1 digit and 1 special character\n";
    }

    if (isValidAdmin(u.username, u.password))
    {
        cout << "[ERROR] This username is reserved for admin.\n";

        return false;
    }

    ifstream infile("Users.txt");
    while (getline(infile, temp.username, '|') && getline(infile, temp.password))
    {
        if (temp.username == u.username)
        {
            exists = true;
            break;
        }
    }
    infile.close();

    if (exists)
    {
        cout << "[ERROR] Username already exists! Try another.\n";
        return false;
    }

    ofstream outfile("Users.txt", ios::app);

    outfile << u.username << "|" << u.password << endl;

    outfile.close();

    role = "USER";

    cout << "[SUCCESS] User registered successfully!\n";
    return true;
}


void forgotPassword()
{
    string username;

    cout << "\n============= FORGOT PASSWORD =============\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your username: ";
    getline(cin, username);

    User u;
    ifstream file("Users.txt");
    if (!file)
    {
        cout << "[ERROR] User database not found!\n";
        return;
    }

    bool found = false;
    while (getline(file, u.username, '|') && getline(file, u.password))
    {
        if (u.username == username)
        {
            cout << "[SUCCESS] Your password is: " << u.password << "\n";
            found = true;
            break;
        }
    }
    file.close();

    if (!found)
    {
        cout << "[ERROR] Username not found!\n";
    }

    pressEnterToContinue();
}

void loadInventoryFromFile()
{
    items.clear();

    ifstream file("Inventory.txt");
    if (!file)
    {
        cout << "\n[ERROR] Inventory file not found!\n";
        return;
    }

    Inventory item;
    string line, temp;
    int field = 0;

    while (getline(file, line))
    {
        item.name = "";
        item.category = "";
        item.stock = 0;
        item.price = 0;

        temp = "";
        field = 0;

        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == '|')
            {
                if (field == 0)
                    item.name = temp;

                else if (field == 1)
                    item.category = temp;

                else if (field == 2)
                    item.stock = stoi(temp);

                temp = "";
                field++;
            }
            else
            {
                temp += line[i];
            }
        }

        item.price = stoi(temp);

        item.cartQty = 0;
        items.push_back(item);
    }

    file.close();

    cout << "\n[INFO] Inventory loaded successfully! Total items: "
        << items.size() << "\n";
}

void saveInventoryToFile()
{
    ofstream file("Inventory.txt");

    for (auto& item : items)
    {
        file << item.name << "|"
            << item.category << "|"
            << item.stock << "|"
            << item.price << endl;
    }

    file.close();
}

void showCategory(const string& cat)
{
    cout << "\n============== " << cat << " CATEGORY ==============\n";

    cout << left << setw(5) << "No"
        << setw(25) << "Product"
        << setw(10) << "Stock"
        << setw(10) << "Price" << endl;

    cout << "-----------------------------------------------\n";

    int count = 1;
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].category == cat)
        {
            cout << left
                << setw(5) << count++
                << setw(25) << items[i].name
                << setw(10) << items[i].stock
                << setw(10) << items[i].price
                << endl;
        }
    }

    cout << "-----------------------------------------------\n";
}

vector<int> getProductsByCategory(const string& category)
{
    vector<int> indices;
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].category == category)
            indices.push_back(i);
    }
    return indices;
}

void viewInventory()
{

    int ch;

    showCategories();

    ch = getIntInput();

    switch (ch)
    {
    case 1:
        showCategory("Electronics");
        break;

    case 2:
        showCategory("Grocery");
        break;

    case 3:
        showCategory("Stationary");
        break;

    case 4:
        showCategory("Medicines");
        break;

    case 5:
        showCategory("Sports");
        break;

    case 6:
        showCategory("Makeup");
        break;

    case 7:
        showCategory("Clothing");
        break;

    case 8:
        showCategory("Jewellery");
        break;

    case 9:
        showCategory("Home Appliances");
        break;

    case 10:
        showCategory("Fruits");
        break;

    case 11:
        showCategory("Vegetables");
        break;

    case 12:
        showCategory("Toys");
        break;

    case 13:
        showCategory("Wedding");
        break;

    case 0:
        return;

    default:
        cout << "Invalid Choice! Try Again.\n";
    }


    pressEnterToContinue();
}

void addNewProduct()
{
    while (true)
    {
        Inventory newItem;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Product Name: ";
        getline(cin, newItem.name);

        cout << "Enter Category: ";
        getline(cin, newItem.category);

        cout << "Enter Stock Quantity: ";
        cin >> newItem.stock;

        cout << "Enter Price: ";
        cin >> newItem.price;

        newItem.cartQty = 0;
        items.push_back(newItem);

        cout << "\n Product Added Successfully!\n";

        cout << "Name     : " << newItem.name << "\n";

        cout << "Category : " << newItem.category << "\n";

        cout << "Stock    : " << newItem.stock << "\n";

        cout << "Price    : Rs. " << newItem.price << "\n";

        saveInventoryToFile();

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Add Another Product? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }

    }

    pressEnterToContinue();
}


void deleteProduct()
{
    while (true)
    {
        showCategories();

        int catChoice; 
        catChoice = getIntInput();

        string category = getCategoryByChoice(catChoice);
        if (category == "")
        {
            cout << "Invalid category!\n";
            return;
        }

        vector<int> indexMap;

        cout << "\n=========== DELETE PRODUCT ("
            << category << ") ===========\n";

        cout << left << setw(5) << "No"
            << setw(25) << "Product\n"
            << "---------------------------------------\n";

        int count = 1;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].category == category)
            {
                indexMap.push_back(i);
                cout << setw(5) << count++
                    << setw(25) << items[i].name
                    << "\n";
            }
        }

        if (indexMap.empty())
        {
            cout << "No products found in this category.\n";
            return;
        }

        int choice;

        cout << "Select product number to delete: ";
        choice = getIntInput();

        if (choice < 1 || choice > indexMap.size())
        {
            cout << "Invalid selection!\n";
            return;
        }

        string deletedName = items[indexMap[choice - 1]].name;
        items.erase(items.begin() + indexMap[choice - 1]);

        cout << "Product Deleted Successfully!\n";

        cout << "Deleted Product: " << deletedName << "\n";

        saveInventoryToFile();

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Delete Another Product? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }

    }


}

string toLower(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
}

void searchProduct()
{
    string search;

    cout << "\n=============== PRODUCT SEARCH ===============\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Product Name to Search: ";
    getline(cin, search);

    search = toLower(search);
    bool found = false;

    cout << "\nMatched Products:\n";

    for (int i = 0; i < items.size(); i++)
    {
        if (toLower(items[i].name).find(search) != string::npos)
        {
            cout << "----------------------------------------\n";

            cout << "Name     : " << items[i].name << "\n";

            cout << "Category : " << items[i].category << "\n";

            cout << "Stock    : " << items[i].stock << "\n";

            cout << "Price    : Rs. " << items[i].price << "\n";

            cout << "----------------------------------------\n";

            found = true;
        }
    }

    if (!found)
        cout << "No match found!\n";

    char ag;

    while (true)
    {
        cout << "\nDo You Want To Search Another Product? (y/n): ";
        cin >> ag;

        if (ag == 'y' || ag == 'Y')

            break;

        else if (ag == 'n' || ag == 'N')
        {
            pressEnterToContinue();
            return;
        }

        else
            cout << "Invalid input! Please enter y or n only.\n";
    }


    pressEnterToContinue();
}

void deleteCartItems()
{
    vector<int> cartIndex;

    for (int i = 0; i < items.size(); i++)
        if (items[i].cartQty > 0)
            cartIndex.push_back(i);

    if (cartIndex.empty())
    {
        cout << "Cart is empty!\n";
        return;
    }

    cout << "\n Select item to delete from cart:\n";

    for (int i = 0; i < cartIndex.size(); i++)
        cout << i + 1 << ". " << items[cartIndex[i]].name << "\n";

    cout << "0. Clear entire cart\n";

    int choice;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 0)
    {
        for (auto idx : cartIndex)
            items[idx].cartQty = 0;

        cout << "Cart cleared successfully!\n";
    }

    else if (choice >= 1 && choice <= cartIndex.size())
    {
        items[cartIndex[choice - 1]].cartQty = 0;
        cout << "Item removed successfully!\n";
    }

    else
        cout << "Invalid choice! Please try again.\n";

}

void lowStockWarning()
{
    const int LOW_STOCK_LIMIT = 5;
    bool found = false;

    cout << "\n========== LOW STOCK ALERT ==========\n";

    cout << left << setw(25) << "Product"
        << setw(15) << "Category"
        << setw(10) << "Stock" << "\n";

    cout << "-----------------------------------------------\n";

    for (int i = 0; i < items.size(); i++)
    {

        if (items[i].stock <= LOW_STOCK_LIMIT)
        {
            cout << setw(25) << items[i].name
                << setw(15) << items[i].category
                << setw(10) << items[i].stock << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "All products have sufficient stock. \n";

    pressEnterToContinue();
}

void viewCart()
{
    int choice;
    while (true)
    {
        double total = 0;
        bool empty = true;

        cout << "============= YOUR CART =============\n";

        for (int i = 0; i < items.size(); i++)
        {

            if (items[i].cartQty > 0)
            {
                empty = false;

                cout << i + 1 << ". "
                    << items[i].name
                    << " X " << items[i].cartQty
                    << " = Rs. " << items[i].cartQty * items[i].price << endl;

                total += items[i].cartQty * items[i].price;
            }
        }

        if (empty)
        {
            cout << "Your cart is empty! \n";
            return;
        }

        cout << "------------------------------------\n";

        cout << "TOTAL: Rs. " << total << "\n";

        cout << "\nOptions:\n";

        cout << "1. Delete Items from Cart\n";

        cout << "2. Checkcout Cart\n";

        cout << "0. Go Back to User Menu\n";

        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            deleteCartItems();
        }

        else if (choice == 2)
        {
            checkoutCart();
        }

        else if (choice == 0)
        {
            return;
        }
        else
        {
            cout << "Invalid choice! Try again. \n";
        }
    }
}

void addToCart()
{
    while (true)
    {
        cout << "================= ADD TO CART =================\n";

        showCategories();

        int catChoice;
        
        catChoice = getIntInput();

        string category = getCategoryByChoice(catChoice);
        if (category == "")
        {
            cout << "Invalid category! \n";
            return;
        }

        vector<int> indexMap;
        cout << "\n============= Products in " << category << " =============\n";

        cout << left << setw(5) << "No"
            << setw(25) << "Product"
            << setw(10) << "Stock"
            << setw(10) << "Price";

        cout << "\n------------------------------------------------\n";

        int count = 1;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].category == category)
            {
                indexMap.push_back(i);

                cout << setw(5) << count++
                    << setw(25) << items[i].name
                    << setw(10) << items[i].stock
                    << setw(10) << items[i].price
                    << endl;
            }
        }

        if (indexMap.empty())
        {
            cout << "No products found in this category. \n";
            return;
        }

        int choice, qty;
        cout << "Select Product Number: ";
        choice = getIntInput();

        if (choice < 1 || choice > indexMap.size())
        {
            cout << "Invalid product selection! \n";
            return;
        }

        Inventory& p = items[indexMap[choice - 1]];

        cout << "Enter Quantity: ";
        qty = getIntInput();

        if (qty <= 0 || qty > p.stock)
        {
            cout << "Invalid quantity or insufficient stock! \n";
            return;
        }

        p.cartQty += qty;

        cout << " " << p.name << " X " << qty << " added to cart successfully! \n";

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Add Another Product in the Cart? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }

    }

}

void checkoutCart()
{
    vector<Inventory> cartItems;
    double totalBill = 0;

    for (auto& item : items)
    {
        if (item.cartQty > 0)
        {
            cartItems.push_back(item);
            totalBill += item.cartQty * item.price;
        }
    }

    if (cartItems.empty())
    {
        cout << "Cart is empty! \n";
        return;
    }

    cout << "\n=================== CART CHECKOUT ===================\n";

    cout << "TOTAL BILL: Rs. " << totalBill << "\n";

    double codCharges = 0;
    bool paymentSuccess = paymentoptions(totalBill, codCharges);

    if (!paymentSuccess)
    {
        cout << "Payment failed. Checkout canceled. \n";
        return;
    }


    for (auto& cartItem : cartItems)
    {
        for (auto& invItem : items)
        {
            if (invItem.name == cartItem.name)
            {
                invItem.stock -= cartItem.cartQty;
                invItem.cartQty = 0;
                break;
            }
        }
    }

    generateReceiptDetailed(cartItems, totalBill, codCharges);

    cout << "\n Checkout Successful! \n";

    saveInventoryToFile();

    pressEnterToContinue();
}

void buyProduct()
{
    while (true)
    {
        showCategories();
        char again;

        double codCharges;

        int catChoice = getIntInput();

        string category = getCategoryByChoice(catChoice);

        if (category == "")
        {
            cout << "Invalid category! \n";
            return;
        }

        vector<int> indexMap;

        cout << "\n============= Products in " << category << " =============\n";

        cout << left << setw(5) << "No"
            << setw(25) << "Product"
            << setw(10) << "Stock"
            << setw(10) << "Price";

        cout << "\n------------------------------------------------\n";

        int count = 1;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].category == category)
            {
                indexMap.push_back(i);

                cout << setw(5) << count++
                    << setw(25) << items[i].name
                    << setw(10) << items[i].stock
                    << setw(10) << items[i].price
                    << endl;
            }
        }

        if (indexMap.empty())
        {
            cout << "No products found in this category. \n";
            return;
        }

        int choice, qty;

        cout << "Select Product Number: ";
        choice = getIntInput();

        if (choice < 1 || choice > indexMap.size())
        {
            cout << "Invalid selection! \n";
            return;
        }

        Inventory& p = items[indexMap[choice - 1]];

        cout << "Enter Quantity: ";
        qty = getIntInput();

        if (qty <= 0 || qty > p.stock)
        {
            cout << "Invalid quantity or insufficient stock! \n";
            return;
        }

        double bill = qty * p.price;

        cout << "\n" << p.name << " X " << qty << " purchased.";

        cout << "\nTOTAL BILL: Rs. " << bill << "\n";

        char confirm;
        cout << "Confirm purchase? (y/n): ";
        cin >> confirm;

        if (confirm != 'y' && confirm != 'Y')
        {
            cout << "Purchase canceled. \n";
            return;
        }

        bool paymentSuccess = paymentoptions(bill, codCharges);

        if (paymentSuccess)
        {
            Inventory temp = p;
            temp.cartQty = qty;
            p.stock -= qty;

            vector<Inventory> purchasedItems = { temp };

            generateReceiptDetailed(purchasedItems, bill, codCharges);

            cout << "\nPurchase Successful! \n";

            cout << "Product Name : " << p.name << endl;

            cout << "Quantity     : " << qty << endl;

            cout << "Total Bill   : Rs. " << bill + codCharges << endl;
        }

        else
        {
            cout << "Payment failed. Purchase not completed. \n";
        }

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Buy Another Product? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }
    }

    pressEnterToContinue();
}

bool paymentoptions(double bill, double& codCharges)
{
    int choice;
    double amount;
    string number;
    bool valid;

    codCharges = 0;

    while (true)
    {
        cout << "\n================== PAYMENT OPTIONS ==================\n";

        cout << "1. Online Payment\n2. Cash on Delivery (COD)\n0. Cancel Payment\n";

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "\n--- ONLINE PAYMENT ---\n";

            while (true)
            {
                cout << "Enter Mobile/Card Number: ";
                cin >> number;

                valid = true;

                if (number.length() != 11)
                {
                    valid = false;
                }

                else
                {
                    for (int i = 0; i < 11; i++)
                    {
                        if (number[i] < '0' || number[i] > '9')
                        {
                            valid = false;
                            break;
                        }
                    }
                }

                if (valid)
                    break;

                else
                    cout << "Invalid mobile number! Please enter exactly 11 digits.\n";
            }

            cout << "Enter Amount (Rs.): ";
            cin >> amount;

            if (amount == bill)
            {
                cout << "Payment Successful!\n";
                return true;
            }

            else
            {
                cout << "Wrong Amount! Please pay Rs. " << bill << "\n";
            }
        }

        else if (choice == 2)
        {
            codCharges = 100;

            cout << "--- CASH ON DELIVERY ---\n";

            cout << "Rs. 100 COD Charges Applied.\n";

            return true;
        }

        else if (choice == 0)
        {
            cout << "Payment Cancelled.\n";
            return false;
        }

        else
        {
            cout << "Invalid option! Try again.\n";
        }
    }
}
void showCategories()
{
    cout << "\n\nAVAILABLE CATEGORIES\n";

    cout << "____________________\n";

    cout << "1. Electronics \n";

    cout << "2. Grocery\n";

    cout << "3. Stationary\n";

    cout << "4. Medicines\n";

    cout << "5. Sports\n";

    cout << "6. Makeup\n";

    cout << "7. Clothing\n";

    cout << "8. Jewellery\n";

    cout << "9. Home Appliances\n";

    cout << "10. Fruits\n";

    cout << "11. Vegetables\n";

    cout << "12. Toys\n";

    cout << "13. Wedding\n";

    cout << "\n0. Go back\n";

    cout << "____________________\n\n";

    cout << "Enter number of category: \n";
}

string getCategoryByChoice(int ch)
{
    switch (ch)
    {
    case 1:
        return "Electronics";

    case 2:
        return "Grocery";

    case 3:
        return "Stationary";

    case 4:
        return "Medicines";

    case 5:
        return "Sports";

    case 6:
        return "Makeup";

    case 7:
        return "Clothing";

    case 8:
        return "Jewellery";

    case 9:
        return "Home Appliances";

    case 10:
        return "Fruits";

    case 11:
        return "Vegetables";

    case 12:
        return "Toys";

    case 13:
        return "Wedding";

    default:
        return "";

    }
}

void restockProduct()
{
    while (true)
    {
        showCategories();

        int catChoice;
        catChoice = getIntInput();
        ;

        string category = getCategoryByChoice(catChoice);

        if (category == "")
        {
            cout << "Invalid category! \n";
            return;
        }

        vector<int> indexMap;

        string line = "-----------------------------------------------------";

        cout << "\n=========== RESTOCK PRODUCT (" << category << ") ===========\n";

        cout << left << setw(5) << "No"
            << setw(25) << "Product"
            << setw(10) << "Stock\n";

        cout << line << "\n";

        int count = 1;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].category == category)
            {
                indexMap.push_back(i);

                cout << setw(5) << count++
                    << setw(25) << items[i].name
                    << setw(10) << items[i].stock << endl;
            }
        }

        if (indexMap.empty())
        {
            cout << "No products found in this category. \n";
            return;
        }

        int choice, qty;
        cout << "\nSelect product number to restock: ";

        choice = getIntInput();
        if (choice < 1 || choice > indexMap.size())
        {
            cout << "Invalid selection! \n";
            return;
        }

        cout << "Enter quantity to add: ";
        qty = getIntInput();

        if (qty <= 0)
        {
            cout << "Invalid quantity! \n";
            return;
        }

        int oldStock = items[indexMap[choice - 1]].stock;

        items[indexMap[choice - 1]].stock += qty;

        cout << "\n Stock Updated Successfully!\n";

        cout << "Product Name    : " << items[indexMap[choice - 1]].name << endl;

        cout << "Previous Stock  : " << oldStock << endl;

        cout << "Added Quantity  : " << qty << endl;

        cout << "Updated Stock   : " << items[indexMap[choice - 1]].stock << endl;

        saveInventoryToFile();

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Restock Another Product Price? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }
    }

    pressEnterToContinue();
}


void updateProductPrice()
{

    while (true)
    {
        showCategories();

        int catChoice;
        cin >> catChoice;

        string category = getCategoryByChoice(catChoice);
        if (category == "")
        {
            cout << "Invalid category! \n";
            return;
        }

        vector<int> indexMap;

        string line = "-------------------------------------------------";
        cout << "\n======= UPDATE PRODUCT PRICE (" << category << ") =======\n";

        cout << left << setw(5) << "No"
            << setw(25) << "Product"
            << setw(10) << "Price\n";

        cout << line << "\n";

        int count = 1;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].category == category)
            {
                indexMap.push_back(i);
                cout << setw(5) << count++
                    << setw(25) << items[i].name
                    << setw(10) << items[i].price << endl;
            }
        }

        if (indexMap.empty())
        {
            cout << "No products found in this category. \n";
            return;
        }

        int choice;
        double newPrice;
        cout << "\nSelect product number to update price: ";
        cin >> choice;

        if (choice < 1 || choice > indexMap.size())
        {
            cout << "Invalid selection! \n";
            return;
        }

        cout << "Enter new price: ";
        cin >> newPrice;

        if (newPrice <= 0)
        {
            cout << "Invalid price! \n";
            return;
        }

        int oldPrice = items[indexMap[choice - 1]].price;
        items[indexMap[choice - 1]].price = newPrice;

        cout << "\n Price Updated Successfully!\n";

        cout << "Product Name   : " << items[indexMap[choice - 1]].name << endl;

        cout << "Previous Price : Rs. " << oldPrice << "\n";

        cout << "New Price      : Rs. " << newPrice << "\n";

        saveInventoryToFile();

        char ag;

        while (true)
        {
            cout << "\nDo You Want To Update Another Product Price? (y/n): ";
            cin >> ag;

            if (ag == 'y' || ag == 'Y')

                break;

            else if (ag == 'n' || ag == 'N')
            {
                pressEnterToContinue();
                return;
            }

            else
                cout << "Invalid input! Please enter y or n only.\n";
        }

    }

    pressEnterToContinue();
}

void generateReceiptDetailed(const vector<Inventory>& purchasedItems, double totalBill, double codCharges = 0)
{
    int receiptCount = 1;

    ofstream receiptfile("Receipt.txt", ios::app);

    string line = "============================================================";

    cout << line << "\n";

    cout << "===================== SHOPPING RECEIPT =====================\n";

    cout << "No   Product             Qty       Price        Total\n";

    cout << "------------------------------------------------------------\n";

    receiptfile << "=============== SHOPPING MANAGEMENT SYSTEM ===============\n\n";

    receiptfile << "===================== SHOPPING RECEIPT =====================\nReceipt # " << receiptCount << endl;;

    receiptfile << left << setw(5) << "No"
        << setw(20) << "Product"
        << setw(10) << "Qty"
        << setw(15) << "Price"
        << setw(15) << "Total" << endl;

    receiptfile << "------------------------------------------------------------\n";

    for (int i = 0; i < purchasedItems.size(); ++i)
    {
        double itemTotal = purchasedItems[i].cartQty * purchasedItems[i].price;

        cout << left << setw(5) << (i + 1)
            << setw(20) << purchasedItems[i].name
            << setw(10) << purchasedItems[i].cartQty
            << setw(15) << purchasedItems[i].price
            << setw(15) << itemTotal
            << endl;

        receiptfile << left << setw(5) << (i + 1)
            << setw(20) << purchasedItems[i].name
            << setw(10) << purchasedItems[i].cartQty
            << setw(15) << purchasedItems[i].price
            << setw(15) << itemTotal << endl;
    }

    if (codCharges > 0)
    {
        cout << "Cash on Delivery Charges: Rs. " << codCharges << "\n";

        receiptfile << "Cash on Delivery Charges: Rs. " << codCharges << endl;
    }

    double finalBill = totalBill + codCharges;

    cout << "------------------------------------------------------------\n";

    cout << right << setw(50) << "TOTAL BILL: Rs. " << finalBill << " \n";

    cout << line << "\n";

    cout << " ORDER CONFIRMED! THANK YOU FOR SHOPPING WITH US! \n";

    receiptfile << "------------------------------------------------------------\n";

    receiptfile << right << setw(50) << "TOTAL BILL: Rs. " << finalBill << endl;

    receiptfile << "============================================================\n";

    receiptfile << "ORDER CONFIRMED!\nTHANK YOU FOR SHOPPING WITH US!\n\n";

    receiptfile.close();
}

void adminMenu()
{
    int choice;
    do
    {
        cout << "\n==================== ADMIN MENU ====================\n";

        cout << "1.  View Inventory\n";

        cout << "2.  Restock Product\n";

        cout << "3.  Update Product Prices\n";

        cout << "4.  Add New Product\n";

        cout << "5.  Delete Product\n";

        cout << "6.  Search Product\n";

        cout << "7.  Low Stock Warning\n";

        cout << "0.  Log Out\n";

        cout << "Enter Choice: ";
        choice = getIntInput();

        switch (choice)
        {
        case 1:
            viewInventory();
            break;

        case 2:
            restockProduct();
            break;

        case 3:
            updateProductPrice();
            break;

        case 4:
            addNewProduct();
            break;

        case 5:
            deleteProduct();
            break;

        case 6:
            searchProduct();
            break;

        case 7:
            lowStockWarning();
            break;

        case 0:
            cout << "\nLogged Out! Thank You for using our system!\n\n";
            break;

        default:
            cout << "Invalid choice! Please try again. \n";
        }

    } while (choice != 0);
}

void userMenu()
{
    int choice;
    do
    {
        cout << "\n==================== USER MENU ====================\n";

        cout << "1.  View Inventory\n";

        cout << "2.  Buy Product (Direct)\n";

        cout << "3.  Add to Cart\n";

        cout << "4.  View Cart\n";

        cout << "5.  Checkout Cart\n";

        cout << "0.  Logout\n";

        cout << "Enter Choice: ";
        choice = getIntInput();


        switch (choice)
        {
        case 1:
            viewInventory();
            break;

        case 2:
            buyProduct();
            break;

        case 3:
            addToCart();
            break;

        case 4:
            viewCart();
            break;

        case 5:
            checkoutCart();
            break;

        case 0:
        {
            bool cartEmpty = true;
            for (auto& item : items)
            {
                if (item.cartQty > 0)
                {
                    cartEmpty = false;
                    break;
                }
            }

            if (!cartEmpty)
            {
                cout << "Cart not empty! Kindly checkout or clear cart before logout! \n";
                choice = -1;
                break;
            }
            else
            {
                cout << "\nLogged Out! Thank You for using our system!\n\n";
                break;
            }
        }

        default:
            cout << "Invalid choice! Try again. \n";
        }

    } while (choice != 0);
}

int main()
{
    loadInventoryFromFile();

    cout << "================== SHOPPING MANAGEMENT SYSTEM ==================\n";

    cout << "Enter Your Desired Choice\n";

    int ch;
    do
    {
        cout << "\n1. Admin\n";

        cout << "2. User\n";

        cout << "0. Exit Program\n";

        cout << "Enter Your Choice: ";
        ch = getIntInput();

        switch (ch)
        {
        case 1:
            if (adminlogin())
            {
                adminMenu();
            }
            break;

        case 2:
        {
            int choice;

            do
            {
                cout << "\n1. Login as user\n";

                cout << "2. Register as user\n";

                cout << "3. Forgot Password\n";

                cout << "0. Go back\n";

                cout << "Enter Your Choice: ";
                choice = getIntInput();

                switch (choice)
                {
                case 1:
                    if (userlogin())
                        userMenu();
                    break;

                case 2:
                {
                    if (registerUser())
                        userMenu();
                    break;
                }


                case 3:
                    forgotPassword();
                    break;

                case 0:
                    break;

                default:
                    cout << "\nInvalid Choice! Please try again\n";
                }
            } while (choice != 0);

            break;
        }

        case 0:
            cout << "\nProgram Exited\n";
            break;

        default:
            cout << "Invalid Choice! Please Try Again.\n";
            break;
        }
    } while (ch != 0);

    return 0;
}