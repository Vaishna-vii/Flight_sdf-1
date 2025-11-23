// Flight Booking System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // For strcmp() and strcpy()

// ------------------ DATA STRUCTURES ------------------
typedef struct Flight {
    char flightNo[10];      // Flight number identifier
    char destination[20];   // Destination city
    char date[12];          // Flight date
    char time[10];          // Flight time
    int totalSeats;         // Total seats on flight
    int availableSeats;     // Seats left for booking
    float fare;             // Ticket price
}flight;

typedef struct Booking {
    char passengerName[30];     // Passenger's name
    int passengerID;            // Unique passenger ID
    char flightNo[10];          // Flight number booked
    int bookingID;              // Unique booking ID
}booking;

// Function prototypes
void viewFlights();
void searchFlights();
void bookTicket();
void cancelTicket();
void viewMyBookings();

// File names
const char flightsFile[] = "flights_record.txt";
const char bookingsFile[] = "bookings.txt";

// Main Function
int main() {
    int choice;
    while (1)  {
        printf("\n=============================\n");
        printf("   FLIGHT BOOKING SYSTEM\n");
        printf("=============================\n");
        printf("1. View All Flights\n");
        printf("2. Search Flights\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. View My Bookings\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice){
        case 1:
            viewFlights();       // List all available flights
            break;
        case 2:
            searchFlights();     // Search by destination
            break;
        case 3:
            bookTicket();        // Make a new booking
            break;
        case 4:
            cancelTicket();      // Cancel an existing booking
            break;
        case 5:
            viewMyBookings();   // Show bookings for a passenger
            break;
        case 6:
            printf("Thank you for using our services.\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
}

// ===================== VIEW ALL FLIGHTS =====================
void viewFlights(){
     printf("\n----- VIEW ALL FLIGHTS -----\n"); // header
    FILE *fp = fopen(flightsFile, "r");          // open flight records in read mode
    struct Flight f;
    if (!fp){ //file missing or cannot open
        printf("No flight records found.\n");
        return;
    }
    printf("\n===== AVAILABLE FLIGHTS =====\n");
    // Read each line of the file into struct f
    // fscanf returns number of items successfully read (7 here)
    while (fscanf(fp, "%s %s %s %s %d %d %f",
                  f.flightNo, f.destination, f.date, f.time,
                  &f.totalSeats, &f.availableSeats, &f.fare) != EOF){
        printf("\nFlight No: %s\nDestination: %s\nDate: %s\nTime: %s\nSeats Left: %d\nFare: %.2f\n",
               f.flightNo, f.destination, f.date, f.time,
               f.availableSeats, f.fare);
    }
    fclose(fp);     // Close file to free resources
}

// ===================== SEARCH FLIGHTS =====================
void searchFlights(){
    char dest[20];
    printf("\n----- SEARCH FLIGHTS -----\n"); // header
    FILE *fp = fopen(flightsFile, "r");      // open flight records in read mode
    flight f;
    int found = 0;
    if (!fp){//file missing or cannot open
        printf("No flights available.\n");
        return;
    }
    printf("Enter destination to search: ");
    scanf("%s", dest);

    printf("\n===== SEARCH RESULTS =====\n");

     // Loop through all flight records
    while (fscanf(fp, "%s %s %s %s %d %d %f",
                  f.flightNo, f.destination, f.date, f.time,
                  &f.totalSeats, &f.availableSeats, &f.fare) != EOF)  {
        // Compare flight destination with user input
        if (strcmp(f.destination, dest) == 0){
            found = 1;    // flight matches destination
            printf("\nFlight No: %s\nDate: %s\nTime: %s\nSeats Left: %d\nFare: %.2f\n",
                   f.flightNo, f.date, f.time, f.availableSeats, f.fare);
        }
    }
    if (!found)
        printf("No flights found for destination: %s\n", dest);

    fclose(fp);     // Close file to free resources
}

// ===================== BOOK TICKET =====================
void bookTicket(){
    printf("\n----- BOOK TICKET -----\n"); // header
    FILE *fp = fopen(flightsFile, "r");           // read flights
    FILE *temp = fopen("temp.txt", "w");          // temp file to update seats
    FILE *bp = fopen(bookingsFile, "a");          // append new booking

    flight f;
    booking b;
    int found = 0;
    if (!fp || !temp || !bp){//missing file or cannot open
        printf("Error accessing files.\n");
        return;
    }
    printf("Enter Flight Number: ");
    scanf("%s", b.flightNo);
    printf("Enter Passenger Name: ");
    scanf("%s", b.passengerName);
    printf("Enter Passenger ID: ");
    scanf("%d", &b.passengerID);

    b.bookingID = rand() % 90000 + 10000;       //Random 5-digit booking ID

    // Process each flight record
    while (fscanf(fp, "%s %s %s %s %d %d %f",
                  f.flightNo, f.destination, f.date, f.time,
                  &f.totalSeats, &f.availableSeats, &f.fare) != EOF)  {
        if (strcmp(f.flightNo, b.flightNo) == 0){
            // Flight matches user's choice
            if (f.availableSeats > 0){
                found = 1;    // seats available
                f.availableSeats--; // Reduce seat count

                fprintf(bp, "%s %d %s %d\n",
                        b.passengerName, b.passengerID,
                        b.flightNo, b.bookingID);   // Save booking

                printf("\nBooking Successful!\nYour Booking ID: %d\n", b.bookingID);
            }
            else {
                printf("No seats available on this flight.\n"); // fully booked
            }
        }
        fprintf(temp, "%s %s %s %s %d %d %f\n",
                f.flightNo, f.destination, f.date, f.time,
                f.totalSeats, f.availableSeats, f.fare);// write updated seats
    }
    if (!found)
        printf("Flight not found.\n");

    remove(flightsFile);
    rename("temp.txt", flightsFile);    // update flight records
    // Close file to free resources
    fclose(fp);
    fclose(temp);
    fclose(bp);
}

// ===================== CANCEL TICKET =====================
void cancelTicket(){
    printf("\n----- CANCEL TICKET -----\n"); // header
    int bookingID;
    printf("Enter Booking ID: ");
    scanf("%d", &bookingID);

    FILE *bp = fopen(bookingsFile, "r");      // read bookings
    FILE *tempB = fopen("tempB.txt", "w");    // temp for updated bookings
    FILE *fp = fopen(flightsFile, "r");       // read flights
    FILE *tempF = fopen("tempF.txt", "w");    // temp for updated flights

    booking b;
    flight f;
    char flightNoToRestore[10];
    int found = 0;

    if (!bp || !tempB || !fp || !tempF){
        printf("Error accessing files.\n");
        return;
    }
    // Read all bookings
    while (fscanf(bp, "%s %d %s %d",
                  b.passengerName, &b.passengerID,
                  b.flightNo, &b.bookingID) != EOF)  {
        if (b.bookingID == bookingID){
            found = 1;  // Booking found → cancel
            strcpy(flightNoToRestore, b.flightNo);  // save flight to restore seat
        }
        else { // Keep other bookings
            fprintf(tempB, "%s %d %s %d\n",
                    b.passengerName, b.passengerID,
                    b.flightNo, b.bookingID);       // keep other bookings
        }
    }
    // Restore seat in the flight file
    while (fscanf(fp, "%s %s %s %s %d %d %f",
                  f.flightNo, f.destination, f.date, f.time,
                  &f.totalSeats, &f.availableSeats, &f.fare) != EOF)  {
        if (strcmp(f.flightNo, flightNoToRestore) == 0)
            f.availableSeats++; // restore seat

        fprintf(tempF, "%s %s %s %s %d %d %f\n",
                f.flightNo, f.destination, f.date, f.time,
                f.totalSeats, f.availableSeats, f.fare);
    }
    // Replace files with updated versions
    remove(bookingsFile);
    rename("tempB.txt", bookingsFile);
    remove(flightsFile);
    rename("tempF.txt", flightsFile);
    if (found)
        printf("Booking Cancelled Successfully.\n");
    else
        printf("Booking ID not found.\n");
    // Close file to free resources
    fclose(bp);
    fclose(tempB);
    fclose(fp);
    fclose(tempF);
}

// ===================== VIEW MY BOOKINGS =====================
void viewMyBookings() {
    printf("\n----- VIEW MY BOOKINGS -----\n"); // header
    int pid;
    booking b;

    FILE *bp = fopen(bookingsFile, "r");        //read bookings

    if (!bp){
        printf("No bookings found.\n");
        return;
    }

    printf("Enter your Passenger ID: ");
    scanf("%d", &pid);

    printf("\n===== YOUR BOOKINGS =====\n");
    int found = 0;
    // Loop through all bookings
    while (fscanf(bp, "%s %d %s %d",
                  b.passengerName, &b.passengerID,
                  b.flightNo, &b.bookingID) != EOF) {
        if (b.passengerID == pid){
            found = 1;
            printf("\nName: %s\nFlight: %s\nBooking ID: %d\n",
                   b.passengerName, b.flightNo, b.bookingID);
        }
    }

    if (!found)
        printf("No bookings found for this ID.\n");

    fclose(bp);     // Close file to free resources
}
