#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 256

// Structure for a song (used in BST)
typedef struct Song {
    char title[100];
    char artist[100];
    struct Song *left, *right;
} Song;

// Function prototypes
Song* insertSong(Song* root, char title[], char artist[]);
void inOrder(Song* root);
Song* searchSong(Song* root, char title[]);
Song* findMin(Song* root);
Song* deleteSong(Song* root, char title[]);
Song* loadSongsFromFile(Song* root, const char* filename);

Song* insertSong(Song* root, char title[], char artist[]) {
    if (root == NULL) {
        Song* newSong = (Song*)malloc(sizeof(Song));
        strcpy(newSong->title, title);
        strcpy(newSong->artist, artist);
        newSong->left = newSong->right = NULL;
        return newSong;
    }
    if (strcmp(title, root->title) < 0)
        root->left = insertSong(root->left, title, artist);
    else if (strcmp(title, root->title) > 0)
        root->right = insertSong(root->right, title, artist);
    else
        printf("Song already exists!\n");
    return root;
}

void inOrder(Song* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("Title: %s, Artist: %s\n", root->title, root->artist);
        inOrder(root->right);
    }
}

Song* searchSong(Song* root, char title[]) {
    if (root == NULL || strcmp(root->title, title) == 0)
        return root;
    if (strcmp(title, root->title) < 0)
        return searchSong(root->left, title);
    return searchSong(root->right, title);
}

Song* findMin(Song* root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

Song* deleteSong(Song* root, char title[]) {
    if (root == NULL) return root;
    if (strcmp(title, root->title) < 0)
        root->left = deleteSong(root->left, title);
    else if (strcmp(title, root->title) > 0)
        root->right = deleteSong(root->right, title);
    else {
        if (root->left == NULL) {
            Song* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Song* temp = root->left;
            free(root);
            return temp;
        }
        Song* temp = findMin(root->right);
        strcpy(root->title, temp->title);
        strcpy(root->artist, temp->artist);
        root->right = deleteSong(root->right, temp->title);
    }
    return root;
}

// Load songs from a file in the format: title|artist
Song* loadSongsFromFile(Song* root, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s\n", filename);
        return root;
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Remove newline
        char *token = strtok(line, "|");
        if (token == NULL) continue;
        char title[100];
        strcpy(title, token);
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        char artist[100];
        strcpy(artist, token);
        root = insertSong(root, title, artist);
    }
    fclose(file);
    return root;
}

int main() {
    Song* root = NULL;
    int choice;
    char title[100], artist[100];

    // Load music library from file (change file name as needed)
    root = loadSongsFromFile(root, "music_library.txt");

    do {
        printf("\nMusic Library Manager\n");
        printf("1. Add Song\n");
        printf("2. Search Song\n");
        printf("3. Display All Songs\n");
        printf("4. Delete Song\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1:
                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                printf("Enter artist name: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = '\0';
                root = insertSong(root, title, artist);
                printf("Song added successfully!\n");
                break;
            case 2:
                printf("Enter song title to search: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                if (searchSong(root, title))
                    printf("Song found in library!\n");
                else
                    printf("Song not found.\n");
                break;
            case 3:
                printf("\nAll Songs:\n");
                inOrder(root);
                break;
            case 4:
                printf("Enter song title to delete: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';
                root = deleteSong(root, title);
                printf("Song deleted if it existed.\n");
                break;
            case 5:
                printf("Exiting... Thank you for using Music Library Manager!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);
    
    return 0;
}
