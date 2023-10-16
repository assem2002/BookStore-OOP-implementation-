#ifndef BOOK_STORE_H_INCLUDED
#define BOOK_STORE_H_INCLUDED
#include<iostream>
//forward Declaration tho not necessary




class book{
private:
    std::string ISBN;
    std::string title;
    std::string authorname;
    std::vector<std::string> pages;
public:
    book(std::string ISBN,std::string title,std::string authorname,std::vector<std::string> pages);
    book(std::string ISBN,std::string title,std::string authorname,int numberOfPages); // Constuctor for adding a book
    const std::string& Page(const int & currentPage) const;
    const std::string& nextPage(const int &currentPage) const;
    const std::string& previousPage(const int &currentPage) const ;
    const std::string&gettitle()const ;


};

class history{
private:
    std::deque<std::tuple<book*, int, char*>> sessions;
public:
    const std::pair<book*,int> chooseSession(const int & choice) const;
    void viewHisotry()const;
    void addSession(book* currentbook, int currentpage, char* date);
    bool isEmpty()const;
};

class loginSignUPMenu{
private:

public:
    std::pair<user*,bool> start(); //the main function to fetch the user and pass it to the using mode class
    int theMessage(); //return Choice
    std::pair<user*,int> loginMenu(); // return (user*) , (integer : {(0)->reader | (1)->admin  |  (-1)->Invalid,Try Again.})
    std::pair<user*,int> signupMenu(); // return (user*) , (integer : {(1)-> created_Successfully |  (-1)->Wrong,exist before})

};

class user{
private:
    std::string username;
    std::string email;
    std::string name;
    std::string password;
    history userHistory{};
public:
    friend std::pair<user*,int> loginSignUPMenu::loginMenu();
    friend std::pair<user*,int> loginSignUPMenu::signupMenu();
    user() = default;
    user(std::string username,std::string email,std::string name,std::string password); //Constructor for sign up.
    std::vector<std::string> viewProfile() const ;
    history* getUserHistory() ;

};


// Interface class (just set of methods) that print data for me
class usingMode{
private:

public:
    user* currentuser;
    bool isadminMode;
    usingMode(user* currentuser, bool isadminMode);//initiate the using mode data members.
    int readerMenu() const ;
    int adminMenu() const;
    void displayProfile() const;
    void viewBooks() const;
    void addBook();
    void readingMode(book* chosenBook,int currentpage);

};
//flow controller class, i intended to write it here for no reason

class program{
public:
    void startprogram(){
        loginSignUPMenu instance;
        user* currentuser;
        bool isAdmin;
        while(true){
            bool signedIn = 1;
            std::tie(currentuser , isAdmin) = instance.start();
            usingMode modeInstance(currentuser,isAdmin);
            while(signedIn){
                int choice;
                if (isAdmin) choice = modeInstance.adminMenu();
                if (!isAdmin) choice = modeInstance.readerMenu();

                if (choice == 1)
                    modeInstance.displayProfile(); //view profile
                else if ((choice == 3 && isAdmin)||choice == 4) //logout
                    signedIn =0;
                else if (choice == 3 && !isAdmin){ //view books and choose
                    modeInstance.viewBooks();
                    int bookChoice;
                    while(true){
                        std::cout<<"Choose a book 1-"<<allBooks.size()<<":";
                        std::cin>>bookChoice;
                        if (bookChoice >=1 && bookChoice <= (int) allBooks.size())
                            break;
                        else
                            std::cout<<"\nNot Valid input!\n";
                    }
                    modeInstance.readingMode(allBooks[bookChoice-1],1); // Enter Reading mode, send the book pointer and page 1.
                    //Enter Reading mode
                }
                else if (choice == 2 && isAdmin)//adding book
                    modeInstance.addBook();

                //choose form Hisrtoy
                else if (choice == 2 && !isAdmin && (currentuser->getUserHistory()->isEmpty()))
                    std::cout<<"\nUser has no history\n";
                else if(choice == 2 && !isAdmin &&!(currentuser->getUserHistory()->isEmpty())){
                    int sessionChoice;
                    currentuser->getUserHistory()->viewHisotry();
                    book* toReadBook;
                    int pageToOpen;
                    while (true){
                        std::cout<<"/nEnter your Choice: ";
                        std::cin>>sessionChoice;
                        std::tie(toReadBook ,pageToOpen) = currentuser->getUserHistory()->chooseSession(sessionChoice);
                        if (pageToOpen !=-1)
                            break;
                        else
                            std::cout<<"\n Not Valid!";
                    }
                    modeInstance.readingMode(toReadBook,pageToOpen);

                }
                else
                    std::cout<<"\nInvalid Choice!\n";



            }

        }
    }
    ~program(){
        for (user* storedUser:readers){
        delete storedUser;
        }
        for (user* storedadmin:admins){
            delete storedadmin;
        }
        for (book* storedbook:allBooks){
            delete storedbook;
        }
    }
};







#endif // BOOK_STORE_H_INCLUDED
