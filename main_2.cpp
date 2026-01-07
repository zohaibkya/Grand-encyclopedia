#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include<iomanip>
#include <chrono>
#include <thread>
#include"Header.h"
using namespace std;

inline void delay(int sec) {
	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono; // nanoseconds, system_clock, seconds

	sleep_for(nanoseconds(10));
	sleep_until(system_clock::now() + seconds(sec));
}



inline void ClearScreen() {
	system("cls");
}



class LibraryDB {
protected:
	bool check_ID_exists(string param_ID) {
		ifstream file("Books/Lists.txt");
		if (!file)
			return false;

		string f_id, f_title, f_author, f_price, f_avail;

		while (getline(file, f_id, '|') && getline(file, f_title, '|') &&
			getline(file, f_author, '|') && getline(file, f_price, '|') &&
			getline(file, f_avail)) {

			if (param_ID == f_id) {
				file.close();
				return true;
			}
		}
		file.close();
		return false;
	}
};

class Library :public LibraryDB, public libart {
private:
	string loggedin_Username;
	string ID;
	string title, author;
	string price;
	string type;

public:
	Library(string t, string l = "") {
		type = t;
		loggedin_Username = l;
		MainMenu();
	}

	void MainMenu() {
		char option;
		do {
			welcome2();
			cout << " ---------------------------------\n";
			slowtext("|1 to Enter library              |\n", 2);
			slowtext("|2 to Add Books                  |\n", 2);
			slowtext("|3 to Purchase Books             |\n", 2);
			slowtext("|4 to Deposit amount             |\n", 2);
			slowtext("|5 to View your Balance          |\n", 2);
			slowtext("|6 to Remove Book                |\n", 2);
			slowtext("|0 to Logout                     |\n", 2);
			cout << " ---------------------------------\n";
			cout << "Enter your option: ";

			cin >> option;
			cin.ignore();
			try {

				switch (option) {
				case '1':
					enterlibrary();
					ClearScreen();
					break;

				case '2':
					add_Books();
					ClearScreen();
					break;
				case '3':
					purchase_book();
					ClearScreen();
					break;
				case '4':deposit_amount();
					ClearScreen();
					break;
				case '5': view_Balance();
					ClearScreen();
					break;
				case '6':removeBook();
					ClearScreen();
					break;
				case '0':
					return;
				default: break;
				}
			}
			catch (FileException e) {
				cout << e.getMessage();
				cout << "***********************************" << endl;

				Error1();
				cout << "Press any key to return to menu...";
				string t; cin >> t;
				ClearScreen();
			}
			catch (BookNotFoundException b) {
				cout << b.getMessage();
				cout << "************************************" << endl;
				delay(2);

				ClearScreen();
			}
		} while (option != '0');
	}

	void view_book_lists() {
		ClearScreen();
		ifstream file("Books/Lists.txt");
		string f_id, f_title, f_author, f_price, f_avail;

		if (!file) {
			throw FileException();
		}
		else {
			cout << "--------------------------------------------------------------------------------------" << endl;
			cout << left << setw(10) << "ID"
				<< setw(30) << "TITLE"
				<< setw(20) << "AUTHOR"
				<< setw(10) << "PRICE"
				<< setw(10) << "AVAIL" << endl;
			cout << "--------------------------------------------------------------------------------------" << endl;

			while (getline(file, f_id, '|') && getline(file, f_title, '|') &&
				getline(file, f_author, '|') && getline(file, f_price, '|') &&
				getline(file, f_avail)) {

				cout << left << setw(10) << f_id
					<< setw(30) << f_title.substr(0, 29)
					<< setw(20) << f_author.substr(0, 19)
					<< setw(10) << f_price
					<< setw(10) << f_avail << endl;
			}
			cout << "--------------------------------------------------------------------------------------" << endl;
			file.close();
		}
	}

