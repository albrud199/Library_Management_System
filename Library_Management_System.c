#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// GEMINI
// Define an enum for error codes
typedef enum {
    ERROR_NONE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_BOOK_NOT_FOUND,
    ERROR_NULL_POINTER,
    ERROR_FILE_IO // Not used in this code, but good to include for completeness
} ErrorCode;

// Function to print error messages to stderr
void printError(ErrorCode error) {
    fprintf(stderr, "Error: ");
    switch (error) {
        case ERROR_MEMORY_ALLOCATION:
            fprintf(stderr, "Memory allocation failed.\n");
            break;
        case ERROR_INVALID_INPUT:
            fprintf(stderr, "Invalid input. Please check your input.\n");
            break;
        case ERROR_BOOK_NOT_FOUND:
            fprintf(stderr, "Book not found in the library.\n");
            break;
        case ERROR_NULL_POINTER:
            fprintf(stderr, "Null pointer error encountered.\n");
            break;
        case ERROR_FILE_IO:
            fprintf(stderr, "File input/output error.\n");
            break;
        case ERROR_NONE: // No error, so no message needed
        default:
            fprintf(stderr, "An unspecified error occurred.\n");
            break;
    }
}

struct Book {
    char *title;
    char *author;
    int year;
};

struct Library {
    struct Book *books;
    int numBooks;
};

