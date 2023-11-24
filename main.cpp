#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <exception>

class Product
{
private:
    std::string name;
    double price;
    int availableQuantity;

public:
    Product() = default;
    Product(std::string name, double price, int availableQuantity)
        : name(name), price(price), availableQuantity(availableQuantity) {}
    Product(const Product &p)
        : name(p.name), price(p.price), availableQuantity(p.availableQuantity) {}
    void setName(std::string name)
    {
        this->name = name;
    }
    std::string getName() const
    {
        return name;
    }
    void setPrice(double price)
    {
        this->price = price;
    }
    double getPrice()
    {
        return price;
    }
    void setAvailableQuantity(int availableQuantity)
    {
        this->availableQuantity = availableQuantity;
    }
    int getAvailableQuantity()
    {
        return availableQuantity;
    }
};

class CartItem
{
private:
    Product product;
    int quantity;

public:
    CartItem() = default;
    CartItem(const Product &p_product, int p_quantity)
        : product(p_product), quantity(p_quantity) {}

    void setProduct(Product product)
    {
        this->product = product;
    }
    Product getProduct()
    {
        return product;
    }
    void setQuantity(int quantity)
    {
        this->quantity = quantity;
    }
    int getQuantity()
    {
        return quantity;
    }
};

// custom exceptions
class InsufficientStockException : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Insufficient stock: stock is less than the quantity you needed";
    }
};

class InvalidInputException : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Invalid Input: Enter a valid quantity";
    }
};

class PaymentFailureException : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "payment failed..";
    }
};

class ShoppingCart
{
private:
    std::vector<CartItem> items;

public:
    void addItems(Product &product, int quantity);
    void displayCart();
    bool paymentProcessing();
};

void ShoppingCart::addItems(Product &product, int quantity)
{
    if (product.getAvailableQuantity() < quantity)
    {
        throw InsufficientStockException();
    }

    bool alreadySet = false;
    // checking if the item that need to be added is already present in the cart
    for (CartItem &cartItem : items)
    {
        if (cartItem.getProduct().getName() == product.getName())
        {
            cartItem.setQuantity(cartItem.getQuantity() + quantity);
            alreadySet = true;
            break;
        }
    }

    if (!alreadySet)
    {
        // if item not in the cart -- add item into the cart
        items.push_back({product, quantity});
    }

    // reduce the available quantity
    int x = product.getAvailableQuantity() - quantity;
    product.setAvailableQuantity(x);

    std::cout << "Item added Successfully " << std::endl;
}

void ShoppingCart::displayCart()
{
    std::cout << "Items present in the cart:" << std::endl;
    double totalCost = 0.0;
    std::cout << "Name" << std::setw(15) << "Quantity" << std::setw(15) << "Price" << std::endl;
    for (CartItem item : items)
    {
        std::cout << item.getProduct().getName() << std::setw(15)
                  << item.getQuantity() << std::setw(15)
                  << item.getProduct().getPrice() << std::endl;
        totalCost += (item.getQuantity() * item.getProduct().getPrice());
    }
    std::cout << "total cost: $" << totalCost << std::endl;
}

bool ShoppingCart::paymentProcessing()
{
    // 10% chance of failure
    if (rand() % 10 == 0)
    {
        throw PaymentFailureException();
    }
    return true;
}

int main()
{

    // available products -- name, price, availableQuantity
    std::vector<Product> productsAvailable = {
        {"Product A", 10.99, 5},
        {"Product B", 5.99, 10},
        {"Product C", 7.49, 8}};

    ShoppingCart shoppingCart;
    try
    {
        // adding items to shopping cart
        char option = 'n';
        do
        {

            std::string productName;
            int productQuantity;
            std::cout << "add item to the cart" << std::endl;
            std::cout << "Enter the product name " << std::endl;
            std::getline(std::cin, productName);

            bool found = false;
            unsigned prod_idx = 0;
            for (; prod_idx < productsAvailable.size(); ++prod_idx)
            {
                const Product &prod = productsAvailable.at(prod_idx);
                if (prod.getName() == productName)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                std::cerr << "Product not found" << std::endl;
            }
            else
            {
                std::cout << "Enter the product quantity " << std::endl;
                std::cin >> productQuantity;

                if (productQuantity <= 0 || !productQuantity)
                {
                    throw InvalidInputException();
                }

                Product &prod_to_cart = productsAvailable.at(prod_idx);
                shoppingCart.addItems(prod_to_cart, productQuantity);
            }

            std::cout << "Do you want to add more item? (y/n)" << std::endl;
            std::cin >> option;
            std::cin.ignore();
        } while (option == 'y' || option == 'Y');

        // displaying shopping cart
        shoppingCart.displayCart();

        srand(time(0));
        // payment processing
        if (shoppingCart.paymentProcessing())
        {
            std::cout << "Payment successful " << std::endl;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}