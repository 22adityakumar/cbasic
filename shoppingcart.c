#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME 20
#define MAX_PASSWORD 20

// Product structure
typedef struct Product {
    char name[50];
    int id;
    int qty;
    int price;
    struct Product* next;
} Product;

// User structure
typedef struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
} User;

// Shopping cart structure
typedef struct ShoppingCart {
    Product* head;
    int count;
} ShoppingCart;

// Function to create a new product
Product* createProduct(char name[], int id, int qty, int price) {
    Product* newproduct = (Product*)malloc(sizeof(Product));
    if (newproduct == NULL) {
        printf("Memory Allocation failed");
        exit(EXIT_FAILURE);
    }

    newproduct->id = id;
    strcpy(newproduct->name, name);
    newproduct->price = price;
    newproduct->qty = qty;
    newproduct->next = NULL;
    return newproduct;
}

// Function to add a product to the product list
void addproduct(Product** head, int price, int qty, int id, char name[]) {
    Product* newproduct = createProduct(name, id, qty, price);
    if (*head == NULL) {
        *head = newproduct;
        return;
    }

    Product* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newproduct;
}

// Function to display all products
void displayProducts(Product* head) {
    if (head == NULL) {
        printf("No Products to display\n");
        return;
    }

    printf("=== Product List ===\n");
    printf("ID   | Name                         | Price | Quantity\n");
    printf("----------------------------------------------------\n");

    Product* current = head;
    while (current != NULL) {
        printf("%-5d| %-30s| %-6d| %-8d\n", current->id, current->name, current->price, current->qty);
        current = current->next;
    }
}

// Function to calculate the total price of items in the shopping cart
float calculateTotalPrice(ShoppingCart* cart) {
    float totalPrice = 0;
    Product* current = cart->head;
    while (current != NULL) {
        totalPrice += current->price * current->qty; // Multiply by quantity
        current = current->next;
    }
    return totalPrice;
}

// Function for user login
void login(User* user) {
    printf("Enter your username: ");
    scanf("%s", user->username);
    printf("Enter your password: ");
    scanf("%s", user->password);
}

// Function to display items in the shopping cart
void displayCart(ShoppingCart* cart) {
    printf("=== Shopping Cart ===\n");
    if (cart == NULL || cart->head == NULL) {
        printf("The cart is empty.\n");
        return;
    }

    printf("ID   | Name                         | Price | Quantity\n");
    printf("----------------------------------------------------\n");

    Product* current = cart->head;
    while (current != NULL) {
        printf("%-5d| %-30s| %-6d| %-8d\n", current->id, current->name, current->price, current->qty);
        current = current->next;
    }

    printf("----------------------------------------------------\n");
    printf("Total Price: %.2f\n", calculateTotalPrice(cart));
}

int main() {
    Product* productList = NULL; // Product list
    ShoppingCart* cart = (ShoppingCart*)malloc(sizeof(ShoppingCart)); // Shopping cart
    cart->head = NULL; // Initialize cart
    cart->count = 0;   // Initialize cart count
    User user;

    printf("==== MENU ====\n");
    printf("1. Input Products\n");
    printf("2. User Login\n");
    printf("3. Display Product List\n");
    printf("4. Add Product to Cart\n");
    printf("5. Display Shopping Cart\n");
    printf("6. Quit\n");

    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Product Name: ");
                char name[50];
                scanf("%s", name);
                printf("Enter Product ID: ");
                int id;
                scanf("%d", &id);
                printf("Enter Product Quantity: ");
                int qty;
                scanf("%d", &qty);
                printf("Enter Product Price: ");
                int price;
                scanf("%d", &price);
                addproduct(&productList, price, qty, id, name);
                break;

            case 2:
                login(&user);
                break;

            case 3:
                displayProducts(productList);
                break;

            case 4:
                printf("Enter the product ID you want to buy: ");
                int productId;
                scanf("%d", &productId);

                Product* current = productList;
                int found = 0;
                while (current != NULL) {
                    if (current->id == productId) {
                        if (current->qty > 0) {
                            printf("Enter quantity: ");
                            int qty;
                            scanf("%d", &qty);
                            if (qty <= current->qty) {
                                printf("%d of Product %s added to cart\n", qty, current->name);
                                current->qty -= qty;

                                // Add product to cart
                                Product* newProduct = createProduct(current->name, current->id, qty, current->price);
                                newProduct->next = cart->head;
                                cart->head = newProduct;
                                cart->count++;
                            } else {
                                printf("Not enough stock available.\n");
                            }
                        } else {
                            printf("Product %s is out of stock\n", current->name);
                        }
                        found = 1;
                        break;
                    }
                    current = current->next;
                }

                if (!found) {
                    printf("Product with ID %d not found\n", productId);
                }
                break;

            case 5:
                displayCart(cart);
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);

    // Free dynamically allocated memory
    Product* temp = productList;
    while (temp != NULL) {
        Product* next = temp->next;
        free(temp);
        temp = next;
    }
    temp = cart->head;
    while (temp != NULL) {
        Product* next = temp->next;
        free(temp);
        temp = next;
    }
    free(cart);

    return 0;
}
