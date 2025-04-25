#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <map>
#include <ctime>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace std;

const float GST_RATE = 0.18f;
const float DISCOUNT_THRESHOLD = 200.0f;
const float DISCOUNT_RATE = 0.10f;
const vector<string> CATEGORIES = {"Fruits", "Vegetables", "Dairy", "Snacks", "Beverages", "Clothing", "All"};

// Base Product Class
class Product {
protected:
    string name;
    float price;
    int quantity;
    string barcode;
    bool bogoOffer;
    string category;

public:
    Product(string n, float p, int q, string b, string c, bool offer = false)
        : name(n), price(p), quantity(q), barcode(b), bogoOffer(offer), category(c) {}

    virtual ~Product() {} // virtual destructor

    virtual void display() const {
        cout << left << setw(20) << name 
             << setw(15) << category
             << "₹" << setw(10) << fixed << setprecision(2) << price
             << setw(10) << quantity
             << setw(15) << barcode;
        if (bogoOffer) cout << "BOGO Offer!";
        cout << endl;
    }

    float getPrice() const { return price; }
    string getName() const { return name; }
    string getBarcode() const { return barcode; }
    bool hasBogoOffer() const { return bogoOffer; }
    void updateQuantity(int qty) { quantity += qty; }
    int getQuantity() const { return quantity; }
    string getCategory() const { return category; }
    bool isAvailable(int requestedQty) const { return quantity >= requestedQty; }
};

// Derived Product Type - Perishable
class PerishableProduct : public Product {
    string expiryDate;

public:
    PerishableProduct(string n, float p, int q, string b, string c, string expiry, bool offer = false)
        : Product(n, p, q, b, c, offer), expiryDate(expiry) {}

    void display() const override {
        Product::display();
        cout << " (Expires on: " << expiryDate << ")";
    }
};

// Derived Product Type - Clothing
class ClothingProduct : public Product {
    string size;

public:
    ClothingProduct(string n, float p, int q, string b, string c, string sz, bool offer = false)
        : Product(n, p, q, b, c, offer), size(sz) {}

    void display() const override {
        Product::display();
        cout << " (Size: " << size << ")";
    }
};

// Cart item stays the same
class CartItem {
public:
    string productName;
    int quantity;
    float unitPrice;
    bool bogoApplied;

    CartItem(string name, int qty, float price, bool bogo) 
        : productName(name), quantity(qty), unitPrice(price), bogoApplied(bogo) {}

    float getTotal() const {
        int paidQty = (bogoApplied) ? ((quantity % 2 == 0) ? quantity / 2 : (quantity / 2 + 1)) : quantity;
        return paidQty * unitPrice;
    }

    void display() const {
        cout << left << setw(20) << productName 
             << setw(10) << quantity 
             << "₹" << setw(10) << fixed << setprecision(2) << unitPrice
             << "₹" << setw(10) << getTotal();
        if (bogoApplied) cout << " (BOGO)";
        cout << endl;
    }
};

// Supermarket class
class Supermarket {
private:
    vector<Product*> products;
    vector<CartItem> cart;
    map<string, string> users;
    string currentUser;

    void loadSampleProducts() {
        products.push_back(new Product("Banana", 30.0, 100, "FR001", "Fruits"));
        products.push_back(new Product("Apple", 30.0, 100, "FR002", "Fruits"));
        products.push_back(new Product("Orange", 30.0, 100, "FR003", "Fruits"));
        products.push_back(new Product("Grapes", 30.0, 100, "FR004", "Fruits"));
        products.push_back(new PerishableProduct("Milk", 25.0, 50, "DA001", "Dairy", "2025-05-01", true));
        products.push_back(new PerishableProduct("Oil", 25.0, 50, "DA001", "Dairy", "2025-05-01", true));
        products.push_back(new Product("Cheese", 120.0, 30, "DA002", "Dairy"));
        products.push_back(new Product("Ghee", 120.0, 30, "DA001", "Dairy"));
        products.push_back(new Product("Butter", 120.0, 30, "DA003", "Dairy"));
        products.push_back(new Product("Potato Chips", 20.0, 80, "SN001", "Snacks"));
        products.push_back(new Product("Moongdal", 20.0, 80, "SN001", "Snacks"));
        products.push_back(new Product("Puffcorn", 20.0, 80, "SN001", "Snacks"));
        products.push_back(new Product("Soda", 35.0, 120, "BE001", "Beverages", true));
        products.push_back(new Product("Sprite", 35.0, 120, "BE001", "Beverages", true));
        products.push_back(new ClothingProduct("T-Shirt", 299.0, 40, "CL001", "Clothing", "L"));
        products.push_back(new ClothingProduct("Skirt", 299.0, 40, "CL001", "Clothing", "L"));
        products.push_back(new ClothingProduct("Frock", 299.0, 40, "CL001", "Clothing", "L"));
    }

