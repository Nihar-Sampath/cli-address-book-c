#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "contact.h"
#include "file.h"

//TEMPERORY BUFFER TO STORE INDEXES DURING SEARCH OPERATIONS
//IF MORE THAN ONE CONTACTS ARE MATCHING WITH USER INPUT THEIR INDEXES WERE STORES IN TEMPERORY ARRAY AND USED LATER ON. 
//TEMP WAS TAKEN AS 100 BY CONSIDERING WORST CASE SCENARIO WHERE USER INPUT IS MATCHES WITH ALL EXISTING CONTACTS
//SIZE INDICATES NUMBER OF USER INPUTS ARE MATCHING
int temp[100];
int size=0;

//SAVES CONTACTS INTO FILE AND EXITS
void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

//INITIALIZES BY GETTING DATA FROM FILE
void initialize(AddressBook *addressBook)
{
    loadContactsFromFile(addressBook);
}
//TO CREATE NEW CONTACT
void createContact(AddressBook *addressBook)
{
    //CHECKS IF THE LIST IS FULL OR NOT. IF LIST IS FULL PRINTS A MESSAGE AND EXITS
	if(addressBook->contactCount>=100)
	{
	    printf("List Full can't add new contacts.\n");
	    return;
	}
    
    //IF LIST NOT FULL CREATES 3 BUFFERS FOR STORING NAME, PHONE NO., EMAIL
    char nameb[40], phoneb[11], emailb[40];
    bool valid=0;
    
    //VALIDATING NAME USING validate_name FUNCTION. IF NAME IS INVALID PERSISTANTLY ASKS USER TO REENTER
    while(!valid)
    {
        printf("Enter name of the contact: ");
        scanf(" %39[^\n]", nameb);
        
        valid=validate_name(nameb);

        if(valid==0)
            printf("Invalid Input.\n");
    }
    
    //VALIDATING PHONE NUMBER USING validate_phone FUNCTION. IF PHONE NO. IS INVALID PERSISTANTLY ASKS USER TO REENTER
    valid=0;
    while(!valid)
    {
        printf("Enter phone number of the contact: ");
        scanf(" %10s", phoneb);
        
        //IF THE NUMBER IS INVALID OR ALREADY EXISTS IN THE LIST. IT IS CONSIDERED INVALID.
        valid=validate_phone(phoneb) && unique_phone(addressBook, phoneb);

        if(valid==0)
                printf("Invalid Input.\n");
    }
    
    //VALIDATING NAME USING validate_email FUNCTION. IF EMAIL IS INVALID PERSISTANTLY ASKS USER TO REENTER
    valid=0;
    while(!valid)
    {
        printf("Enter email of the contact: ");
        scanf(" %39s", emailb);
        
        //IF THE EMAIL IS INVALID OR ALREADY EXISTS IN THE LIST. IT IS CONSIDERED INVALID.
        valid=validate_email(emailb) && unique_email(addressBook, emailb);

        if(valid==0)
                printf("Invalid Input.\n");
    }
    
    printf("\n<----------CONTACT ADDED SUCCESSFULLY---------->\n\n");
    //COPIES DATA TO THE RESPECTIVE INDEXES AND STRUCTURE MEMBERS AND INCREMENTS CONTACT COUNT 
    strcpy(addressBook->contacts[addressBook->contactCount].name, nameb);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phoneb);
    strcpy(addressBook->contacts[addressBook->contactCount].email, emailb);
    addressBook->contactCount++;
}

//TO CHECK IF THE ENTERED PHONE NUMBER IS UNIQUE OR NOT
bool unique_phone(AddressBook *addressBook, char *phoneb)
{
    if(addressBook->contactCount==0)
        return 1;

    for(int i=0; i<addressBook->contactCount; i++)
    {
        if(!(strcmp(addressBook->contacts[i].phone, phoneb)))
        {
            printf("Phone number already exists.\n");
            return 0;
        }
    }
    return 1;
}

