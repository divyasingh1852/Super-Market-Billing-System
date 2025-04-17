#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <map>
#include <ctime>
using namespace std;

#define GST_RATE 0.18
#define DISCOUNT_THRESHOLD 200.0
#define DISCOUNT_RATE 0.10

class Product {
private:
    string name;
    float price;
    int quantity;
    string barcode;
    bool bogoOffer;
    string category;

public:
    Product(string n, float p, int q, string b, string c, bool offer = false)
        : name(n), price(p), quantity(q), barcode(b), bogoOffer(offer), category(c) {}

    void display() {
        cout << "Product: " << name << " | Category: " << category
             << " | Price: ₹" << fixed << setprecision(2) << price
             << " | Quantity: " << quantity
             << " | Barcode: " << barcode;
        if (bogoOffer) cout << " | Offer: Buy One Get One Free!";
        cout << endl;
    }

    float getPrice() { return price; }
    string getName() { return name; }
    string getBarcode() { return barcode; }
    bool hasBogoOffer() { return bogoOffer; }
    void updateQuantity(int qty) { quantity -= qty; }
    int getQuantity() { return quantity; }
    string getCategory() { return category; }
};

class Supermarket {
private:
    vector<Product> products;
    vector<pair<string, int>> cart;
    float totalAmount;
    map<string, string> users;
    string currentUser;

public:
    Supermarket() : totalAmount(0.0) {}

    void registerUser(string username, string password) {
        users[username] = password;
        cout << "Registration successful!\n";
    }

    bool loginUser(string username, string password) {
        if (users.find(username) != users.end() && users[username] == password) {
            currentUser = username;
            cout << "Login successful!\n";
            return true;
        }
        cout << "Invalid credentials!\n";
        return false;
    }

    void addProduct(string name, float price, int quantity, string barcode, string category, bool bogoOffer = false) {
        products.push_back(Product(name, price, quantity, barcode, category, bogoOffer));
    }

    void displayProductsByCategory(string category) {
        if (category == "All") {
            cout << "\nAll Available Products:\n";
        } else {
            cout << "\nProducts in Category: " << category << "\n";
        }

        for (auto &product : products) {
            if (category == "All" || product.getCategory() == category) {
                product.display();
            }
        }
    }

    void addToCart(string name, int quantity) {
        for (auto &product : products) {
            if (product.getName() == name) {
                if (product.getQuantity() >= quantity) {
                    product.updateQuantity(quantity);
                    if (product.hasBogoOffer()) {
                        quantity *= 2;
                        cout << "Special Offer Applied: Buy One Get One Free on " << name << "!\n";
                    }
                    cart.push_back({name, quantity});
                    totalAmount += product.getPrice() * quantity;
                    cout << quantity << " x " << name << " added to the cart.\n";
                    return;
                } else {
                    cout << "Not enough stock for " << name << ".\n";
                    return;
                }
            }
        }
        cout << "Product not found!\n";
    }