	void add_Books() {
		ClearScreen();
		if (type == "Admin") {
			cout << "Enter Book Id: ";
			cin >> ID;

			if (check_ID_exists(ID)) {
				cout << "ID of this book already exists!" << endl;
			}
			else {
				cin.ignore();
				cout << "Enter Title ";
				getline(cin, title);
				cout << "Enter Author ";
				getline(cin, author);
				do {
					cout << "Enter Price ";
					cin >> price;
				} while (price[0] == '-' || price[0] == '0');

				ofstream writeFile("Books/Lists.txt", ios::app);
				if (writeFile) {
					writeFile << ID << "|" << title << "|" << author << "|" << price << "|" << "Yes" << endl;
					writeFile.close();
					cout << "Book added successfully!" << endl;
				}
			}
		}
		else {
			cout << "Only Admins can Add or Remove Books" << endl;
		}

		cout << "\nPress any key to continue: " << endl;
		string temp;

		cin >> temp;
	}

	void purchase_book() {
		ClearScreen();
		if (type == "User") {


			string temp_id;
			cout << "Enter Book ID to purchase: ";
			cin >> temp_id;

			ifstream readFile("Books/Lists.txt");
			ofstream writeFile("Books/temp.txt");

			if (!readFile || !writeFile) {
				if (readFile) readFile.close();
				if (writeFile) writeFile.close();
				throw FileException();
				string t; cin >> t;
				return;
			}

			string f_id, f_title, f_author, f_price, f_avail;
			double book_price;
			bool found = false;
			bool already_sold = false;
			string book_name = "";

			while (getline(readFile, f_id, '|') && getline(readFile, f_title, '|') &&
				getline(readFile, f_author, '|') && getline(readFile, f_price, '|') &&
				getline(readFile, f_avail)) {

				if (temp_id == f_id) {
					found = true;
					book_name = f_title;
					book_price = stod(f_price);

					if (f_avail == "No") {
						already_sold = true;
						writeFile << f_id << "|" << f_title << "|" << f_author << "|" << f_price << "|" << f_avail << endl;
					}
					else {
						writeFile << f_id << "|" << f_title << "|" << f_author << "|" << f_price << "|" << "No" << endl;
					}
				}
				else {
					writeFile << f_id << "|" << f_title << "|" << f_author << "|" << f_price << "|" << f_avail << endl;
				}
			}

			readFile.close();
			writeFile.close();



			if (found && !already_sold) {

				bool user_bought = deduct_user_money(book_price);
				if (user_bought) {
					cout << "Congrats! Book '" << book_name << "' Purchased Successfully." << endl;
					remove("Books/Lists.txt");
					rename("Books/temp.txt", "Books/Lists.txt");
				}
				else {
					cout << "Transaction failed. Insufficient funds!!" << endl;
					remove("Books/temp.txt");
				}

			}
			else if (already_sold) {
				cout << "Book '" << book_name << "' is ALREADY purchased." << endl;
				remove("Books/temp.txt");
			}
			else {
				remove("Books/temp.txt");
				throw BookNotFoundException();

			}
		}
		else {
			cout << "Admin or Guests cannot Purchase Books" << endl;
			ClearScreen();
		}
		cout << "\nPress Any Key to return to Menu";
		string t;
		cin >> t;
	}

	bool deduct_user_money(double price) {

		double book_price = price;
		double new_amount;
		ifstream readFile("regular_user_credentials.txt");
		ofstream writeFile("temp.txt");

		string tempUsername, tempPassword, tempBalance;
		bool action_completed = false;

		while (getline(readFile, tempUsername, '|') && getline(readFile, tempPassword, '|') && getline(readFile, tempBalance)) {

			if (tempUsername == loggedin_Username) {
				double user_Balance = stod(tempBalance);
				if (book_price > user_Balance) {
					writeFile << tempUsername << "|" << tempPassword << "|" << tempBalance << endl;
				}
				else {
					new_amount = user_Balance - book_price;
					writeFile << tempUsername << "|" << tempPassword << "|" << new_amount << endl;
					action_completed = true;
				}
			}
			else
				writeFile << tempUsername << "|" << tempPassword << "|" << tempBalance << endl;

		}

		readFile.close();
		writeFile.close();

		remove("regular_user_credentials.txt");
		rename("temp.txt", "regular_user_credentials.txt");

		return action_completed;

	}


