//TO CHECK IF THE ENTERED EMAIL IS UNIQUE OR NOT
bool unique_email(AddressBook *addressBook, char *emailb)
{
    if(addressBook->contactCount==0)
        return 1;

    for(int i=0; i<addressBook->contactCount; i++)
    {
        if(!(strcmp(addressBook->contacts[i].email, emailb)))
        {
            printf("Email already exists.\n");
            return 0;
        }
    }
    return 1;
}

//VALIDATES NAME BY ALLOWING ALPHABETS OR SPACES. IF ANYOTHER CHARACTER FOUND IT IS CONSIDERED INVALID
bool validate_name(char *s)
{
    while(*s)
    {
        if((*s>='a' && *s<='z') || (*s>='A' && *s<='Z') || *s==' ')
            s++;
        else
            return 0;
    }
    return 1;
}

//VALIDATES PHONE NUMBER ONLY ALLOWING NUMBERS. IF ANY OTHER CHARACTERS FOUND IT IS CONSIDERED INVALID
bool validate_phone(char *s)
{
    if(strlen(s)!=10)
        return 0;

    while(*s)
    {
        if(*s>='0' && *s<='9')
            s++;
        else
            return 0;
    }
    return 1;
}

/*
VALIDATES EMAIL USING VARIOUS CASES:
CASE 1: IF EMAIL HAs CAPITAL CHARACTERS FROM A......Z IT IS INVALID
CASE 2: IF EMAIL STARTS WITH ANY CHARACTER OTHER THAN LOWER CASE CHARACTERS [a.....z]
CASE 3: IF EMAIL DOESN'T HAs ANY '@' SYMBOL IT IS CONSIDERED INVALID
CASE 4: IF EMAIL HAs MORE THAN 1 '@' SYMBOL IT IS CONSIDERED INVALID
CASE 5: IF EMAIL HAs '.' JUST BEFORE '@' OR RIGHT AFTER '@' IT IS CONSIDERED INVALID
CASE 6: IF EMAIL DOESN'T HAS '.' AFTER '@' IT IS CONSIDERED INVALID
CASE 7: IF EMAIL HAS CONTINUOUS '.' (DOTS) IT IS CONSIDERED INVALID
CASE 8: IF EMAIL ENDS WITH '@' OR '.' IT IS CONSIDERED INVALID
*/
bool validate_email(char *s)
{
    if(!(s[0]>='a' && s[0]<='z'))
        return 0;

    s++;
    if(!strchr(s, '@'))
        return 0;

    bool atr=0;

    
    while(*s)
    {
        if((*s>='a' && *s<='z') || (*s>='0' && *s<='9'))
            s++;
        else if(*s=='@')
        {
            if(atr==1)
                return 0;
            else
                atr=1;

            if(*(s+1)=='.' || *(s-1)=='.')
                return 0;

            s++;

            if(!(strchr(s, '.')))
                return 0;
        }
        else if(*s=='.')
        {
            if(*(s+1)=='.')
                return 0;
            s++;
        }
        else
        {
            return 0;
        }
    }
    if(*(s-1)=='.' || *(s-1)=='@')
        return 0;

    return 1;
}

//SEARCHES CONTACTS BY CALLING RESPECTIVE FUNCTION
void searchContact(AddressBook *addressBook) 
{
    //IF THERE ARE NO CONTACTS CURRENTLY. NOTIFIES USER TO ADD CONTACTS
    if(addressBook->contactCount==0)
    {
        printf("No contacts to search. Please add contacts.\n");
        return;
    }

    int choice;
    printf("\nSelect one option:\n");
    printf("1.Search by Name.\n2.Search by Phone number.\n3.Search by E-mail.\n4.Go back\n");
    scanf(" %d", &choice);
    
    switch (choice)
    {
        case 1:
        search_by_name(addressBook);
        break;
        
        case 2:
        search_by_phone(addressBook);
        break;
        
        case 3:
        search_by_email(addressBook);
        break;
       
        /*
        REASON FOR RESETTING SIZE TO 0 INSTEAD OF JUST GOING BACK WHEN "USER CHOSE OPTION 4.""
        ASSUME "USER SELECTED  [EDIT CONTACT OR DELETE CONTACT]'
        THEN "SEARCHCONTACT FUNCTION WILL BE CALLED".
        AND IF USER "SELECTS OPTION 4" (GO BACK) IT WILL "RETURN TO   EDIT CONTACT / DELETE CONTACT FUNCTION"
        AND WILL PROMPT A MESSAGE TO THE USER AS "1.EDIT BY NAME, PHONE..... OR 1. DELETE BY NAME, PHONE...."  DUE TO THE GIVEN IF CONDITION "if(size>0)""
        IF WE "RESET SIZE TO ZERO" HERE THE IF CONDITION if(size>0) "WILL BECOME FALSE" AND THE MESSAGE 
        "1.EDIT BY NAME, PHONE..... OR 1. DELETE BY NAME, PHONE...." - WILL NOT BE SHOWN ON THE TERMINAL.
        */
        case 4:
        size=0;
        return;

        default:
        printf("Invalid Input.\n");
    }

}

