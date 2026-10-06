#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= PRODUCT BST =================

typedef struct Product {
    int id;
    char name[50];
    int stock;
    float price;
    struct Product *left;
    struct Product *right;
} Product;

// ================= TRANSACTION LINKED LIST =================

typedef struct Transaction {
    int productId;
    char productName[50];
    char type[20];
    int quantity;
    struct Transaction *next;
} Transaction;

Transaction *history = NULL;

// Create product
Product* createProduct(int id, char name[], int stock, float price) {
    Product *newProduct = (Product*)malloc(sizeof(Product));

    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->stock = stock;
    newProduct->price = price;
    newProduct->left = NULL;
    newProduct->right = NULL;

    return newProduct;
}

// Insert product into BST
Product* insertProduct(Product *root, int id, char name[], int stock, float price) {
    if (root == NULL)
        return createProduct(id, name, stock, price);

    if (id < root->id)
        root->left = insertProduct(root->left, id, name, stock, price);
    else if (id > root->id)
        root->right = insertProduct(root->right, id, name, stock, price);
    else
        printf("Product ID already exists!\n");

    return root;
}

// Search product
Product* searchProduct(Product *root, int id) {
    if (root == NULL || root->id == id)
        return root;

    if (id < root->id)
        return searchProduct(root->left, id);

    return searchProduct(root->right, id);
}

// Find minimum node
Product* findMin(Product *root) {
    while (root->left != NULL)
        root = root->left;

    return root;
}

// Delete product
Product* deleteProduct(Product *root, int id) {
    if (root == NULL)
        return NULL;

    if (id < root->id)
        root->left = deleteProduct(root->left, id);

    else if (id > root->id)
        root->right = deleteProduct(root->right, id);

    else {
        if (root->left == NULL) {
            Product *temp = root->right;
            free(root);
            return temp;
        }

        if (root->right == NULL) {
            Product *temp = root->left;
            free(root);
            return temp;
        }

        Product *temp = findMin(root->right);

        root->id = temp->id;
        strcpy(root->name, temp->name);
        root->stock = temp->stock;
        root->price = temp->price;

        root->right = deleteProduct(root->right, temp->id);
    }

    return root;
}

// Add transaction
void addTransaction(int id, char name[], char type[], int quantity) {
    Transaction *newTransaction =
        (Transaction*)malloc(sizeof(Transaction));

    newTransaction->productId = id;
    strcpy(newTransaction->productName, name);
    strcpy(newTransaction->type, type);
    newTransaction->quantity = quantity;

    newTransaction->next = history;
    history = newTransaction;
}

// Display transactions
void displayHistory() {
    Transaction *temp = history;

    if (temp == NULL) {
        printf("\nNo transaction history.\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");

    while (temp != NULL) {
        printf("Product ID : %d\n", temp->productId);
        printf("Product    : %s\n", temp->productName);
        printf("Type       : %s\n", temp->type);
        printf("Quantity   : %d\n", temp->quantity);
        printf("----------------------------------------\n");

        temp = temp->next;
    }
}

// Display products using inorder traversal
void displayProducts(Product *root) {
    if (root == NULL)
        return;

    displayProducts(root->left);

    printf("ID: %d | Name: %-20s | Stock: %d | Price: %.2f\n",
           root->id, root->name, root->stock, root->price);

    displayProducts(root->right);
}

// Add product
void addProduct(Product **root) {
    int id, stock;
    float price;
    char name[50];

    printf("\nEnter Product ID: ");
    scanf("%d", &id);

    if (searchProduct(*root, id) != NULL) {
        printf("Product already exists!\n");
        return;
    }

    printf("Enter Product Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Stock: ");
    scanf("%d", &stock);

    printf("Enter Price: ");
    scanf("%f", &price);

    *root = insertProduct(*root, id, name, stock, price);

    printf("Product added successfully!\n");
}

// Global root
Product *root = NULL;

// Search product
void search() {
    int id;

    printf("\nEnter Product ID: ");
    scanf("%d", &id);

    Product *p = searchProduct(root, id);

    if (p == NULL) {
        printf("Product not found!\n");
    } else {
        printf("\nProduct Found!\n");
        printf("ID    : %d\n", p->id);
        printf("Name  : %s\n", p->name);
        printf("Stock : %d\n", p->stock);
        printf("Price : %.2f\n", p->price);
    }
}

// Update stock
void updateStock(Product *root) {
    int id, quantity, choice;

    printf("\nEnter Product ID: ");
    scanf("%d", &id);

    Product *p = searchProduct(root, id);

    if (p == NULL) {
        printf("Product not found!\n");
        return;
    }

    printf("Current Stock: %d\n", p->stock);

    printf("1. Purchase / Add Stock\n");
    printf("2. Sale / Remove Stock\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("Invalid quantity!\n");
        return;
    }

    if (choice == 1) {
        p->stock += quantity;

        addTransaction(p->id, p->name, "PURCHASE", quantity);

        printf("Stock updated successfully!\n");
    }

    else if (choice == 2) {
        if (quantity > p->stock) {
            printf("Insufficient stock!\n");
            return;
        }

        p->stock -= quantity;

        addTransaction(p->id, p->name, "SALE", quantity);

        printf("Stock updated successfully!\n");
    }

    else {
        printf("Invalid choice!\n");
    }
}



// Main
int main() {
    int choice, id;

    root = insertProduct(root, 101, "Laptop", 10, 55000);
    root = insertProduct(root, 102, "Mouse", 25, 800);
    root = insertProduct(root, 103, "Keyboard", 15, 1500);
    root = insertProduct(root, 104, "Monitor", 8, 12000);
    root = insertProduct(root, 105, "Headphones", 20, 2500);

    printf("\n========================================\n");
    printf("       INVENTORY MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    while (1) {
        printf("\n========== MENU ==========\n");
        printf("1. Add Product\n");
        printf("2. Search Product\n");
        printf("3. Update Stock\n");
        printf("4. Delete Product\n");
        printf("5. Display All Products\n");
        printf("6. View Transaction History\n");
        printf("7. Exit\n");
        printf("==========================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addProduct(&root);
                break;

            case 2:
                search();
                break;

            case 3:
                updateStock(root);
                break;

            case 4:
                printf("\nEnter Product ID to delete: ");
                scanf("%d", &id);

                if (searchProduct(root, id) == NULL) {
                    printf("Product not found!\n");
                } else {
                    root = deleteProduct(root, id);
                    printf("Product deleted successfully!\n");
                }
                break;

            case 5:
                printf("\n========== PRODUCT INVENTORY ==========\n");

                if (root == NULL)
                    printf("Inventory is empty.\n");
                else
                    displayProducts(root);

                break;

            case 6:
                displayHistory();
                break;

            case 7:
                printf("\nThank you!\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}