// Function to add a book to the library with error handling
ErrorCode addBook(struct Library *library, const char *title, const char *author, int year) {
    if (library == NULL) {
        return ERROR_NULL_POINTER;
    }
    if (title == NULL || author == NULL) {
        return ERROR_INVALID_INPUT;
    }

    struct Book *tempBooks = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (tempBooks == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    library->books = tempBooks;

    char *newTitle = malloc(strlen(title) + 1);
    char *newAuthor = malloc(strlen(author) + 1);

    if (newTitle == NULL || newAuthor == NULL) {
        free(newTitle); // free title if allocated, even if author alloc fails
        free(newAuthor); // free author if allocated, even if title alloc fails. Though one of them might be NULL due to alloc failure, free(NULL) is safe.
        return ERROR_MEMORY_ALLOCATION;
    }

    strcpy(newTitle, title);
    strcpy(newAuthor, author);

    library->books[library->numBooks].title = newTitle;
    library->books[library->numBooks].author = newAuthor;
    library->books[library->numBooks].year = year;

    library->numBooks++;
    printf("Book added successfully.\n");
    return ERROR_NONE;
}

// Function to display all books with error handling
ErrorCode displayBooks(struct Library *library) {
    if (library == NULL) {
        return ERROR_NULL_POINTER;
    }

    printf("\nBooks in the library:\n");
    if (library->numBooks == 0) {
        printf("No books in the library.\n");
        return ERROR_NONE;
    }

    for (int i = 0; i < library->numBooks; i++) {
        if (library->books[i].title == NULL || library->books[i].author == NULL) {
            fprintf(stderr, "Warning: Incomplete book information at index %d.\n", i);
            printf("%d. Title: <Unknown>, Author: <Unknown>, Year: %d\n", i + 1, library->books[i].year);
        } else {
            printf("%d. Title: %s, Author: %s, Year: %d\n", i + 1,
                   library->books[i].title,
                   library->books[i].author,
                   library->books[i].year);
        }
    }
    return ERROR_NONE;
}

// Function to search for a book by title with error handling
ErrorCode searchBook(struct Library *library, const char *title) {
    if (library == NULL || title == NULL) {
        return ERROR_NULL_POINTER;
    }

    printf("\nSearch results:\n");
    for (int i = 0; i < library->numBooks; i++) {
        if (library->books[i].title != NULL && strcmp(library->books[i].title, title) == 0) {
            printf("Title: %s, Author: %s, Year: %d\n",
                   library->books[i].title,
                   library->books[i].author,
                   library->books[i].year);
            return ERROR_NONE; // Book found
        }
    }
    printf("Book not found.\n");
    return ERROR_BOOK_NOT_FOUND;
}

// Function to delete a book by title with error handling
ErrorCode deleteBook(struct Library *library, const char *title) {
    if (library == NULL || title == NULL) {
        return ERROR_NULL_POINTER;
    }

    int bookIndex = -1;
    for (int i = 0; i < library->numBooks; i++) {
        if (library->books[i].title != NULL && strcmp(library->books[i].title, title) == 0) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found.\n");
        return ERROR_BOOK_NOT_FOUND;
    }

    free(library->books[bookIndex].title);
    free(library->books[bookIndex].author);

    for (int j = bookIndex; j < library->numBooks - 1; j++) {
        library->books[j] = library->books[j + 1];
    }

    library->numBooks--;
    if (library->numBooks > 0) { // Only realloc if there are still books
        struct Book *tempBooks = realloc(library->books, library->numBooks * sizeof(struct Book));
        if (tempBooks == NULL && library->numBooks > 0) { // Realloc failed, but it's not critical for deletion functionality, just memory might be slightly more than needed.
            fprintf(stderr, "Warning: Reallocation failed after deleting a book. Memory might not be optimally used.\n");
            // In this case, we proceed without reallocation to avoid further complication and potential data loss.
            // In a critical application, you might need to handle this more robustly, possibly by marking the library in an error state.
        } else {
            library->books = tempBooks;
        }
    } else {
        free(library->books); // Free if no books left, set to NULL for consistency
        library->books = NULL;
    }

    printf("Book deleted successfully.\n");
    return ERROR_NONE;
}

// Function to free allocated memory with NULL check
ErrorCode freeLibrary(struct Library *library) {
    if (library == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (library->books != NULL) {
        for (int i = 0; i < library->numBooks; i++) {
            free(library->books[i].title);
            free(library->books[i].author);
        }
        free(library->books);
    }
    library->books = NULL; // Set to NULL to prevent dangling pointer
    library->numBooks = 0;
    return ERROR_NONE;
}

// Main function with enhanced input validation and error handling
int main() {
    struct Library library;
    library.books = NULL;
    library.numBooks = 0;

    int choice;
    char inputBuffer[256]; // Buffer to read input

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            printf("Error reading input.\n");
            choice = 5; // Exit on input error
            continue;
        }

        if (sscanf(inputBuffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            choice = 0; // Invalid choice, loop again
            continue;
        }

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;

                printf("Enter book title: ");
                if (fgets(title, sizeof(title), stdin) == NULL) {
                    printf("Error reading title.\n");
                    break;
                }
                title[strcspn(title, "\n")] = 0; // Remove trailing newline

                printf("Enter author name: ");
                if (fgets(author, sizeof(author), stdin) == NULL) {
                    printf("Error reading author.\n");
                    break;
                }
                author[strcspn(author, "\n")] = 0; // Remove trailing newline

                printf("Enter publication year: ");
                if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
                    printf("Error reading year.\n");
                    break;
                }
                if (sscanf(inputBuffer, "%d", &year) != 1) {
                    printf("Invalid year format. Please enter a number.\n");
                    break;
                }

                ErrorCode result = addBook(&library, title, author, year);
                if (result != ERROR_NONE) {
                    printError(result);
                }
                break;
            }
            case 2:
                displayBooks(&library);
                break;
            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                if (fgets(searchTitle, sizeof(searchTitle), stdin) == NULL) {
                    printf("Error reading search title.\n");
                    break;
                }
                searchTitle[strcspn(searchTitle, "\n")] = 0; // Remove trailing newline
                searchBook(&library, searchTitle);
                break;
            }
            case 4: {
                char deleteTitle[100];
                printf("Enter title to delete: ");
                if (fgets(deleteTitle, sizeof(deleteTitle), stdin) == NULL) {
                    printf("Error reading delete title.\n");
                    break;
                }
                deleteTitle[strcspn(deleteTitle, "\n")] = 0; // Remove trailing newline
                ErrorCode result = deleteBook(&library, deleteTitle);
                if (result != ERROR_NONE && result != ERROR_BOOK_NOT_FOUND) { // Only print error if it's not "book not found" which is normal flow
                    printError(result);
                }
                break;
            }
            case 5:
                printf("Exiting program...\n");
                ErrorCode result_free = freeLibrary(&library);
                if (result_free != ERROR_NONE && result_free != ERROR_NULL_POINTER) {
                    printError(result_free); // Though freeing library should ideally not fail unless program logic is severely flawed.
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

=========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//ds
struct Book {
    char *title;
    char *author;
    int year;
};

struct Library {
    struct Book *books;
    int numBooks;
};

void addBook(struct Library *library, const char *title, const char *author, int year) {
    if (title == NULL || author == NULL) {
        printf("Error: Title or author is NULL.\n");
        return;
    }

    struct Book *temp = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (temp == NULL) {
        printf("Memory allocation failed. Book not added.\n");
        return;
    }
    library->books = temp;

    library->books[library->numBooks].title = malloc(strlen(title) + 1);
    if (library->books[library->numBooks].title == NULL) {
        printf("Memory allocation failed for title.\n");
        struct Book *temp2 = realloc(library->books, library->numBooks * sizeof(struct Book));
        if (temp2 != NULL) {
            library->books = temp2;
        } else {
            printf("Failed to shrink books array.\n");
        }
        return;
    }

    library->books[library->numBooks].author = malloc(strlen(author) + 1);
    if (library->books[library->numBooks].author == NULL) {
        printf("Memory allocation failed for author.\n");
        free(library->books[library->numBooks].title);
        struct Book *temp2 = realloc(library->books, library->numBooks * sizeof(struct Book));
        if (temp2 != NULL) {
            library->books = temp2;
        } else {
            printf("Failed to shrink books array.\n");
        }
        return;
    }

    strcpy(library->books[library->numBooks].title, title);
    strcpy(library->books[library->numBooks].author, author);
    library->books[library->numBooks].year = year;

    library->numBooks++;
    printf("Book added successfully.\n");
}

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

void deleteBook(struct Library *library, const char *title) {
    for (int i = 0; i < library->numBooks; i++) {
        if (strcmp(library->books[i].title, title) == 0) {
            free(library->books[i].title);
            free(library->books[i].author);

            for (int j = i; j < library->numBooks - 1; j++) {
                library->books[j] = library->books[j + 1];
            }

            library->numBooks--;

            if (library->numBooks == 0) {
                free(library->books);
                library->books = NULL;
            } else {
                struct Book *temp = realloc(library->books, library->numBooks * sizeof(struct Book));
                if (temp == NULL) {
                    printf("Warning: Memory shrink failed after deletion.\n");
                } else {
                    library->books = temp;
                }
            }

            printf("Book deleted successfully.\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void freeLibrary(struct Library *library) {
    for (int i = 0; i < library->numBooks; i++) {
        free(library->books[i].title);
        free(library->books[i].author);
    }
    free(library->books);
    library->books = NULL;
    library->numBooks = 0;
}

int main() {
    struct Library library = {NULL, 0};
    int choice;

    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        int result = scanf("%d", &choice);
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Clear input buffer

        if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;

                printf("Enter book title: ");
                if (scanf("%99[^\n]", title) != 1) {
                    printf("Invalid title.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                while ((c = getchar()) != '\n' && c != EOF);

                printf("Enter author name: ");
                if (scanf("%99[^\n]", author) != 1) {
                    printf("Invalid author.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                while ((c = getchar()) != '\n' && c != EOF);

                printf("Enter publication year: ");
                if (scanf("%d", &year) != 1) {
                    printf("Invalid year.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                while ((c = getchar()) != '\n' && c != EOF);

                addBook(&library, title, author, year);
                break;
            }
            case 2:
                displayBooks(&library);
                break;
            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                if (scanf("%99[^\n]", searchTitle) != 1) {
                    printf("Invalid title.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                while ((c = getchar()) != '\n' && c != EOF);
                searchBook(&library, searchTitle);
                break;
            }
            case 4: {
                char deleteTitle[100];
                printf("Enter title to delete: ");
                if (scanf("%99[^\n]", deleteTitle) != 1) {
                    printf("Invalid title.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                while ((c = getchar()) != '\n' && c != EOF);
                deleteBook(&library, deleteTitle);
                break;
            }
            case 5:
                printf("Exiting program...\n");
                freeLibrary(&library);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

======================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//copilot
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
    // Check for NULL pointers
    if (library == NULL || title == NULL || author == NULL) {
        printf("Invalid input.\n");
        return;
    }

    library->books = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (library->books == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    library->books[library->numBooks].title = malloc(strlen(title) + 1);
    library->books[library->numBooks].author = malloc(strlen(author) + 1);
    
    if (library->books[library->numBooks].title == NULL || library->books[library->numBooks].author == NULL) {
        printf("Memory allocation failed.\n");
        free(library->books[library->numBooks].title);
        free(library->books[library->numBooks].author);
        return;
    }
    
    strcpy(library->books[library->numBooks].title, title);
    strcpy(library->books[library->numBooks].author, author);
    library->books[library->numBooks].year = year;
    
    library->numBooks++;
    printf("Book added successfully.\n");
}

// Function to display all books
void displayBooks(const struct Library *library) {
    // Check for NULL pointer
    if (library == NULL) {
        printf("Invalid library.\n");
        return;
    }
    
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
void searchBook(const struct Library *library, const char *title) {
    // Check for NULL pointers
    if (library == NULL || title == NULL) {
        printf("Invalid input.\n");
        return;
    }

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

// Function to delete a book by title
void deleteBook(struct Library *library, const char *title) {
    // Check for NULL pointers
    if (library == NULL || title == NULL) {
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < library->numBooks; i++) {
        if (strcmp(library->books[i].title, title) == 0) {
            free(library->books[i].title);
            free(library->books[i].author);
            
            for (int j = i; j < library->numBooks - 1; j++) {
                library->books[j] = library->books[j + 1];
            }
            
            library->numBooks--;
            library->books = realloc(library->books, library->numBooks * sizeof(struct Book));
            if (library->numBooks > 0 && library->books == NULL) {
                printf("Memory allocation failed.\n");
                return;
            }

            printf("Book deleted successfully.\n");
            return;
        }
    }
    printf("Book not found.\n");
}

// Function to free allocated memory
void freeLibrary(struct Library *library) {
    // Check for NULL pointer
    if (library == NULL) {
        return;
    }

    for (int i = 0; i < library->numBooks; i++) {
        free(library->books[i].title);
        free(library->books[i].author);
    }
    free(library->books);
}

// Main function
int main() {
    struct Library library;
    library.books = NULL;
    library.numBooks = 0;
    
    int choice;
    
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }
        getchar();

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;
                printf("Enter book title: ");
                if (scanf(" %[^\n]s", title) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                printf("Enter author name: ");
                if (scanf(" %[^\n]s", author) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                printf("Enter publication year: ");
                if (scanf("%d", &year) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                addBook(&library, title, author, year);
                break;
            }
            case 2:
                displayBooks(&library);
                break;
            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                if (scanf(" %[^\n]s", searchTitle) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                searchBook(&library, searchTitle);
                break;
            }
            case 4: {
                char deleteTitle[100];
                printf("Enter title to delete: ");
                if (scanf(" %[^\n]s", deleteTitle) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                deleteBook(&library, deleteTitle);
                break;
            }
            case 5:
                printf("Exiting program...\n");
                freeLibrary(&library);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}


===================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// chatgpt
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
    if (!title || !author || year <= 0) {
        printf("Invalid book details provided.\n");
        return;
    }

    struct Book *newBooks = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (!newBooks) {
        printf("Memory allocation failed.\n");
        return;
    }
    library->books = newBooks;
    
    library->books[library->numBooks].title = malloc(strlen(title) + 1);
    library->books[library->numBooks].author = malloc(strlen(author) + 1);
    
    if (!library->books[library->numBooks].title || !library->books[library->numBooks].author) {
        printf("Memory allocation failed.\n");
        free(library->books[library->numBooks].title);
        free(library->books[library->numBooks].author);
        return;
    }
    
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
    if (!title) {
        printf("Invalid title input.\n");
        return;
    }
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

// Function to delete a book by title
void deleteBook(struct Library *library, const char *title) {
    if (!title) {
        printf("Invalid title input.\n");
        return;
    }
    for (int i = 0; i < library->numBooks; i++) {
        if (strcmp(library->books[i].title, title) == 0) {
            free(library->books[i].title);
            free(library->books[i].author);
            
            for (int j = i; j < library->numBooks - 1; j++) {
                library->books[j] = library->books[j + 1];
            }
            
            library->numBooks--;
            struct Book *newBooks = realloc(library->books, library->numBooks * sizeof(struct Book));
            if (!newBooks && library->numBooks > 0) {
                printf("Memory reallocation failed after deletion.\n");
                return;
            }
            library->books = newBooks;
            printf("Book deleted successfully.\n");
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
    struct Library library = {NULL, 0};
    int choice;
    
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;
                printf("Enter book title: ");
                if (!fgets(title, sizeof(title), stdin)) {
                    printf("Error reading input.\n");
                    continue;
                }
                title[strcspn(title, "\n")] = 0;
                printf("Enter author name: ");
                if (!fgets(author, sizeof(author), stdin)) {
                    printf("Error reading input.\n");
                    continue;
                }
                author[strcspn(author, "\n")] = 0;
                printf("Enter publication year: ");
                if (scanf("%d", &year) != 1) {
                    printf("Invalid year input.\n");
                    while (getchar() != '\n');
                    continue;
                }
                getchar();
                addBook(&library, title, author, year);
                break;
            }
            case 2:
                displayBooks(&library);
                break;
            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                if (!fgets(searchTitle, sizeof(searchTitle), stdin)) {
                    printf("Error reading input.\n");
                    continue;
                }
                searchTitle[strcspn(searchTitle, "\n")] = 0;
                searchBook(&library, searchTitle);
                break;
            }
            case 4: {
                char deleteTitle[100];
                printf("Enter title to delete: ");
                if (!fgets(deleteTitle, sizeof(deleteTitle), stdin)) {
                    printf("Error reading input.\n");
                    continue;
                }
                deleteTitle[strcspn(deleteTitle, "\n")] = 0;
                deleteBook(&library, deleteTitle);
                break;
            }
            case 5:
                printf("Exiting program...\n");
                freeLibrary(&library);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}


=============
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


==========================================

    deepseekr1
    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    char title[100];
    char author[100];
    int year;
};

struct Library {
    struct Book *books;  // Dynamic array of books
    int numBooks;        // Current number of books
    int capacity;        // Allocated capacity
};

void addBook(struct Library *lib, const char *title, const char *author, int year) {
    if (lib->numBooks >= lib->capacity) {
        int new_capacity = (lib->capacity == 0) ? 1 : lib->capacity * 2;
        struct Book *new_books = (struct Book *)realloc(lib->books, new_capacity * sizeof(struct Book));
        if (!new_books) {
            printf("Failed to allocate memory. Cannot add book.\n");
            return;
        }
        lib->books = new_books;
        lib->capacity = new_capacity;
    }

    struct Book newBook;
    strncpy(newBook.title, title, sizeof(newBook.title) - 1);
    newBook.title[sizeof(newBook.title) - 1] = '\0';
    strncpy(newBook.author, author, sizeof(newBook.author) - 1);
    newBook.author[sizeof(newBook.author) - 1] = '\0';
    newBook.year = year;

    lib->books[lib->numBooks] = newBook;
    lib->numBooks++;
    printf("Book added successfully.\n");
}

void displayBooks(struct Library library) {
    printf("Books in the library:\n");
    if (library.numBooks == 0) {
        printf("No books in the library.\n");
        return;
    }
    
    for (int i = 0; i < library.numBooks; i++) {
        printf("%d. Title: %s, Author: %s, Year: %d\n", i + 1,
               library.books[i].title,
               library.books[i].author,
               library.books[i].year);
    }
}

void searchBook(struct Library library, const char *title) {
    printf("Search results:\n");
    
    for (int i = 0; i < library.numBooks; i++) {
        if (strcmp(library.books[i].title, title) == 0) {
            printf("Title: %s, Author: %s, Year: %d\n",
                   library.books[i].title,
                   library.books[i].author,
                   library.books[i].year);
            return;
        }
    }
    printf("Book not found.\n");
}

int main() {
    struct Library library;
    library.books = NULL;
    library.numBooks = 0;
    library.capacity = 0;
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
                char title[100];
                char author[100];
                int year;

                printf("Enter book title: ");
                scanf(" %99[^\n]", title);
                getchar(); // Consume newline

                printf("Enter author name: ");
                scanf(" %99[^\n]", author);
                getchar();

                printf("Enter publication year: ");
                scanf("%d", &year);
                getchar();

                addBook(&library, title, author, year);
                break;
            }

            case 2:
                displayBooks(library);
                break;

            case 3: {
                char searchTitle[100];
                printf("Enter title to search: ");
                scanf(" %99[^\n]", searchTitle);
                getchar();

                searchBook(library, searchTitle);
                break;
            }

            case 4:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    free(library.books); // Free dynamically allocated memory

    return 0;
}

==============================

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
    library->books = realloc(library->books, (library->numBooks + 1) * sizeof(struct Book));
    if (library->books == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    library->books[library->numBooks].title = malloc(strlen(title) + 1);
    library->books[library->numBooks].author = malloc(strlen(author) + 1);
    
    if (library->books[library->numBooks].title == NULL || library->books[library->numBooks].author == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
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

// Function to delete a book by title
void deleteBook(struct Library *library, const char *title) {
    for (int i = 0; i < library->numBooks; i++) {
        if (strcmp(library->books[i].title, title) == 0) {
            free(library->books[i].title);
            free(library->books[i].author);
            
            for (int j = i; j < library->numBooks - 1; j++) {
                library->books[j] = library->books[j + 1];
            }
            
            library->numBooks--;
            library->books = realloc(library->books, library->numBooks * sizeof(struct Book));
            printf("Book deleted successfully.\n");
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
    library.books = NULL;
    library.numBooks = 0;
    
    int choice;
    
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                char title[100], author[100];
                int year;
                printf("Enter book title: ");
                scanf(" %[^\n]s", title);
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
            case 4: {
                char deleteTitle[100];
                printf("Enter title to delete: ");
                scanf(" %[^\n]s", deleteTitle);
                deleteBook(&library, deleteTitle);
                break;
            }
            case 5:
                printf("Exiting program...\n");
                freeLibrary(&library);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}