    void generateReceipt() {
        if (cart.empty()) {
            cout << "\nCart is empty. No receipt to generate.\n";
            return;
        }

        time_t now = time(0);
        tm *ltm = localtime(&now);
        int receiptNumber = rand() % 10000 + 1000;
        string filename = "receipt_" + to_string(receiptNumber) + ".txt";
        ofstream receiptFile(filename);

        float discount = (totalAmount > DISCOUNT_THRESHOLD) ? (totalAmount * DISCOUNT_RATE) : 0.0;
        float gstAmount = (totalAmount - discount) * GST_RATE;
        float finalAmount = (totalAmount - discount) + gstAmount;

        receiptFile << "=====================================\n";
        receiptFile << "        SUPERMARKET BILLING SYSTEM   \n";
        receiptFile << "=====================================\n";
        receiptFile << "Receipt Number: " << receiptNumber << "\n";
        receiptFile << "Customer: " << currentUser << "\n";
        receiptFile << "Date: " << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday
                    << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\n";
        receiptFile << "-------------------------------------\n";
        receiptFile << "Item\tQty\tPrice\tTotal\n";
        receiptFile << "-------------------------------------\n";

        for (auto &item : cart) {
            for (auto &product : products) {
                if (product.getName() == item.first) {
                    float total = item.second * product.getPrice();
                    receiptFile << item.first << "\t" << item.second << "\t₹" << product.getPrice()
                                << "\t₹" << total << "\n";
                }
            }
        }

        receiptFile << "-------------------------------------\n";
        receiptFile << "Subtotal: ₹" << totalAmount << "\n";
        receiptFile << "Discount: ₹" << discount << "\n";
        receiptFile << "GST (18%): ₹" << gstAmount << "\n";
        receiptFile << "Final Amount: ₹" << finalAmount << "\n";
        receiptFile << "=====================================\n";

        receiptFile.close();
        cout << "\nReceipt has been saved as '" << filename << "'.\n";

        // Payment Method
        processPayment(finalAmount);
    }

    void processPayment(float amount) {
        cout << "\nSelect Payment Method:\n";
        cout << "1. Cash\n";
        cout << "2. Credit/Debit Card\n";
        cout << "3. Net Banking\n";
        cout << "4. UPI (Paytm, Google Pay, PhonePe)\n";
        cout << "Enter choice (1-4): ";

        int choice;
        cin >> choice;

        // Declare variables outside the switch to avoid "jump to case label" error
        string netUsername, upiId;
        long long cardNumber;

        switch (choice) {
            case 1:
                cout << "Payment of ₹" << amount << " made in Cash. Thank you!\n";
                break;
            case 2:
                cout << "Enter Card Number: ";
                cin >> cardNumber;
                cout << "Payment of ₹" << amount << " made using Credit/Debit Card. Thank you!\n";
                break;
            case 3:
                cout << "Enter Net Banking Username: ";
                cin >> netUsername;
                cout << "Payment of ₹" << amount << " made using Net Banking. Thank you!\n";
                break;
            case 4:
                cout << "Enter UPI ID: ";
                cin >> upiId;
                cout << "Payment of ₹" << amount << " made using UPI (e.g., Paytm, Google Pay). Thank you!\n";
                break;
            default:
                cout << "Invalid payment option selected!\n";
                break;
        }
    }
};

int main() {
    Supermarket store;
    store.addProduct("Apple", 10.5, 100, "1001", "Fruits");
    store.addProduct("Milk", 20.0, 30, "3001", "Juices", true);
    store.addProduct("Chips", 10.0, 50, "4001", "Accessories");
    store.addProduct("T-shirt", 500.0, 20, "5001", "Clothes");
    store.addProduct("Banana", 15.0, 50, "1002", "Fruits");
    store.addProduct("Carrot", 30.0, 40, "3002", "Vegetables");

    int option;
    string username, password;

    cout << "1. Register\n2. Login\nChoose option: ";
    cin >> option;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (option == 1) {
        store.registerUser(username, password);
    } else if (option == 2) {
        if (!store.loginUser(username, password)) {
            return 0;
        }
    } else {
        cout << "Invalid choice!\n";
        return 0;
    }

    // Category selection
    string category;
    cout << "\nSelect Category:\n1. Fruits\n2. Vegetables\n3. Juices\n4. Accessories\n5. Clothes\n6. All\n";
    cout << "Enter your choice: ";
    int catChoice;
    cin >> catChoice;

    switch (catChoice) {
        case 1: category = "Fruits"; break;
        case 2: category = "Vegetables"; break;
        case 3: category = "Juices"; break;
        case 4: category = "Accessories"; break;
        case 5: category = "Clothes"; break;
        case 6: category = "All"; break;
        default: category = "All"; break;
    }

    store.displayProductsByCategory(category);

   

    store.addToCart("Chips", 10);
    store.generateReceipt();

    return 0;
}

























