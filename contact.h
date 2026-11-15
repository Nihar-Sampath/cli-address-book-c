#include <stdbool.h>


#ifndef CONTACT_H
#define CONTACT_H


#define MAX_CONTACTS 100

typedef struct {
    char name[40];
    char phone[11];
    char email[40];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount; 
} AddressBook;

//AddressBook addressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);

bool validate_name(char *s);
bool validate_phone(char *s);
bool validate_email(char *s);

bool unique_phone(AddressBook *addressBook, char *phoneb);
bool unique_email(AddressBook *addressBook, char *emailb);

void search_by_name(AddressBook *addressBook);
void search_by_phone(AddressBook *addressBook);
void search_by_email(AddressBook *addressBook);


void edit_by_name(AddressBook *addressBook);
void edit_by_phone(AddressBook *addressBook);
void edit_by_email(AddressBook *addressBook);

void delete_by_name(AddressBook *addressBook);
void delete_by_phone(AddressBook *addressBook);
void delete_by_email(AddressBook *addressBook);

#endif
