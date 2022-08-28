/*
 * Title: ATM System
 * Author: Jesse Bertucci
 * Created: Augest 28, 2022
 *
 * Initially, this ATM system allows a user to either login or create an account
 * 
 * Account information is stored in non-volatile memory in the form of a .txt file.
 * An account file name will being with "account" followed by a four digit account number.
 * Inside the account file, the customer's name, password, and balance are stored.
 * 
 * The total number of accounts the 'Canadian National Bank' has is stored within an 'number of accounts' .txt file and is
 * used for assigning account numbers to new users.
 * 
 * If the user creates an account, a new account file is created.
 * if the user decides to login, they will be presented with a login screen.
 * 
 * The user has five attempts to login.
 * If login is successful, account information is downloaded from account file for use.
 * 
 * Once logged in, the user can then check their balance, deposit money, withdraw month, or exit the system.
 * 
 */ 

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// This function introduces the user to the system, it is the first thing they will see. 
int welcome(){

    int option;

    cout << "Welcome to the Canadian National Bank." << endl;
    cout << "----- Login or Create an Account -----" << endl;   
    cout << "1. Login" << endl;
    cout << "2. Create an Account" << endl;
    cout << "--------------------------------------" << endl;
    
    cin >> option;  

    return option;  
}

// This function creates an account for a new user
int createAnAccount(){

    system("clear");

    string strAccounts;        // Will store the number of accounts read in from 'number of accounts' file
    int intAccounts;           // Will store the converstion from string to int for above

    string accountFileName;    // Will be used to create a new account file
    string firstName;          
    string lastName;
    string password;
    int balance;

    fstream numOfAccounts;
        numOfAccounts.open("numOfAccounts.txt", ios::in); // Reads from a .txt file that contains the total number of accounts the bank has
        if(numOfAccounts.is_open() == false){
            cout << "System Error." << endl;
            return -1;
        }
    getline(numOfAccounts, strAccounts); // Get the number of accounts the bank has
    intAccounts = stoi(strAccounts);     // convert number of accounts from string to int
    numOfAccounts.close();       

    intAccounts++;                           // Assigns a new user the next available account number
    strAccounts = to_string(intAccounts);    // converts new account number to string for creation of account file
   
    numOfAccounts.open("numOfAccounts.txt", ios::out); // Writes to 'number of accounts' file
    if(numOfAccounts.is_open() == false){
        cout << "System Error." << endl;
        return -1;
    }
    numOfAccounts << strAccounts << endl;           // Updates the 'num of accounts' file to reflect the addition of one new account
    numOfAccounts.close();

    accountFileName = "account" + to_string(intAccounts + 1000) + ".txt";   // Assigning new account file name to temp string variable

   fstream accountFile;
        accountFile.open(accountFileName, ios::out); // Creates new account file
        if(accountFile.is_open() == false){
            cout << "Account access has failed." << endl;
            return -1;
        }
    
    cout << "Enter your first name: ";            // Get new user information
    cin >> firstName;
    cout << "Enter your last name: ";
    cin >> lastName;
    cout << "Enter your password: ";
    cin >> password;
    cout << "Enter intial account balance: ";
    cin >> balance;

    accountFile << to_string(intAccounts + 1000) << endl;       // Write new user info to account file
    accountFile << firstName << " " << lastName << endl;
    accountFile << password << endl;
    accountFile << to_string(balance);

    accountFile.close();

    system("clear");

    cout << "Your name is: " << firstName << " " << lastName << endl;      // Reminds user of account number and password
    cout << "Your account number is: " << intAccounts + 1000 << endl;
    cout << "Your password is: " << password << endl;
    cout << "Your initial balance is " << balance << endl;
    cout << "Your account number and password is used to login, please remember them." << endl;
    cout << '\n';

    return 0;
}

// Allows the user to login to their account
// If successful, will send back account details to main by dereferrencing pointers
int login(int *accountNumber, string *name, int *balance){

    string accountNumb;              // Temporary storage of account details
    string accountFileName;
    string accountName;
    string accountPassword;
    string strBalance;

    string passwordAttempt;         // Will be used to determine if correct password was entered

    int accountNum;         

    int loginAttempts = 0;          // Max 5 login attempts
    int successfulLogin = 0;        // Loop control variable
    
    while(loginAttempts < 5 and successfulLogin == 0){
        
        int loginFailed = 0;
        
        system("clear");

        if(loginAttempts > 0){
            cout << "Incorrect account number or password." << endl;
            cout << "You have " << 5 - loginAttempts << " attempts left." << endl;
        }
        
        cout << "Enter your four digit account number: ";
        cin >> accountNum;

        accountFileName = "account" + to_string(accountNum) + ".txt";   // Converts account number to account file name

        fstream accountFile;
            accountFile.open(accountFileName, ios::in); // If account exists, get details
            if(accountFile.is_open()){
                getline(accountFile, accountNumb);
                getline(accountFile, accountName);
                getline(accountFile, accountPassword);
                getline(accountFile, strBalance);
                accountFile.close();
            } else {
                loginFailed = 1;                        // Won't notify user if account does not exist for security reasons
            }

        cout << "Enter your password: ";
        cin >> passwordAttempt;

        if(passwordAttempt == accountPassword and loginFailed == 0){    // Exits the loop is login was successful
            cout << "Login successful." << endl;
            successfulLogin = 1;
        }

        loginAttempts++;
    }

    if(successfulLogin == 1){                // Send account details to main function variables if login was successful
        *accountNumber = accountNum;
        *name = accountName;
        *balance = stoi(strBalance);
    }

    return successfulLogin;
}