	void enterlibrary()
	{
		ClearScreen();
		libraryart2();
		char option;
		do {
			buybookart();
			slowtext("Enter your option: ", 30);
			cin >> option;
			string key;
			try {
				switch (option) {
				case '1':
					view_book_lists();
					cout << "Enter any key to exit";
					cin >> key;
					ClearScreen();
					break;
				case '2':
					ClearScreen();
					libraryart2();
					preview_book();
					ClearScreen();
					break;
				case '3':
					ClearScreen();
					libraryart2();
					view_book_by_page();
					ClearScreen();
					break;
				case 'x':
					ClearScreen();
					libraryart2();
					slowtext("\033[31mExiting ...\033[0m", 30);
					break;
				default:
					ClearScreen();
					cout << "invalid input";
					Error1();
					delay(2);
					ClearScreen();
				}
			}
			catch (BookNotFoundException b) {
				b.getMessage();
				delay(2);
				ClearScreen();
			}
		} while (option != 'x');

	}

	void bookbypage(const string& bookId)

	{
		string z = "Available Books\\" + bookId + ".txt";
		ifstream open(z);
		if (!open)
		{
			cout << "\n" << z << " File not opened!" << endl;

		}
		int line = 0;
		string x, y;
		int pagenum = 0;
		bool found = false;
		bool foundend = false;
		string defaultx = "Page ";
		cout << "Enter the page: ";
		cin >> pagenum;
		string start = defaultx + to_string(pagenum);
		string end1 = defaultx + to_string(pagenum + 1);
		while (getline(open, x))

		{
			line++;
			if (x.find(start) != string::npos)
			{
				found = true;
				break;
			}
		}
		int line2 = line;
		if (!found)

		{

			cout << "Page not found";

		}

		while (getline(open, y))

		{

			line2++;

			if (y.find(end1) != string::npos)

			{


				foundend = true;

				break;
			}

		}

		if (!foundend)

		{

			//cout << "not found";

		}
		else {
			int lineno = 0;

			open.clear();

			open.seekg(0);
			bool run = false;


			cout << " ===============================================================================================================" << endl;
			while (getline(open, y))

			{

				lineno++;

				if (lineno >= line && lineno <= line2 - 1)

				{

					cout << y << endl;

				}

			}

			cout << " ==============================================================================================================" << endl;
			open.close();
		}



	}
	void previewbook(const string& bookId)
	{
		string z = "Available Books\\" + bookId + ".txt";
		ifstream open(z);
		if (!open)
		{
			cout << "The txt file for this book doesnot exist." << endl;
		}
		int line = 0;
		string x, y;
		int pagenum = 0;
		bool found = false;
		bool foundend = false;
		string defaultx = "Contents";
		string end1 = "===== Page";
		while (getline(open, x))

		{
			line++;
			if (x.find(defaultx) != string::npos)
			{
				found = true;
				break;
			}
		}
		int line2 = line;
		if (!found)

		{

			cout << "Contents not found\n";

		}

		while (getline(open, y))

		{
			line2++;
			if (y.find(end1) != string::npos)
			{
				foundend = true;
				break;
			}

		}

		if (!foundend)

		{

			//cout << " Contents not found";

		}
		else {
			int lineno = 0;

			open.clear();

			open.seekg(0);

			cout << "\033[33m ===============================================================================================================" << endl;
			while (getline(open, y))

			{

				lineno++;

				if (lineno >= line && lineno <= line2 - 1)

				{

					cout << y << endl;

				}

			}

			cout << " ==============================================================================================================\033[0m" << endl;
			open.close();
		}
	}
	void preview_book()
	{
		ClearScreen();
		string bookId;
		libraryart2();
		slowtext("\033[32mEnter book ID: \033[0m", 30);
		cin >> bookId;


		ifstream readFile("Books/Lists.txt");


		string f_id, f_title, f_author, f_price, f_avail;
		bool found = false;

		while (getline(readFile, f_id, '|') && getline(readFile, f_title, '|') &&
			getline(readFile, f_author, '|') && getline(readFile, f_price, '|') &&
			getline(readFile, f_avail)) {

			if (f_id == bookId) {
				found = true;
			}

		}

		readFile.close();

		if (!found) {
			throw BookNotFoundException();
		}
		else {

			previewbook(bookId);

			slowtext("\033[32mEnter Any key to exit to menu: \033[0m");
			string t;
			cin >> t;
		}
	}
	void view_book_by_page()
	{
		if (type == "Guest")
		{

			cout << "Guests can only preview books.\n";
			delay(2);
			return;
		}

		string bookid;
		cout << "Enter book ID: ";
		cin >> bookid;

		ifstream readFile("Books/Lists.txt");


		string f_id, f_title, f_author, f_price, f_avail;
		bool found = false;

		while (getline(readFile, f_id, '|') && getline(readFile, f_title, '|') &&
			getline(readFile, f_author, '|') && getline(readFile, f_price, '|') &&
			getline(readFile, f_avail)) {

			if (f_id == bookid) {
				found = true;
			}

		}

		readFile.close();

		if (!found) {
			throw BookNotFoundException();
		}
		else {
			char choice;
			do
			{
				ClearScreen();
				bookbypage(bookid);

				cout << "\n----------------------------------\n";
				cout << "[N] View another page\n";
				cout << "[E] Exit to main menu\n";
				cout << "----------------------------------\n";
				cout << "Choice: ";
				cin >> choice;

				choice = tolower(choice);

			} while (choice == 'n');
		}

	}









