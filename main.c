#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passengerData {
    unsigned int seatNo; //passenger seat Number
    char lastName[15]; //passenger's last name
    char firstName[10]; //passenger's first name
};

//prototypes
unsigned int enterchoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main()
{
    FILE *pfPtr; //passenger data file pointer
    unsigned int choice; //user's choice

    //fopen opens the file; exits if file cannot be opened
    if ((pfPtr = fopen("passData.dat", "rb+")) == NULL) {
        puts("File could not be opened.");
    }//end if
    else {
        //enable user to specify action
        while((choice = enterchoice()) != 5) {
            switch(choice){
                //create textfile from record file
                case 1:
                    textFile(pfPtr);
                    break;
                //update record
                case 2:
                    updateRecord(pfPtr);
                    break;
                //create record
                case 3:
                    newRecord(pfPtr);
                    break;
                //delete existing record
                case 4:
                    deleteRecord(pfPtr);
                    break;
                //display message if user does not select valid choice
                default:
                    puts("Incorrect Choice!");
                    break;
            }//end switch
        }//end while
        fclose(pfPtr);
    }//end else
    return 0;
}//end main



//create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; //passenger.dat file
    int result; //used to test if fread read any bytes

    //create passengerData with default information
    struct passengerData blankRec = {0, "", ""};

    //fopen opens the file; exits if the file cannot be opened
    if ((writePtr = fopen("PassData.txt","w")) == NULL) {
        puts("File could not be opened.");
    }//end if
    else {
        rewind(readPtr); //Sets pointer to beginning of file
        fprintf(writePtr, "%-8s%-14s%-8s\n", "SeatNo.", "Last Name", "First Name");

        //copy all records from random-access file into text file
        while (!feof(readPtr)) {
            result = fread(&blankRec, sizeof(struct passengerData), 1, readPtr);

            //write single record to text file
            if (result != 0 && blankRec.seatNo != 0) {
                fprintf(writePtr, "%-8d%-14s%-8s\n", blankRec.seatNo, blankRec.lastName, blankRec.firstName);
            }//end if
        }//end while
        fclose(writePtr); //fclose closes the file
    }//end else
}//end function textFile


//update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account; //account number
    char newLastName[15];
    char newFirstName[10];

    //create passengerData with default information
    struct passengerData blankRec = {0, "", ""};

    //obtain number of account to update
    printf("%s", "Enter account to update (1 - 10): ");
    scanf("%d", &account);

    //move file pointer to correct record in file
    fseek(fPtr, (account -1) * sizeof(struct passengerData), SEEK_SET);

    //read record from file
    fread(&blankRec, sizeof(struct passengerData), 1, fPtr);

    //display error if account does not exit
    if (blankRec.seatNo == 0){
        printf("%s", "Account #%d has no information.\n", account);
    }//end if
    else {// update record
        printf("%-8d%-16s%-11s\n\n", blankRec.seatNo, blankRec.lastName, blankRec.firstName);

        //request new information from user
        printf("%s", "Enter new last name and new first name: ");
        scanf("%s%s", newLastName, newFirstName);
        strcpy(blankRec.lastName, newLastName);
        strcpy(blankRec.firstName, newFirstName);

        printf("%-8d%-16s%-11s\n", blankRec.seatNo, blankRec.lastName, blankRec.firstName);

        //move file pointer to correct record in file
        fseek(fPtr, (account - 1) * sizeof(struct passengerData), SEEK_SET);

        //write updated record over old record in file
        fwrite(&blankRec, sizeof(struct passengerData), 1, fPtr);
    }//end else
}//end function updateRecord


//delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct passengerData passd; //stores record read from file
    struct passengerData blankRec = {0, "", ""};

    unsigned int accountNum; //account number

    //obtain number of account to delete
    printf("%s", "Enter number of account to delete (1 - 10): ");
    scanf("%d", &accountNum);

    //move file pointer to record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct passengerData), SEEK_SET);

    //read record from file
    fread(&passd, sizeof(struct passengerData), 1, fPtr);

    //display error if record does not exist
    if (passd.seatNo == 0) {
        printf("%s", "Account %d does not exist.\n", accountNum);
    }//end if
    else {//delete record
        //move file pointer to correct record in file
        fseek(fPtr, (accountNum - 1) * sizeof(struct passengerData), SEEK_SET);

        //replace existing record with blank record
        fwrite(&blankRec, sizeof(struct passengerData), 1, fPtr);
    }//end else
}//end function deleteRecord


//create and insert a record
void newRecord(FILE *fPtr)
{
    //create passenger data with default information
    struct passengerData blankRec = {0, "", ""};

    unsigned int accountNum; //accountNum

    //obtain number of account to create
    printf("%s", "Enter new account number (1 - 10): ");
    scanf("%d", &accountNum);

    //move file pointer to correct record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct passengerData), SEEK_SET);

    //read record from file
    fread(&blankRec, sizeof(struct passengerData), 1, fPtr);

    //display error if account already exists
    if (blankRec.seatNo != 0) {
        printf("%d", "Account #%d already contains information.\n,", blankRec.seatNo);
    }//end if
    else {//create record
        //user enters last name and first name
        printf("%s", "Enter last name, first name\n> ");
        scanf("%14s%9s", blankRec.lastName, blankRec.firstName);

        blankRec.seatNo = accountNum;

        //move file pointer to correct record in file
        fseek(fPtr, (blankRec.seatNo - 1) * sizeof(struct passengerData), SEEK_SET);

        //insert record in file
        fwrite(&blankRec, sizeof(struct passengerData), 1, fPtr);
    }//end else
} //end function newRecord


//enable user to input menu choice
unsigned int enterchoice(void)
{
    unsigned int menuChoice; //variable to store user's input

    //display available options
    printf("%s", "\nEnter your choice\n"
           "1. - Store formatted text file of passengers called\n"
           "     \passData.txt\ for printing\n"
           "2. - update an account\n"
           "3. - add a new account\n"
           "4. - delete an account\n"
           "5. - end program\n> ");

           scanf("%u",&menuChoice); //receives choice from user
           return menuChoice;
}//end function enterchoice
