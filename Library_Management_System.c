#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    char *title;
    char *author;
    int year;
};

struct Library {
    struct Book *books;
    int numBooks;
};

// Function to add a book to the library
void addBook(struct Library *library, const char *title, const char *author, int year) {
    // Allocate/Reallocate memory for new book
    library->books = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (library->books == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Allocate memory for book title and author
    library->books[library->numBooks].title = malloc(strlen(title) + 1);
    library->books[library->numBooks].author = malloc(strlen(author) + 1);
    
    if (library->books[library->numBooks].title == NULL || library->books[library->numBooks].author == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Copy book details
    strcpy(library->books[library->numBooks].title, title);
    strcpy(library->books[library->numBooks].author, author);
    library->books[library->numBooks].year = year;

    library->numBooks++;
    printf("Book added successfully.\n");
}

// Function to display all books
void displayBooks(struct Library *library) {
    printf("\nBooks in the library:\n");
    
    if (library->numBooks == 0) {
        printf("No books in the library.\n");
        return;
    }
    
    for (int i = 0; i < library->numBooks; i++) {
        printf("%d. Title: %s, Author: %s, Year: %d\n", i + 1,
               library->books[i].title,
               library->books[i].author,
               library->books[i].year);
    }
}

// Function to search for a book by title
void searchBook(struct Library *library, const char *title) {
    printf("\nSearch results:\n");
    
    for (int i = 0; i < library->numBooks; i++) {
        if (strcmp(library->books[i].title, title) == 0) {
            printf("Title: %s, Author: %s, Year: %d\n",
                   library->books[i].title,
                   library->books[i].author,
                   library->books[i].year);
            return;
        }
    }
    printf("Book not found.\n");
}

// Function to free allocated memory
void freeLibrary(struct Library *library) {
    for (int i = 0; i < library->numBooks; i++) {
        free(library->books[i].title);
        free(library->books[i].author);
    }
    free(library->books);
}

// Main function
int main() {
    struct Library library;
    library.books = NULL;  // Initialize pointer to NULL
    library.numBooks = 0;
    
    int choice;

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;

                printf("Enter book title: ");
                scanf(" %[^\n]s", title); // Accept multi-word input

                printf("Enter author name: ");
                scanf(" %[^\n]s", author);

                printf("Enter publication year: ");
                scanf("%d", &year);

                addBook(&library, title, author, year);
                break;
            }

            case 2:
                displayBooks(&library);
                break;

            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                scanf(" %[^\n]s", searchTitle);
                searchBook(&library, searchTitle);
                break;
            }

            case 4:
                printf("Exiting program...\n");
                freeLibrary(&library);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}