	void removeBook() {
		if (type == "Admin") {
			string bookId;
			cout << "Enter Book Id" << endl;
			cin >> bookId;

			ifstream readFile("Books/Lists.txt");


			string f_id, f_title, f_author, f_price, f_avail;
			string book_name;
			bool found = false;

			while (getline(readFile, f_id, '|') && getline(readFile, f_title, '|') &&
				getline(readFile, f_author, '|') && getline(readFile, f_price, '|') &&
				getline(readFile, f_avail)) {

				if (f_id == bookId) {
					book_name = f_title;
					found = true;
				}

			}

			readFile.close();

			if (!found) {
				throw BookNotFoundException();
			}
			else {
				ifstream readFile("Books/Lists.txt");
				ofstream writeFile("Books/temp.txt");

				string f_id, f_title, f_author, f_price, f_avail;

				while (getline(readFile, f_id, '|') && getline(readFile, f_title, '|') &&
					getline(readFile, f_author, '|') && getline(readFile, f_price, '|') &&
					getline(readFile, f_avail)) {

					if (f_id == bookId) {
						continue;
					}
					else {
						writeFile << f_id << "|" << f_title << "|" << f_author << "|" << f_price << "|" << f_avail << endl;
					}

				}

				readFile.close();
				writeFile.close();

				cout << "Book: " << book_name << " Successfully Removed" << endl;
				delay(2);

				remove("Books/Lists.txt");
				rename("Books/temp.txt", "Books/Lists.txt");

			}



		}
		else {
			cout << "Only Admins can Remove Books" << endl;
			delay(2);
		}
	}

	void deposit_amount() {
		ClearScreen();
		if (type != "User") {
			cout << "Only Regular Users Deposit" << endl;
		}

		else {
			double temp_amount, new_amount = 0;
			cout << "Enter Amount you want to Deposit" << endl;
			cin >> temp_amount;

			if (temp_amount <= 0)
				cout << "Amount cannot be Negative nor Zero" << endl;
			else {


				ifstream readFile("regular_user_credentials.txt");
				ofstream writeFile("temp.txt");

				string tempUsername, tempPassword, tempBalance;
				bool found = false;

				while (getline(readFile, tempUsername, '|') && getline(readFile, tempPassword, '|') && getline(readFile, tempBalance)) {

					if (tempUsername == loggedin_Username) {
						double amount = stod(tempBalance) + temp_amount;
						writeFile << tempUsername << "|" << tempPassword << "|" << amount << endl;
						new_amount = amount;

					}
					else
						writeFile << tempUsername << "|" << tempPassword << "|" << tempBalance << endl;

				}

				readFile.close();
				writeFile.close();

				remove("regular_user_credentials.txt");
				rename("temp.txt", "regular_user_credentials.txt");

				cout << "Successfully Deposited. Your new Balance is " << new_amount << endl;

			}

		}
		cout << "\nPress Any Key to return to Menu...";
		string t;
		cin >> t;

	}