//SEARCHES INPUT VIA STRCASESTR FUNCTION. SO ALL THE CONTACTS WHICH HAVE THE INPUT NAME AS WHOLE OR AS SUBSTRING WILL BE SHOWN
void search_by_name(AddressBook *addressBook)
{
    char nbuff[40];
    printf("Enter name: ");
    scanf(" %39[^\n]", nbuff);

    char *sptr=NULL;
    size=0;
    printf("\n__________________________________________________________________________________________________________\n");
    printf("| S.No | Name %35c| Phone no.   | Email %34c|\n", ' ', ' ');
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    for(int i=0; i<addressBook->contactCount; i++)
    {
        sptr=strcasestr(addressBook->contacts[i].name, nbuff);
        if(sptr!=NULL)
        {
            printf("| %-4d | %-40s| %-10s  | %-40s|\n", size+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            //ADDS THE INDEX VALUES OF THE CONTACTS WHICH WERE MATCHED WITH INPUT
            temp[size++]=i;
            sptr=NULL;
        }
    }
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    if(size==0)
        printf("\nContact with name \"%s\" not found.\n", nbuff);
}

//SEARCHES FOR PHONE NUMBERS VIA STRSTR SO USER CAN FIND THE CONTACT INFORMATION EVEN IF HE REMEMBERS ONLY FEW CONTINUOUS DIGITS OF A PHONE NUMBER
void search_by_phone(AddressBook *addressBook)
{
    char pbuff[11];
    printf("Enter phone no: ");
    scanf(" %10s", pbuff);
    
    char *sptr=NULL;

    size=0;
    printf("\n__________________________________________________________________________________________________________\n");
    printf("| S.No | Name %35c| Phone no.   | Email %34c|\n", ' ', ' ');
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    for(int i=0; i<addressBook->contactCount; i++)
    {
        sptr=strstr(addressBook->contacts[i].phone, pbuff);
        if(sptr!=NULL)
        {
            printf("| %-4d | %-40s| %-10s  | %-40s|\n", size+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            //ADDS THE INDEX VALUES OF THE CONTACTS WHICH WERE MATCHED WITH INPUT
            temp[size++]=i;
            sptr=NULL;
        }
    }
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    
    if(size==0)
        printf("\nContact with phone number \"%s\" not found.\n", pbuff);
}

//SEARCHES EMAIL VIA STRSTR SO USER CAN EASILY FIND BY .COM, .IN, .ORG...ETC. OR BY ONLY REMEMBERING PART OF THE EMAIL
void search_by_email(AddressBook *addressBook)
{
    char ebuff[40];
    printf("Enter email: ");
    scanf(" %39s", ebuff);
    
    char *sptr=NULL;
    size=0;
    printf("\n__________________________________________________________________________________________________________\n");
    printf("| S.No | Name %35c| Phone no.   | Email %34c|\n", ' ', ' ');
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    for(int i=0; i<addressBook->contactCount; i++)
    {
        sptr=strcasestr(addressBook->contacts[i].email, ebuff);
        if(sptr!=NULL)
        {
            printf("| %-4d | %-40s| %-10s  | %-40s|\n", size+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            //ADDS THE INDEX VALUES OF THE CONTACTS WHICH WERE MATCHED WITH INPUT
            temp[size++]=i;
            sptr=NULL;
        }
    }
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    if(size==0)
        printf("\nContact with email \"%s\" not found.\n", ebuff);
}


//TO EDIT CONTACTS ACCORDING TO USER NEEDS
void editContact(AddressBook *addressBook)
{
	//IF THERE ARE NO CONTACTS. NOTIFIES USER TO ADD CONTACTS BY DISPLAYING A MESSAGE
    if(addressBook->contactCount==0)
    {
        printf("No contacts to edit. Please add contacts.\n");
        return;
    }

    //SERCHES FOR CONTACT FIRST
	searchContact(addressBook);
	if(size>0)
	{
	    int choice;
	    printf("\nSelect one option:\n");
	    printf("1.Edit name\n2.Edit phone\n3.Edit email\n4.Go back\n");
	    scanf(" %d", &choice);
	    
	    switch (choice)
        {
            case 1:
            edit_by_name(addressBook);
            break;
            
            case 2:
            edit_by_phone(addressBook);
            break;

            case 3:
            edit_by_email(addressBook);
            break;

            case 4:
            return;

            default:
            printf("Invalid option.\n");
            break;
        }
    }   
}

//EDITS NAME OF THE CONTACT
void edit_by_name(AddressBook *addressBook)
{
    bool valid=0;
    char nameb[40];

    //IF THERE IS ONLY ONE CONTACT WHICH MATCHES WITH USER INPUT. IT DIRECTLY TAKES INPUT AND EDITS
    if(size==1)
    {
        while(!valid)
        {
            printf("Enter new name of the contact: ");
            scanf(" %39[^\n]", nameb);
        
            valid=validate_name(nameb);
            if(valid==0)
                printf("Invalid Input.\n");
        }
        strcpy(addressBook->contacts[temp[size-1]].name, nameb);
    } //IF MORE THAN ONE CONTACT MATCHES WITH USER INPUT IT DISPLAYS ALL THE MATCHING CONTACTS AND ASKS USER TO ENTER INDEX TO EDIT THAT PARTICULAR CONTACT
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the name you want to edit: ");
            scanf(" %d", &index);
            //CHECKS INDEX TO MAKE SURE IT IS WITHIN RANGE. IF NOT IT ASKS PERSISTANTLY
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                while(!valid)
                {
                    printf("Enter new name of the contact: ");
                    scanf(" %[^\n]", nameb);
                    //VALIDATES NAME
                    valid=validate_name(nameb);
                    if(valid==0)
                        printf("Invalid Input.\n");
                }
                strcpy(addressBook->contacts[temp[index-1]].name, nameb);
                break;
            }
        }
    }
    printf("\n<<==========  NAME EDITED SUCCESSFULLY  ==========>>\n\n");
}

