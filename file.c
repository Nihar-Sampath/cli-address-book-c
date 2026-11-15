#include <stdio.h>
#include "file.h"

//TO UPLOAD CURRENT CONTACTS INTO THE FILE
void saveContactsToFile(AddressBook *addressBook) {
    FILE *fptr = fopen("AddressBook.csv", "w");
    fprintf(fptr, "#%d\n", addressBook->contactCount);

    for(int i=0; i<addressBook->contactCount; i++)
        fprintf(fptr, "%s, %s, %s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
}

//TO GET DATA FROM FILE AND SAVE IT IN ARRAY
void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fptr = fopen("AddressBook.csv", "r");

    //IF FILE DOESNT EXIST. IT SETS CONTACT COUNT TO ZERO MEANING NO DATA HAS BEEN STORED YET.
    if(fptr==NULL)
    {
        addressBook->contactCount=0;
        return;
    }

    //IF FILE EXISTS IT GETS DATA FROM THE FILE AND STORES IN LOCAL ARRAY
    fscanf(fptr, "#%d\n", &addressBook->contactCount);

    for (int i = 0; i < addressBook->contactCount; i++) {
        fscanf(fptr, "%[^,], %[^,], %[^\n]\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    fclose(fptr);
}