    string toLower(const string& str) {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    Product* findProduct(const string& name) {
        string lowerName = toLower(name);
        for (auto& product : products) {
            if (toLower(product->getName()) == lowerName) {
                return product;
            }
        }
        return nullptr;
    }

    void displayProductHeader() const {
        cout << left << setw(20) << "Name" 
             << setw(15) << "Category"
             << setw(10) << "Price"
             << setw(10) << "Qty"
             << setw(15) << "Barcode"
             << "Offer" << endl;
        cout << string(80, '-') << endl;
    }

    void displayCartHeader() const {
        cout << left << setw(20) << "Item" 
             << setw(10) << "Qty"
             << setw(12) << "Unit Price"
             << setw(12) << "Total"
             << "Notes" << endl;
        cout << string(60, '-') << endl;
    }

public:
    Supermarket() {
        loadSampleProducts();
    }

    ~Supermarket() {
        for (auto p : products) delete p;
    }

    void registerUser(const string& username, const string& password) {
        users[username] = password;
        cout << "Registration successful!\n";
    }

    bool loginUser(const string& username, const string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second == password) {
            currentUser = username;
            cout << "Login successful! Welcome, " << username << "!\n";
            return true;
        }
        cout << "Invalid credentials!\n";
        return false;
    }

    void displayProductsByCategory(const string& category) const {
        cout << "\nAvailable Products";
        if (category != "All") cout << " in " << category;
        cout << ":\n";
        
        displayProductHeader();
        for (auto product : products) {
            if (category == "All" || product->getCategory() == category) {
                product->display();
                cout << endl;
            }
        }
    }

    bool addToCart(const string& name, int quantity) {
        Product* product = findProduct(name);
        if (!product) {
            cout << "Product not found!\n";
            return false;
        }

        if (!product->isAvailable(quantity)) {
            cout << "Not enough stock (Available: " << product->getQuantity() << ")\n";
            return false;
        }

        bool bogoApplied = product->hasBogoOffer();
        cart.emplace_back(product->getName(), quantity, product->getPrice(), bogoApplied);
        product->updateQuantity(-quantity);
        cout << quantity << " x " << product->getName() << " added to cart.\n";
        return true;
    }

    void viewCart() const {
        if (cart.empty()) {
            cout << "\nYour cart is empty.\n";
            return;
        }

        cout << "\nYour Shopping Cart:\n";
        displayCartHeader();

        float subtotal = 0.0f;
        for (const auto& item : cart) {
            item.display();
            subtotal += item.getTotal();
        }

        cout << string(60, '-') << endl;
        cout << right << setw(50) << "Subtotal: ₹" << fixed << setprecision(2) << subtotal << endl;
    }

    void removeFromCart(int index) {
        if (index < 0 || index >= cart.size()) {
            cout << "Invalid item number!\n";
            return;
        }

        Product* product = findProduct(cart[index].productName);
        if (product) product->updateQuantity(cart[index].quantity);
        cart.erase(cart.begin() + index);
        cout << "Item removed from cart.\n";
    }

