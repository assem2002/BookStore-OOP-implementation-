#include<iostream>
#include<vector>
#include<deque>
#include<set>
#include<tuple>
#include<ctime>
class user;
class book;
class history;
class loginSignUPMenu;
class usingMode;
std::string invalidityFlag = "-1"; //made a mistake building function return by reference and there's a case where i send temporary variable so i had to make this variable to just fix it fast
// our dataBase
std::vector<book*> allBooks;
std::vector<user*> admins;
std::vector<user*> readers;
//Backend Side
#include "book_store.h"


// definition of book Member Functions
book::book(std::string ISBN,std::string title,std::string authorname,std::vector<std::string> pages):
    ISBN(ISBN),title(title),authorname(authorname),pages(pages){}
book::book(std::string ISBN,std::string title,std::string authorname,int numberOfPages):ISBN(ISBN),title(title),authorname(authorname){ //Constructor definition for book
    for(int pageNumber= 1 ; pageNumber<=numberOfPages ; pageNumber++){
        std::cout<<"Enter Page#"<<pageNumber<<": ";
        std::string temp;
        std::cin>>temp;
        pages.push_back(temp);
        std::cout<<std::endl;
    }
}
const std::string& book::Page(const int & currentPage) const{
    if ((currentPage> (int)pages.size()) || currentPage<=0)
        return invalidityFlag ;// if -1 ,then means the page you're asking for invalid page
    return pages[currentPage-1];
}
const std::string& book::nextPage(const int &currentPage) const{
    return Page(currentPage+1);
}
const std::string& book::previousPage(const int &currentPage) const{
    return Page(currentPage-1);
}
const std::string& book::gettitle()const {
    return title;
}

//Definition Of ->History<- Member Functions

const std::pair<book*,int> history::chooseSession(const int & choice) const{
    if (choice>(int)sessions.size() || choice<1)
        return std::make_pair(allBooks[0],-1); // if invalid choice, then return -1
    //just to index properly use Choice Variable decremented (0 index issue)
    return std::make_pair(std::get<0>(sessions[choice-1]),std::get<1>(sessions[choice-1]));
}


void history::viewHisotry() const{
    for(int session = 0 ; session<(int)sessions.size();session++){
        std::cout<<session+1<<") "<<(std::get<0>(sessions[session])->gettitle())<<" |page: "<<(std::get<1>(sessions[session]))<<" | "<<(std::get<2>(sessions[session]))<<std::endl;
    }
}


void history::addSession(book* currentbook, int currentpage, char* date){
    sessions.push_front(std::make_tuple(currentbook,currentpage,date));
}
bool history::isEmpty() const{
    return (sessions.size()==0);
}



//Definition of User  Member Functions
user::user(std::string username,std::string email,std::string name,std::string password):
    username(username),email(email),name(name),password(password){
}
std::vector<std::string> user::viewProfile() const{
    std::vector<std::string> temp {username,email,name};
    return temp;

}
history* user::getUserHistory() {
    return (&userHistory);
}


//FrontEnd side


//Login and Signup menu
int loginSignUPMenu::theMessage(){
    std::cout<<"MENU:"<<std::endl;
    std::cout<<"            1) Log In"<<std::endl;
    std::cout<<"            2) Sign Up"<<std::endl;
    std::cout<<"Enter Your Choice in Range 1 - 2: ";
    std::string userChoice;
    std::cin>>userChoice;
    if (userChoice == "1" )
        return 1;
    if(userChoice == "2")
        return 2;
    return -1;
}



std::pair<user*,int> loginSignUPMenu::loginMenu(){
    int adminpermission = 1;
    int readerpermission=  0;
    int invalid= -1;
    user* who{};
    std::cout<<"Enter Your UserName: ";
    std::string userNameEnrty;
    std::cin.ignore(3,'\n');
    std::getline(std::cin,userNameEnrty);
    std::cout<<"Enter Your Password: ";
    std::string passwordEnrty;
    std::getline(std::cin,passwordEnrty);
    std::cout<<std::endl;
    for(user* &admin :admins){
        if (admin->password == passwordEnrty && admin->username ==userNameEnrty){
            return std::make_pair(admin,adminpermission);
        }
    }
    for (user* &reader :readers){
        if(reader->password == passwordEnrty && reader->username ==userNameEnrty){
            return std::make_pair(reader,readerpermission);
        }
    }
    return std::make_pair(who,invalid);

}



