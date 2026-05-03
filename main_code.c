#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -------- STRUCTS -------- */
struct Train {
    int train_no;
    char from[50], to[50];
    char name[50];
    int seats;
    int fare;
};

struct Passenger {
    char name[50];
    char gender[10];
    int age;
};

/* -------- LOWERCASE -------- */
void toLower(char str[]) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

/* -------- SEARCH -------- */
void search_trains(char source[], char dest[]) {
    FILE *fp;
    struct Train t;
    int found = 0;

    fp = fopen("train_data.txt", "r");
    if (fp == NULL) {
        printf("File not found!\n");
        return;
    }

    toLower(source);
    toLower(dest);

    printf("\nAvailable Trains:\n");

    while (fscanf(fp, "%d %s %s %s %d %d",
        &t.train_no, t.from, t.to, t.name, &t.seats, &t.fare) == 6) {

        char f[50], d[50];
        strcpy(f, t.from);
        strcpy(d, t.to);

        toLower(f);
        toLower(d);

        if (strcmp(source, f) == 0 && strcmp(dest, d) == 0) {
            printf("Train No:%d | Name:%s | Seats:%d | Fare:%d\n",
                   t.train_no, t.name, t.seats, t.fare);
            found = 1;
        }
    }

    if (!found)
        printf(" No trains available\n");

    fclose(fp);
}

/* -------- BOOK -------- */
void book_ticket(int train_no, char date[]) {
    FILE *fp, *temp, *book;
    struct Train t;
    struct Passenger p[10];
    int tickets, found = 0;
    int total;

    printf("Number of Tickets: ");
    scanf("%d", &tickets);

    for (int i = 0; i < tickets; i++) {
        printf("\nPassenger %d\n", i + 1);

        printf("Name: ");
        scanf("%s", p[i].name);

        printf("Gender (M/F): ");
        scanf("%s", p[i].gender);

        printf("Age: ");
        scanf("%d", &p[i].age);
    }

    fp = fopen("train_data.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("File error!\n");
        return;
    }

    while (fscanf(fp, "%d %s %s %s %d %d",
        &t.train_no, t.from, t.to, t.name, &t.seats, &t.fare) == 6) {

        if (t.train_no == train_no && t.seats >= tickets) {
            t.seats -= tickets;
            found = 1;

            total = t.fare * tickets;

            book = fopen("bookings.txt", "a");

            for (int i = 0; i < tickets; i++) {
                fprintf(book, "%d %s %s %s %d %s %s %s %d\n",
                        t.train_no, t.from, t.to, t.name,
                        t.fare, date,
                        p[i].name, p[i].gender, p[i].age);
            }

            fclose(book);

            printf("\n BOOKING SUCCESSFUL\n");
            printf("Train: %s (%d)\n", t.name, t.train_no);
            printf("Tickets: %d\n", tickets);
            printf("Date: %s\n", date);
            for (int i = 0; i < tickets; i++) {
                printf("Passenger %d: %s | %s | %d\n",
                       i + 1, p[i].name, p[i].gender, p[i].age);
            }

            printf("Fare per ticket: %d\n", t.fare);
            printf(" Total Amount: %d\n", total);
        }

        fprintf(temp, "%d %s %s %s %d %d\n",
                t.train_no, t.from, t.to, t.name, t.seats, t.fare);
    }

    fclose(fp);
    fclose(temp);

    remove("train_data.txt");
    rename("temp.txt", "train_data.txt");

    if (!found)
        printf(" Booking Failed\n");
}

/* -------- CANCEL -------- */
void cancel_ticket(int train_no) {
    FILE *book, *temp, *cancel;
    int tn, age, fare;
    char from[50], to[50], tname[50], date[20];
    char pname[50], gender[10], cancel_name[50];
    int found = 0;

    printf("Enter Passenger Name: ");
    scanf("%s", cancel_name);

    book = fopen("bookings.txt", "r");
    temp = fopen("temp_book.txt", "w");
    cancel = fopen("cancelled.txt", "a");

    if (book == NULL) {
        printf("No bookings found\n");
        return;
    }

    while (fscanf(book, "%d %s %s %s %d %s %s %s %d",
        &tn, from, to, tname, &fare, date, pname, gender, &age) == 9) {

        if (tn == train_no && strcmp(pname, cancel_name) == 0 && !found) {

            fprintf(cancel, "%d %s %s %s %d %s %s %s %d\n",
                    tn, from, to, tname, fare, date, pname, gender, age);

            printf(" Ticket cancelled for %s\n", pname);
            found = 1;
            continue;
        }

        fprintf(temp, "%d %s %s %s %d %s %s %s %d\n",
                tn, from, to, tname, fare, date, pname, gender, age);
    }

    fclose(book);
    fclose(temp);
    fclose(cancel);

    remove("bookings.txt");
    rename("temp_book.txt", "bookings.txt");

    if (!found)
        printf(" Ticket not found\n");
}

/* -------- MAIN -------- */
int main() {
    int choice, train_no;
    char from[50], to[50], date[20];

    while (1) {
        printf("\n--- Train Management System ---\n");
        printf("1.Search\n2.Book\n3.Cancel\n4.Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("From: ");
            scanf("%s", from);
            printf("To: ");
            scanf("%s", to);
            search_trains(from, to);
            break;

        case 2:
            printf("Train No: ");
            scanf("%d", &train_no);
            printf("Date: ");
            scanf("%s", date);
            book_ticket(train_no, date);
            break;

        case 3:
            printf("Train No: ");
            scanf("%d", &train_no);
            cancel_ticket(train_no);
            break;

        case 4:
            exit(0);

        default:
            printf("Invalid choice\n");
        }
    }
}