//EDITS PHONE NUMBER 
void edit_by_phone(AddressBook *addressBook)
{
    bool valid=0;
    char phoneb[11];

    //IF THERE IS ONLY ONE CONTACT WHICH MATCHES WITH USER INPUT. IT DIRECTLY TAKES INPUT AND EDITS
    if(size==1)
    {
        while(!valid)
        {
            printf("Enter new phone number of the contact: ");
            scanf(" %10s", phoneb);
            //TO ENSURE NO TWO USER WILL HAVE SAME NUMBERS. IT VERIFIES VIA UNIQUE_PHONE AND MAKES IT INVALID IF IT ALREADY EXISTS
            valid=validate_phone(phoneb) && unique_phone(addressBook, phoneb);
            if(valid==0)
                printf("Invalid Input.\n");
        }
        strcpy(addressBook->contacts[temp[size-1]].phone, phoneb);
    } ////IF MORE THAN ONE CONTACT MATCHES WITH USER INPUT IT DISPLAYS ALL THE MATCHING CONTACTS AND ASKS USER TO ENTER INDEX TO EDIT THAT PARTICULAR CONTACT
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the phone you want to edit: ");
            scanf(" %d", &index);
            //CHECKS INDEX TO MAKE SURE IT IS WITHIN RANGE. IF NOT IT ASKS PERSISTANTLY
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                while(!valid)
                {
                    printf("Enter new phone number of the contact: ");
                    scanf(" %10s", phoneb);
                    //TO ENSURE NO TWO USER WILL HAVE SAME PHONE. IT VERIFIES VIA UNIQUE_PHONE AND MAKES IT INVALID IF IT ALREADY EXISTS
                    valid=validate_phone(phoneb) && unique_phone(addressBook, phoneb);
                    if(valid==0)
                        printf("Invalid Input.\n");
                }
                strcpy(addressBook->contacts[temp[index-1]].phone, phoneb);
                break;
            }
        }
    }
    printf("\n<<==========  PHONE NUMBER EDITED SUCCESSFULLY  ==========>>\n\n");
}