std::pair<user*,int> loginSignUPMenu::signupMenu(){
    std::cout<<"Enter Your UserName (No Spaces): ";
    std::string userNameEnrty;
    std::cin.ignore(3,'\n');
    std::getline(std::cin,userNameEnrty);
    std::cout<<"Enter Your Password (No Spaces): ";
    std::string PasswordEnrty;
    std::getline(std::cin,PasswordEnrty);
    std::cout<<"Enter Your Name (No Spaces): ";
    std::string NameEnrty;
    std::getline(std::cin,NameEnrty);
    std::cout<<"Enter Your Email (No Spaces): ";
    std::string EmailEnrty;
    std::getline(std::cin,EmailEnrty);
    int valid = -1;
    user* createdUser;

    for(user* &admin :admins){
        if (admin->username ==userNameEnrty)
            return std::make_pair(createdUser,valid);
    }
    for (user* &reader :readers){
        if(reader->username ==userNameEnrty)
            return std::make_pair(createdUser,valid);
    }

    user* newUser = new user(userNameEnrty,EmailEnrty,NameEnrty,PasswordEnrty);
    valid= 1;
    readers.push_back(newUser);
    return std::make_pair(newUser,valid);
}

//Controlling the flow when a user is Entering the Program
//start Method
std::pair<user*,bool> loginSignUPMenu::start(){
    while(true){
        int dontStartAgain=1; // indicates if the user want to display the login in signup message again after invalid input;
        int choice;
        while(true){
            choice = theMessage();
            if (choice != -1)
                break;
            else
                std::cout<<"\nInvalid Input!\n";
        }
        user* validUser;
        int userPremissionAndValidty;
        if (choice == 1 ){ //login Mode
            while(dontStartAgain){

                std::tie(validUser,userPremissionAndValidty) = loginMenu(); //go for login method returning the user and the whether its valid or admin or reader.
                if (userPremissionAndValidty != -1) //admin or reader depends on the userPremissionAndValidty variable.
                    return std::make_pair(validUser,userPremissionAndValidty); //we're good to go to using mode
                else{ //not valid input
                    std::cout<<"Not Valid user !\n";
                    std::cout<<"\n1) Try Again\n"<<"2)Try To SignUp\n";
                    int choiceBackOrAgain;
                    std::cin>>choiceBackOrAgain;
                    if(choiceBackOrAgain == 2) dontStartAgain =0;
                }
            }



        }
        if (choice == 2){ //SignUp Mode
            while(dontStartAgain){
                user* validUser;
                int userPremissionAndValidty;
                std::tie(validUser,userPremissionAndValidty) = signupMenu();
                if(userPremissionAndValidty == -1){
                    std::cout<<"User Exist Before !\n";
                    std::cout<<"\n1) Try Again\n"<<"2)Try To Signin\nEnter Your Choice 1- 2: ";
                    int choiceBackOrAgain;
                    std::cin>>choiceBackOrAgain;
                    if(choiceBackOrAgain == 2) dontStartAgain =0;
                }
                else{
                    bool readerpremission = 0; // means the user passed will Enter to the using Mode as reader only
                    return std::make_pair(validUser,readerpremission); //good to go to using mode
                }
            }
        }

    }
}
//using Mode Constructor
usingMode::usingMode(user* currentuser, bool isadminMode):currentuser(currentuser) , isadminMode(isadminMode) {}