    void generateReceipt() {
        if (cart.empty()) {
            cout << "\nCart is empty. No receipt to generate.\n";
            return;
        }

        time_t now = time(0);
        tm* ltm = localtime(&now);
        string filename = "receipt_" + currentUser + "_" + to_string(now) + ".txt";
        ofstream receiptFile(filename);

        if (!receiptFile) {
            cout << "Error creating receipt file!\n";
            return;
        }

        float subtotal = 0.0f;
        for (const auto& item : cart) subtotal += item.getTotal();
        float discount = (subtotal > DISCOUNT_THRESHOLD) ? subtotal * DISCOUNT_RATE : 0.0f;
        float gstAmount = (subtotal - discount) * GST_RATE;
        float finalAmount = subtotal - discount + gstAmount;

        receiptFile << "===== SUPERMARKET RECEIPT =====\n";
        receiptFile << "Customer: " << currentUser << "\n";
        receiptFile << "Date: " << 1900 + ltm->tm_year << "-" 
                    << 1 + ltm->tm_mon << "-" << ltm->tm_mday << "\n";
        receiptFile << "----------------------------------\n";
        for (const auto& item : cart) {
            receiptFile << item.productName << " x" << item.quantity << " = ₹" 
                        << item.getTotal();
            if (item.bogoApplied) receiptFile << " (BOGO)";
            receiptFile << "\n";
        }
        receiptFile << "Subtotal: ₹" << subtotal << "\n";
        receiptFile << "Discount: ₹" << discount << "\n";
        receiptFile << "GST: ₹" << gstAmount << "\n";
        receiptFile << "Total: ₹" << finalAmount << "\n";
        receiptFile << "===============================\n";

        receiptFile.close();
        cout << "\nReceipt saved as '" << filename << "'\n";

        processPayment(finalAmount);
        cart.clear();
    }

    void processPayment(float amount) {
        cout << "\nPayment Amount: ₹" << fixed << setprecision(2) << amount << "\n";
        cout << "1. Cash\n2. Card\n3. Net Banking\n4. UPI\nEnter choice: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter 1-4: ";
        }
        cin.ignore();

        cout << "Payment of ₹" << amount << " successful. Thank you!\n";
    }

    void showMainMenu() {
        while (true) {
            cout << "\n===== MAIN MENU =====\n";
            cout << "1. Browse Products\n2. View Cart\n3. Add to Cart\n4. Remove from Cart\n5. Checkout\n6. Exit\nChoice: ";

            int choice;
            while (!(cin >> choice) || choice < 1 || choice > 6) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter 1-6: ";
            }
            cin.ignore();

            if (choice == 1) {
                cout << "Select Category:\n";
                for (size_t i = 0; i < CATEGORIES.size(); ++i)
                    cout << i + 1 << ". " << CATEGORIES[i] << endl;\

                int catChoice;
                cin >> catChoice;
                cin.ignore();
                if (catChoice >= 1 && catChoice <= CATEGORIES.size())
                    displayProductsByCategory(CATEGORIES[catChoice - 1]);
            } else if (choice == 2) viewCart();
            else if (choice == 3) {
                string name;
                int qty;
                cout << "Product name: ";
                getline(cin, name);
                cout << "Quantity: ";
                cin >> qty;
                cin.ignore();
                addToCart(name, qty);
            } else if (choice == 4) {
                viewCart();
                int idx;
                cout << "Item number to remove: ";
                cin >> idx;
                cin.ignore();
                removeFromCart(idx - 1);
            } else if (choice == 5) {
                generateReceipt();
                return;
            } else {
                cout << "Thank you for shopping!\n";
                return;
            }
        }
    }
};

void showWelcomeMenu(Supermarket& store) {
    while (true) {
        cout << "\n===== WELCOME =====\n1. Register\n2. Login\n3. Exit\nChoice: ";
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string user, pass;
            cout << "Username: ";
            getline(cin, user);
            cout << "Password: ";
            getline(cin, pass);
            store.registerUser(user, pass);
        } else if (choice == 2) {
            string user, pass;
            cout << "Username: ";
            getline(cin, user);
            cout << "Password: ";
            getline(cin, pass);
            if (store.loginUser(user, pass)) store.showMainMenu();
        } else {
            cout << "Exiting. Goodbye!\n";
            break;
        }
    }
}

int main() {
    Supermarket store;
    showWelcomeMenu(store);
return 0;
}
























