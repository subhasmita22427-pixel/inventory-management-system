#include <stdio.h>
#include <string.h>
#define MAX_ITEMS 100
#define FILE_NAME "inventory.txt"

typedef struct 
{
    int code;
    char label[50];
    int stock;
    float cost;
} Item;

void insertItem(Item list[], int *size);
void listAll(const Item list[], int size);
void fetchItem(const Item list[], int size);
void purchaseItem(Item list[], int size);
void saveToFile(const Item list[], int size);
void loadFromFile(Item list[], int *size);

int main() 
{
    Item inventory[MAX_ITEMS];
    int total = 0;
    int opt;
    loadFromFile(inventory, &total);
    printf("\n STORE INVENTORY MANAGEMENT SYSTEM \n");
    do {
        printf("\nMenu:\n");
        printf("1. Insert / Update Item\n");
        printf("2. Show All Items\n");
        printf("3. Find Item\n");
        printf("4. Purchase Item\n");
        printf("5. Exit\n");
        printf("Select option: ");
        scanf("%d", &opt);

        switch (opt) 
        {
            case 1:
                insertItem(inventory, &total);
                break;
            case 2:
                listAll(inventory, total);
                break;
            case 3:
                fetchItem(inventory, total);
                break;
            case 4:
                purchaseItem(inventory, total);
                break;
            case 5:
                saveToFile(inventory, total);
                printf("\nExiting... All data saved to file.\n");
                break;
            default:
                printf("\nInvalid option. Try again.\n");
        }
    } while (opt != 5);
    return 0;
}

// Add or update item
void insertItem(Item list[], int *size) 
{
    if (*size >= MAX_ITEMS) 
    { printf("\nStorage full. Cannot add more.\n");
        return;
    }

    Item temp;
    printf("\nEnter item code: ");
    scanf("%d", &temp.code);

    for (int i = 0; i < *size; i++) 
    {
        if (list[i].code == temp.code) 
        {
            int choice, qty;
            printf("\nItem found: %s (Current stock: %d)\n", list[i].label, list[i].stock);
            printf("1. Add stock\n2. Deduct stock\nEnter choice: ");
            scanf("%d", &choice);
            if (choice == 1) 
            { printf("Enter quantity to add: ");
                scanf("%d", &qty);
                list[i].stock += qty;
                printf("\nStock updated successfully!\n");
            } 
            else if (choice == 2) 
            { printf("Enter quantity to deduct: ");
                scanf("%d", &qty);
                if (qty > list[i].stock)
                    printf("\nNot enough stock!\n");
                else 
                { list[i].stock -= qty;
                    printf("\nStock updated successfully!\n");
                }
            } 
            else 
                printf("\nInvalid choice.\n");
            return;
        }
    }
    // add New item
    printf("Enter item name: ");
    scanf("%s", temp.label);
    printf("Enter stock quantity: ");
    scanf("%d", &temp.stock);
    printf("Enter unit cost: ");
    scanf("%f", &temp.cost);
    list[*size] = temp;
    (*size)++;
    printf("\nItem added successfully.\n");
}
// Show all items in tablular form
void listAll(const Item list[], int size) 
{
    if (size == 0) 
    {
        printf("\nNo items found in inventory.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s %-15s %-12s\n", "CODE", "NAME", "STOCK", " UNIT COST", "TOTAL VALUE");
    for (int i = 0; i < size; i++) 
    {
        printf("%-10d %-20s %-10d %-10.2f %-12.2f\n",
               list[i].code, list[i].label, list[i].stock, list[i].cost, list[i].stock * list[i].cost);
    }
}

// Find item by code or name
void fetchItem(const Item list[], int size) 
{
    if (size == 0) 
    { printf("\nInventory is empty.\n"); return; }

    int opt;
    printf("\nFind by:\n1. Code\n2. Name\nEnter choice: ");
    scanf("%d", &opt);
    int code;
    char name[50];
    int found = 0;

    if (opt == 1) 
    {
        printf("Enter item code: ");
        scanf("%d", &code);
        for (int i = 0; i < size; i++) 
        { if (list[i].code == code) 
            { printf("\nItem Found:\nCode: %d\nName: %s\nStock: %d\nCost: %.2f\nTotal: %.2f\n",
                list[i].code, list[i].label, list[i].stock, list[i].cost, list[i].stock * list[i].cost);
                found = 1;
                break;
            }
        }
    } 
    else if (opt == 2) 
    {
        printf("Enter item name: ");
        scanf("%s", name);
        for (int i = 0; i < size; i++) 
        { if (strcmp(list[i].label, name) == 0) 
            {  printf("\nItem Found:\nCode: %d\nName: %s\nStock: %d\nCost: %.2f\nTotal: %.2f\n",
                 list[i].code, list[i].label, list[i].stock, list[i].cost, list[i].stock * list[i].cost);
                found = 1;
                break;
            }
        }
    } 
    else 
        printf("\nInvalid option.\n");
    if (!found) 
        printf("\nNo matching item found.\n");
}

// Purchase item
void purchaseItem(Item list[], int size) 
{
    if (size == 0) 
    { printf("\nNo items available for purchase.\n");
        return; }
    int code, qty, found = 0;
    printf("\nEnter item code to purchase: ");
    scanf("%d", &code);
    for (int i = 0; i < size; i++) 
    { if (list[i].code == code) 
        {
            found = 1;
            printf("Item: %s | Price: %.2f | Available: %d\n", list[i].label, list[i].cost, list[i].stock);
            printf("Enter quantity to buy: ");
            scanf("%d", &qty);
            if (qty > list[i].stock)
            {
                printf("\nNot enough stock available!\n");
                return;
            }
            float total = qty * list[i].cost;
            list[i].stock -= qty;
            printf("\nPurchase successful!\nTotal cost: %.2f\nRemaining stock: %d\n", total, list[i].stock);
            return;
        }
    }
    if (!found)
        printf("\nItem not found.\n");
}
// Save inventory to a text file 
void saveToFile(const Item list[], int size)
{
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL)
    {
        printf("\nError saving to file!\n");
        return;
    }
    fprintf(fp, "%-10s %-20s %-10s %-10s\n", "CODE", "NAME", "STOCK", "UNIT COST");
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%-10d %-20s %-10d %-10.2f\n",
                list[i].code, list[i].label, list[i].stock, list[i].cost);
    }

    fclose(fp);
    printf("\nInventory saved to %s successfully.\n", FILE_NAME);
}
void loadFromFile(Item list[], int *size)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    { *size = 0;
        printf("\nNo previous file found.\n");
        return;
    }
    *size = 0;
    while (fscanf(fp, "%d %s %d %f",
                  &list[*size].code,
                  list[*size].label,
                  &list[*size].stock,
                  &list[*size].cost) == 4)
    { (*size)++; }
    fclose(fp);
    printf("\n Loaded %d items from %s.\n", *size, FILE_NAME);
}