//using Mode -Reader Menu
int usingMode::readerMenu() const{
    std::cout<<"\n\n\n     1)View Profile\n     2)List & Select From My Histroy\n     3)List & Select From Available Books\n     4)Logout\n";
    while (true){
        std::cout<<"\nEnter Your Choice 1- 4:";
        int userChoice;
        std::cin>>userChoice;
        if (userChoice<1 || userChoice>4)
            std::cout<<"Invalid Input!\n\n";
        else
            return userChoice;
    }
}
//using mode admin menu
int usingMode::adminMenu()const{
    std::cout<<"1)View Profile\n2)AddBook\n3)Logout\n";
    while (true){
        std::cout<<"\nEnter Your Choice 1- 3: ";
        int userChoice;
        std::cin>>userChoice;
        if (userChoice<1 || userChoice>3)
            std::cout<<"Invalid Input!\n\n";
        else
            return userChoice;
    }

}
//using mode display user info
void usingMode::displayProfile()const{
    std::vector<std::string> allData =currentuser->viewProfile(); //vector of username & email &  name
    std::cout<<"\nName:       "<<allData[2]<<std::endl;
    std::cout<<"UserName:  "<<allData[0]<<std::endl;
    std::cout<<"Email:    "<<allData[1]<<"\n\n";
}
//view books in the all books vector
void usingMode::viewBooks()const {
    int num = 1;
    for (auto &book :allBooks)
        std::cout<<(num++)<<") "<<(book->gettitle())<<std::endl;

}
// add book to the all books vector
void usingMode::addBook(){
    std::string ISBN,title,authorName;
    int howManyPages;
    std::cout<<"\nEnter Book ISBN: ";
    std::cin>>ISBN;
    std::cin.ignore(256,'\n');
    std::cout<<"\nEnter Book Title: ";
    std::getline(std::cin,title);
    std::cout<<"\nEnter Book Author Name: ";
    std::getline(std::cin,authorName);
    std::cout<<"\nEnter Book How Many Pages: ";
    std::cin>>howManyPages;
    book* newBook = new book(ISBN,title,authorName,howManyPages);
    allBooks.push_back(newBook);
}
//READING mode
void usingMode::readingMode(book* chosenBook,int currentpage){
    std::cout<<"\n"<<(chosenBook->Page(currentpage));
    while(true){ //To stand still in the reading mode till you choose to stop
        int readingChoice;
        std::cout<<"\n 1) Next Page\n 2) Previous Page\n 3) Stop Reading\n Enter you Choice 1-3: ";
        std::cin>>readingChoice;
        if (readingChoice == 1){ //next page
            std::string res = chosenBook->nextPage(currentpage);
            if (res == "-1"){
                std::cout<<"\nEnd Of Book!\n";
                std::cout<<"\n\n"<<chosenBook->Page(currentpage)<<std::endl<<std::endl;
            }
            else{
                std::cout<<currentpage<<std::endl;
                std::cout<<"\n\n"<<res<<std::endl;
                currentpage++;
            }
        }
        else if (readingChoice == 2){ //Previous Page
            std::string res = chosenBook->previousPage(currentpage);
            if (res == "-1"){
                std::cout<<"\nStart Of Book!\n";
                std::cout<<"\n"<<chosenBook->Page(currentpage)<<std::endl<<std::endl;
            }
            else{
                std::cout<<"\n"<<res<<std::endl;
                currentpage--;
            }
        }
        else if (readingChoice == 3){ //Stop Reading in exist the loop
            time_t now = time(NULL);
            char* dateString = ctime(&now);
            currentuser->getUserHistory()->addSession(chosenBook,currentpage,dateString);
            break;
        }
        else
            std::cout<<"\nInvalid Enrty!\n";

    }
}


int main(){
    //
    std::vector<std::string> dummypages = {"Hello","to the book","we're happy to see you"};

    book* firstbook = new book{"112233","c++ How To Program","Assem",dummypages};
    allBooks.push_back(firstbook);

   book* secondbook = new book{"5566","The Grokking Algorithm","medhat",dummypages};
    allBooks.push_back(secondbook);

    user* admin1 = new user("assemmedhat","assem2002@gmail.com","Assem Bob","0000");

    user* admin2 = new user("anonymous","annno2002@gmail.com","The Ghost","8888");

    user* reader1 = new user("reader1","readerfirst@gmail.com","the reader1","9999");

    user* reader2 = new user("reader2","readersecond@gmail.com","the reader1","7777");

    admins.push_back(admin1);
    admins.push_back(admin2);
    readers.push_back(reader1);
    readers.push_back(reader2);
    program initiateIt;//calling the program function
    initiateIt.startprogram();
    return 0;


}