	void view_Balance() {
		ClearScreen();
		if (type != "User") {
			cout << "Only Regular Users have Balance" << endl;
		}
		else {

			ifstream readFile("regular_user_credentials.txt");

			string tempUsername, tempPassword, tempBalance;
			cout << loggedin_Username << "---------------" << endl;
			while (getline(readFile, tempUsername, '|') &&
				getline(readFile, tempPassword, '|') &&
				getline(readFile, tempBalance)) {

				if (tempUsername == loggedin_Username) {
					cout << "\nYour Balance is: " << tempBalance << endl;
					break;
				}
			}
			readFile.close();

		}
		cout << "\nPress Any Key to return to Menu...";
		string t;
		cin >> t;
	}
};
















class Login : public libart {
private:
	string username;
	string password;

public:
	string current_user_session;
	Login() {}

	string authenticate() {
		char option;
		welcome();
		slowtext("\033[31m[a] Regular user\033[0m", 5);
		slowtext("\t\t\033[32m[b] Admin user\033[0m", 5);
		slowtext("\t\t\t\033[33m[c] Guest\033[0m", 5);
		slowtext("\n\033[34m[d] Forgot user password\033[0m", 5);
		slowtext("\t\033[35m[e] Forgot admin password\033[0m", 5);
		slowtext("\t\033[40m[f] Register new user\033[0m\n", 5);
		slowtext("\n\t\t\t\t\033[41m[x] Leave program\033[0m\n", 5);

		cout << "\nEnter your option: ";
		cin >> option;
		cout << "\033\0m";
		cin.ignore();

		switch (option)
		{
		case 'a':
			if (RegularUser() == true)
				return "User";
			else
				return "";
			break;

		case 'b':
			if (Admin() == true)
				return "Admin";
			else
				return "";
			break;

		case 'c':Guest();
			return "Guest";
			break;
		case 'f':Register();
			return "User";
			break;
		case 'd':ForgotPasswordUser();
			return "";
			break;
		case 'e':
			ForgotPasswordAdmin();
			return "";
			break;
		case 'x':
			return "Exit";
			break;
		default:
			return "";
		}

	}

	bool RegularUser() {
		cout << "\033[31mRegular User" << endl;
		cout << "Enter username: " << endl;
		cin >> username;
		cout << "\nEnter Password: \033[0m" << endl;
		cin >> password;

		bool matched = CheckCredentailsUser(username, password);

		if (matched == true) {
			current_user_session = username;
			return true;

		}
		else {
			ClearScreen();
			Error1();
			slowtext(" Username or Password incorrect!!!!!\n\033[32mProgram will now restart!!!!\033[0m", 30);
			delay(2);
			ClearScreen();
			return false;

		}

	}
	bool Admin() {
		cout << "\n\033[32mAAdmin User" << endl;
		cout << "Enter username: " << endl;
		cin >> username;
		cout << "\nEnter Password: " << endl;
		cin >> password;
		cout << "\033[0m";
		bool matched = CheckCredentailsAdmin(username, password);

		if (matched == true) {
			return true;
		}
		else {
			ClearScreen();
			Error1();
			cout << "Username or Password incorrect" << endl;
			delay(2);
			ClearScreen();
			return false;
		}

	}
	void Guest() {}
	void Register() {
		bool checked;
		do {
			slowtext("\033[32mEEnter username: ", 30);
			cin >> username;
			cout << "\033[0m";
			checked = AlreadyExists(username);
			if (checked == true) {
				cout << "Username Already Exists" << endl;
			}
		} while (AlreadyExists(username) == true);

		cout << "\nEnter password" << endl;
		cin >> password;
		cout << endl;


		SavetoFile(username, password);
		cout << "Registered Successfully" << endl;
		delay(2);
		ClearScreen();

	}
	void ForgotPasswordUser() {
		cout << "Forgot Password" << endl;
		cout << "Enter username: " << endl;
		cin >> username;
		cout << endl;

		ifstream readFile("regular_user_credentials.txt");
		ofstream writeFile("temp.txt");

		string tempUsername, tempPassword, tempBalance;
		bool found = false;

		while (getline(readFile, tempUsername, '|') && getline(readFile, tempPassword, '|') && getline(readFile, tempBalance)) {

			if (tempUsername == username) {
				string newPass;
				cout << "User found! Enter your NEW password: ";
				cin >> newPass;


				writeFile << tempUsername << "|" << newPass << "|" << tempBalance << endl;
				found = true;
				cout << "Password updated successfully!" << endl;
			}
			else {

				writeFile << tempUsername << "|" << tempPassword << "|" << tempBalance << endl;
			}
		}

		readFile.close();
		writeFile.close();

		if (found) {
			remove("regular_user_credentials.txt");
			rename("temp.txt", "regular_user_credentials.txt");
		}
		else {
			remove("temp.txt");
			cout << "Error: Username not found." << endl;
		}
		delay(2);
		ClearScreen();

	}