//EDITS EMAIL
void edit_by_email(AddressBook *addressBook)
{
    bool valid=0;
    char emailb[40];
    //IF THERE IS ONLY ONE CONTACT WHICH MATCHES WITH USER INPUT. IT DIRECTLY TAKES INPUT AND EDITS
    if(size==1)
    {    
        while(!valid)
        {
            printf("Enter new email of the contact: ");
            scanf(" %39s", emailb);
            //TO ENSURE NO TWO USER WILL HAVE SAME EMAIL. IT VERIFIES VIA UNIQUE_EMAIL AND MAKES IT INVALID IF IT ALREADY EXISTS
            valid=validate_email(emailb) && unique_email(addressBook, emailb);
            if(valid==0)
                printf("Invalid Input.\n");
        }
        strcpy(addressBook->contacts[temp[size-1]].email, emailb);
    } //IF MORE THAN ONE CONTACT MATCHES WITH USER INPUT IT DISPLAYS ALL THE MATCHING CONTACTS AND ASKS USER TO ENTER INDEX TO EDIT THAT PARTICULAR CONTACT
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the email you want to edit: ");
            scanf(" %d", &index);
            //CHECKS INDEX TO MAKE SURE IT IS WITHIN RANGE. IF NOT IT ASKS PERSISTANTLY
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                while(!valid)
                {
                    printf("Enter new email of the contact: ");
                    scanf(" %39s", emailb);
        
                    valid=validate_email(emailb) && unique_email(addressBook, emailb);
                    if(valid==0)
                        printf("Invalid Input.\n");
                }
                strcpy(addressBook->contacts[temp[index-1]].email, emailb);
                break;
            }
        }
    }
    printf("\n<<==========  EMAIL EDITED SUCCESSFULLY  ==========>>\n\n");
}

//TO DELETE A CONTACT ACCORDING TO USER NEEDS
void deleteContact(AddressBook *addressBook)
{
	//IF THERE ARE NO CONTACTS. NOTIFIES USER TO ADD CONTACTS BY DISPLAYING A MESSAGE
    if(addressBook->contactCount==0)
    {
        printf("No contacts to delete. Please add contacts.\n");
        return;
    }

	searchContact(addressBook);
	if(size>0)
	{
	    int choice;
	    printf("\nSelect one option:\n");
	    printf("1. Delete by name\n2. Delete by phone\n3. Delete by email\n4. Go back to main menu\n");
	    scanf(" %d", &choice);
	    
	    switch (choice)
        {
            case 1:
            delete_by_name(addressBook);
            break;
            
            case 2:
            delete_by_phone(addressBook);
            break;

            case 3:
            delete_by_email(addressBook);
            break;

            case 4:
            return;

            default:
            printf("Invalid option.\n");
            break;
        }
    }   
}

