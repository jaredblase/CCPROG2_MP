/***********************************************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned. I
have constructed the functions and their respective algorithms and corresponding code by myself. The program was run, tested,
and debugged by my own efforts. I further certify that I have not copied in part or whole or otherwise plagiarized the work of
other students and/or persons.
Jared Blase Del Castillo Sy, DLSU ID# 11929200
*************************************************************************************************************************************/

#include "shopping.c"

int main()
{
	int nUser;
	char cOpt, aAdPass[10];
	userType listUsers[M_USER];
	itemType listItems[M_USER][M_ITEM];
	
	// load users
	loadUsers(listUsers);
	// load items
	loadItems(listUsers, listItems);
	
	printf("Welcome to Easy Shopping!\n\n");
	
	do {
		cOpt = mainMenu();	
		
		switch(cOpt) {
			case '1':
				system("cls");
				if(userLen(listUsers) < 100) {
					registerUser(listUsers);
					system("cls");
					printf("Account registered!\n");
					printf("You can now log in.\n");
					loadItems(listUsers, listItems);
				} else
					printf("System reached maximum number of users!\n");
				break;
				
			case '2':
				system("cls");
				printf("-----------Login Page----------\n\n");
				nUser = logIn(listUsers);
				if (nUser >= 0) {
					printf("Log-in successful!\n");
					userMenu(listItems, listUsers, nUser);
				//	system("cls");
				} else {
					printf("Invalid user ID and/or password!\n");
				}
				if(!userLen(listUsers))
					printf("What the fuck?\n");
				break;
				
			case '3':
				inputString("Admin Password", aAdPass, 10);
				if (!strcmp(ADPASS, aAdPass))
					adminMenu(listUsers, listItems);
				else
					printf ("Unauthorized access not allowed!\n");
				break;

			case '4':
				printf("Thank you!\n");
				saveUsers(listUsers);
				saveItems(listUsers, listItems);
				break;
			
			default:
				printf(ERROR);
		}
	} while (cOpt != '4');
	
	return 0;
}