	void ForgotPasswordAdmin() {

		cout << "Forgot Password" << endl;
		cout << "Enter username: " << endl;
		cin >> username;
		cout << endl;

		ifstream readFile("admin_credentials.txt");
		ofstream writeFile("temp.txt");

		string tempUsername, tempPassword;
		bool found = false;

		while (getline(readFile, tempUsername, '|') && getline(readFile, tempPassword)) {

			if (tempUsername == username) {

				string newPass;
				cout << "User found! Enter your NEW password: ";
				cin >> newPass;

				writeFile << tempUsername << "|" << newPass << endl;
				found = true;
				cout << "Password updated successfully!" << endl;
			}
			else {

				writeFile << tempUsername << "|" << tempPassword << endl;
			}
		}

		readFile.close();
		writeFile.close();


		if (found) {
			remove("admin_credentials.txt");
			rename("temp.txt", "admin_credentials.txt");
		}
		else {
			remove("temp.txt");
			cout << "Error: Username not found." << endl;
		}
		delay(2);
		ClearScreen();

	}

	void SavetoFile(string username, string password) {
		ofstream file("regular_user_credentials.txt", ios::app);

		file << username << "|" << password << "|" << "500" << endl;
		file.close();

	}

	bool AlreadyExists(string username) {
		ifstream readFile("regular_user_credentials.txt");

		string tempUsername, tempPassword, tempBalance;
		bool found = false;
		while ((getline(readFile, tempUsername, '|') && getline(readFile, tempPassword, '|')) && getline(readFile, tempBalance)) {
			if (username == tempUsername)
				found = true;
		}
		readFile.close();
		return found;
	}

	bool CheckCredentailsUser(string username, string password) {

		ifstream readFile("regular_user_credentials.txt");

		string tempUsername, tempPassword, tempBalance;
		bool found = false;
		while ((getline(readFile, tempUsername, '|') && getline(readFile, tempPassword, '|')) && getline(readFile, tempBalance)) {
			if ((username == tempUsername) && (password == tempPassword))
				found = true;
		}
		readFile.close();
		return found;
	}

	bool CheckCredentailsAdmin(string username, string password) {

		ifstream readFile("admin_credentials.txt");

		string tempUsername, tempPassword;
		bool found = false;
		while ((getline(readFile, tempUsername, '|') && getline(readFile, tempPassword))) {
			if ((username == tempUsername) && (password == tempPassword))
				found = true;
		}
		readFile.close();
		return found;
	}


};

int main()
{
	string role;
	Login l;

	while (true)
	{
		role = l.authenticate();

		if (role == "Exit")
		{
			cout << "EExiting Program\n";
			break;
		}

		if (role == "")
		{
			ClearScreen();
			continue;
		}

		ClearScreen();

		if (role == "Guest")
		{
			session();
			delay(2);
			ClearScreen();
		}
		else
		{
			loggedin();
			delay(3);
			ClearScreen();
			session();
			delay(2);
			ClearScreen();
		}
		Library lib(role, l.current_user_session);
		ClearScreen();
	}
}