//DELETE USER CONTACT BY NAME
void delete_by_name(AddressBook *addressBook)
{
    //IF THERE IS ONLY ONE MATCHING CONTACT IT REMOVES THAT CONTACT
    if(size==1)
    {
        for(int i=temp[size-1]; i<addressBook->contactCount-1; i++)
            addressBook->contacts[i]=addressBook->contacts[i+1];

        printf("The only contact has been deleted.\n");
        size--;
        addressBook->contactCount--;
        return;
    } //IF THERE ARE MORE THAN ONE MATCHING CONTACTS DELETES CONTACT BY INDEX OF THE LIST WHICH HAS BEEN SHOWN
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the name you want to delete: ");
            scanf(" %d", &index);
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                for(int i=temp[index-1]; i<addressBook->contactCount-1; i++)
                    addressBook->contacts[i]=addressBook->contacts[i+1];

                addressBook->contactCount--;
                break;
            }
        }
    } 
    printf("\n<<==========  CONTACT DELETED SUCCESSFULLY  ==========>>\n\n");
}

void delete_by_phone(AddressBook *addressBook)
{
    if(size==1)
    {
        for(int i=temp[size-1]; i<addressBook->contactCount-1; i++)
            addressBook->contacts[i]=addressBook->contacts[i+1];
        
        printf("The only contact has been deleted.\n");
        size--;
        addressBook->contactCount--;
        return;
    } //IF THERE ARE MORE THAN ONE MATCHING CONTACTS DELETES CONTACT BY INDEX OF THE LIST WHICH HAS BEEN SHOWN
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the phone you want to delete: ");
            scanf(" %d", &index);
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                for(int i=temp[index-1]; i<addressBook->contactCount - 1; i++)
                    addressBook->contacts[i]=addressBook->contacts[i+1];
                
                addressBook->contactCount--;
                break;
            }
        }
    }
    printf("\n<<==========  CONTACT DELETED SUCCESSFULLY  ==========>>\n\n");
}

void delete_by_email(AddressBook *addressBook)
{
    if(size==1)
    {
        for(int i=temp[size-1]; i<addressBook->contactCount-1; i++)
            addressBook->contacts[i]=addressBook->contacts[i+1];

        printf("The only contact has been deleted.\n");
        size--;
        addressBook->contactCount--;
        return;
    } //IF THERE ARE MORE THAN ONE MATCHING CONTACTS DELETES CONTACT BY INDEX OF THE LIST WHICH HAS BEEN SHOWN
    else if(size>1)
    {
        while(1)
        {
            int index;
            printf("Enter index of the email you want to delete: ");
            scanf(" %d", &index);
            if(index>size || index<=0)
            {
                printf("Enter a valid input.\n");
                continue;
            }
            else
            {
                for(int i=temp[index-1]; i<addressBook->contactCount - 1; i++)
                    addressBook->contacts[i]=addressBook->contacts[i+1];

                addressBook->contactCount--;
                break;
            }
        }
    }
    printf("\n<<==========  CONTACT DELETED SUCCESSFULLY  ==========>>\n\n");
}


//TO DISPLAY CONTACTS
void listContacts(AddressBook *addressBook) 
{
    //IF THERE ARE NO CONTACTS. NOTIFIES USER TO ADD CONTACT BY DISPLAYING A MESSAGE
    if(addressBook->contactCount==0)
    {
        printf("No contacts to list. Please add contacts.\n");
        return;
    }
    // Sorting contacts based on names
    for(int i=0; i<addressBook->contactCount; i++)
    {
        for(int j=i+1; j<addressBook->contactCount; j++)
        {
            int n=strcasecmp(addressBook->contacts[i].name, addressBook->contacts[j].name);
            if(n>0)
            {
                Contact temp=addressBook->contacts[i];
                addressBook->contacts[i]=addressBook->contacts[j];
                addressBook->contacts[j]=temp;
            }
        }
    }
    //DISPLAYING ALL THE EXISTING CONTACTS 
    printf("\n__________________________________________________________________________________________________________\n");
    printf("| S.No | Name %35c| Phone no.   | Email %34c|\n", ' ', ' ');
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
    for(int i=0; i<addressBook->contactCount; i++)
    {
        printf("| %-4d | %-40s| %-10s  | %-40s|\n", i+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    printf("|______|_________________________________________|_____________|_________________________________________|\n");
}