// Main menu function, takes the name of the customer as an argument
int menu(string name){
    
    int option;

    system("clear");
    cout << "Glad to see you back " << name << "." << endl;
    cout << "---------------- Menu ----------------" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Exit" << endl;
    cout << "--------------------------------------" << endl;
    cout << "Selection: ";
    cin >> option;

    return option;
}

// deposit function, takes account number as argument
int deposit(int accountNumber){

    system("clear");   

    int deposit;
    int balance;

    string accountNumb;        // Temp storage for account details
    string accountName;
    string accountPassword;
    string strBalance;

    string accountFileName = "account" + to_string(accountNumber) + ".txt";     // Converts account number to account file name

     fstream accountFile;
        accountFile.open(accountFileName, ios::in); // Reads account file
        if(accountFile.is_open() == false){
            cout << "Account does not exist." << endl;
            return -1;
        }

    getline(accountFile, accountNumb);               // Get account details and store them in temp variables
    getline(accountFile, accountName);
    getline(accountFile, accountPassword);
    getline(accountFile, strBalance);

    accountFile.close();

    cout << "Enter the amount you are depositing: ";
    cin >> deposit;

    balance = stoi(strBalance) + deposit;           // Update balance
    strBalance = to_string(balance);                // Convert balance to string for storage into account file

    cout << "Your new balance is: " << balance << endl;

        accountFile.open(accountFileName, ios::out); // Writes to account file
        if(accountFile.is_open() == false){
            cout << "Account does not exist." << endl;
            return -1;
        }

    accountFile << accountNumb << endl;           // Update account file with new balance
    accountFile << accountName << endl;
    accountFile << accountPassword << endl;
    accountFile << strBalance << endl;

    accountFile.close();

    return 0;
}

int withdraw(int accountNumber){

    system("clear");   

    int withdraw;
    int balance;

    string accountNumb;          // Temp storage for account details   
    string accountName;
    string accountPassword;
    string strBalance;

    string accountFileName = "account" + to_string(accountNumber) + ".txt";     // Converts account number to account file name

     fstream accountFile;
        accountFile.open(accountFileName, ios::in); // Reads account file
        if(accountFile.is_open() == false){
            cout << "Account does not exist." << endl;
            return -1;
        }

    getline(accountFile, accountNumb);              // Get account details and store them in temp variables
    getline(accountFile, accountName);
    getline(accountFile, accountPassword);
    getline(accountFile, strBalance);

    accountFile.close();

    cout << "Enter the amount you are withdrawing: ";
    cin >> withdraw;

    balance = stoi(strBalance) - withdraw;          // Update balance
    strBalance = to_string(balance);                // Convert balance to string for storage into account file

    cout << "Your new balance is: " << balance << endl;

        accountFile.open(accountFileName, ios::out); // Writes to account file
        if(accountFile.is_open() == false){
            cout << "Account does not exist." << endl;
            return -1;
        }

    accountFile << accountNumb << endl;              // Update account file with new balance
    accountFile << accountName << endl;
    accountFile << accountPassword << endl;
    accountFile << strBalance << endl;

    accountFile.close();

    return 0;
}

// Display account balance function, takes account balance as an argument
void displayBalance(int balance){
    system("clear");
    cout << "Your balance is: " << balance << endl;
}

// Main will call this function after every cycle through the menu to determine if user is finished with their banking
int returnToMenuOrExit(){
    
    int selection;

    cout << '\n';
    cout << "------------ Return to Main Menu? ------------" << endl;
    cout << "1. Yes." << endl;
    cout << "2. No, finished banking." << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Selection: ";
    cin >> selection;

    return selection;
}

int main(){

    system("clear");

    int welcomeOption = 0;
    int menuOption;

    int option;

    int accountNumber;                 // Temp storage of account details
    string name;
    string password;
    int balance;
    
    while(welcomeOption != 1){        // Loop will return user to main menu if new account is made
        welcomeOption = welcome();
        switch(welcomeOption){
            case 1:
                if(login(&accountNumber, &name, &balance) == 1){
                    cout << "Login successful." << endl;
                } else {
                    cout << "Login failed." << endl;
                    return 0;
                }
                break;
            case 2:
                if(createAnAccount() != 0){
                    return -1;                    
                }
                break;
        }
    }

    int loopControl = 1;
    while(loopControl == 1 and option != 4){           // Will cycle throught the menu options until the user quits
        option = menu(name);
        switch (option){
            case 1:
                displayBalance(balance);
                loopControl = returnToMenuOrExit();
                break;
            case 2:
                deposit(accountNumber);
                loopControl = returnToMenuOrExit();
                break;
            case 3:
                withdraw(accountNumber);
                loopControl = returnToMenuOrExit();
                break;
            case 4:
                break;
        }
    }

    system("clear");
    cout << "Thank you for your business, goodbye." << endl;
    
    return 0;
}