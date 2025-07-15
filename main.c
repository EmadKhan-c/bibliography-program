#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bibliography.h"

Entry entries[MAX_ENTRIES];
int count = 0;

void extractField(char* line, char* dest) {
    char *start = strchr(line, '{');
    char *end = strchr(line, '}');
    if (start && end && end > start) {
        strncpy(dest, start + 1, end - start - 1);
        dest[end - start - 1] = '\0';
    } else {
        strcpy(dest, "");
    }
}

void loadEntries(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Could not open file.\n"); return; }

    char line[512];
    Entry temp = {"", "", "", "", 0, ""};
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "@")) temp = (Entry){"", "", "", "", 0, ""};
        else if (strstr(line, "author")) extractField(line, temp.author);
        else if (strstr(line, "title")) extractField(line, temp.title);
        else if (strstr(line, "journal") || strstr(line, "booktitle")) extractField(line, temp.journal);
        else if (strstr(line, "year")) {
            char yearStr[16]; extractField(line, yearStr);
            temp.year = atoi(yearStr);
        }
        else if (strstr(line, "doi")) extractField(line, temp.doi);
        else if (strchr(line, '}') && strlen(temp.title) > 0)
            entries[count++] = temp;
    }

    fclose(file);
    printf("Loaded %d entries.\n", count);
}

void searchByAuthor() {
    char term[MAX_STR];
    printf("Author search: "); scanf(" %[^\n]", term);
    for (int i = 0; i < count; i++)
        if (strstr(entries[i].author, term))
            printf("%s (%d): %s\n", entries[i].author, entries[i].year, entries[i].title);
}

void searchByTitle() {
    char term[MAX_STR];
    printf("Title search: "); scanf(" %[^\n]", term);
    for (int i = 0; i < count; i++)
        if (strstr(entries[i].title, term))
            printf("%s (%d): %s\n", entries[i].author, entries[i].year, entries[i].title);
}

void addEntry(const char* filename) {
    FILE* f = fopen(filename, "a");
    Entry e;
    printf("Type: "); scanf(" %[^\n]", e.type);
    printf("Author: "); scanf(" %[^\n]", e.author);
    printf("Title: "); scanf(" %[^\n]", e.title);
    printf("Journal: "); scanf(" %[^\n]", e.journal);
    printf("Year: "); scanf("%d", &e.year);
    printf("DOI: "); scanf(" %[^\n]", e.doi);

    fprintf(f, "@%s{\n", e.type);
    fprintf(f, "   author = {%s},\n", e.author);
    fprintf(f, "   title = {%s},\n", e.title);
    fprintf(f, "   journal = {%s},\n", e.journal);
    fprintf(f, "   year = {%d},\n", e.year);
    fprintf(f, "   doi = {%s}\n}\n\n", e.doi);

    fclose(f);
    entries[count++] = e;
    printf("Entry added.\n");
}

void menu(const char* filename) {
    int opt;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Search Author\n2. Search Title\n3. Add Entry\n4. Exit\nChoose: ");
        scanf("%d", &opt);
        switch (opt) {
            case 1: searchByAuthor(); break;
            case 2: searchByTitle(); break;
            case 3: addEntry(filename); break;
            case 4: printf("Goodbye!\n"); break;
            default: printf("Invalid.\n");
        }
    } while (opt != 4);
}

int main() {
    const char* filename = "C:/Users/emadk/OneDrive - UWE Bristol/Year 1/Programing for Engineers/Resit MiniCoursework/2425resitbiblio.txt";
    loadEntries(filename);
    menu(filename);
    return 0;
}
