#include "shopping.h"

/* Loads the users into the program from Users.txt.
   @param list is the pointer to the list of users.
*/
void
loadUsers(userType list[])
{
	int nPop = 0;
	FILE *fp = fopen("Users.txt", "r");
	
	if (fp) {
		while(!feof(fp)) {
			fscanf (fp, "%ld ", &list[nPop].nID);
		
			fgets(list[nPop].aPass, 12, fp);
			fgets(list[nPop].aName, 22, fp);
			fgets(list[nPop].aAddress, 32, fp);
			fgets(list[nPop].aContact, 17, fp);
			
			remEnter(list[nPop].aPass);
			remEnter(list[nPop].aName);
			remEnter(list[nPop].aAddress);
			remEnter(list[nPop].aContact);
			fscanf(fp, "\n");
			nPop++;
		}
		fclose(fp);
	}	
	//initialize the rest
	while (nPop < 100)
		list[nPop++].nID = 0;
	
	qsort((void*)list, userLen(list), sizeof(list[0]), comparator);
}

/* Loads the items into the program from Items.txt.
   @param list is the pointer to the list of items.
   @param listUsers is the pointer to the list of users.
*/
void
loadItems(userType listUsers[], itemType list[][M_ITEM])
{
	int i, j, nID, nProID;
	FILE *fp = fopen("Items.txt", "r");
	
	for (i = 0; i < M_USER * M_ITEM; i++) // initialize
		list[i / M_ITEM][i % M_ITEM].nProID = list[i / M_ITEM][i % M_ITEM].nID = 0;
	
	if (fp) {
		while (!feof(fp)) {
			fscanf (fp, "%d %d\n", &nProID, &nID);
			i = uniqueID(listUsers, nID);
			j = itemLen(list[i]);
			
			list[i][j].nID = nID;
			list[i][j].nProID = nProID;
			fgets(list[i][j].aName, 22, fp);
			fgets(list[i][j].aCategory, 17, fp);
			fgets(list[i][j].aDescription, 32, fp);
	
			remEnter(list[i][j].aName);
			remEnter(list[i][j].aCategory);
			remEnter(list[i][j].aDescription);
			
			fscanf (fp, "%d %f\n\n", &list[i][j].nQuan, &list[i][j].fPrice);
		}
		fclose(fp);
	}
	
	for (i = 0; i < userLen(listUsers); i++)
		sortItems(list[i]);
}

/* Displays the main menu and asks the user for input.
   Precondition: The user inputs a single character.
   @return a character based on the selected option of the user.
*/
char
mainMenu()
{
	char cOpt;
	
	printf ("\n|------------------------------|\n");
	printf ("|           Main Menu          |\n");
	printf ("|1 - Register                  |\n");
	printf ("|2 - Enter User Menu           |\n");
	printf ("|3 - Enter Admin Menu          |\n");
	printf ("|4 - Exit Program              |\n");
	printf ("|------------------------------|\n");
	printf ("Enter Option: ");
	scanf(" %c", &cOpt);
	if(getchar() != '\n') {
		cOpt = -1;
		while(getchar() != '\n');
	}
	
	return cOpt;
}

/* Handles the user registration.
   @param list is the pointer to the list of users.
*/
void
registerUser(userType list[])
{
	int i = userLen(list), temp = 0;
	
	printf ("==================Registration Page==================\n\n");
	printf ("Welcome to Easy Shopping, new user!\n");
	printf ("Please fill the following information to register!\n\n");
	
	do {
		printf ("Unique Numeric User ID: ");
		scanf ("%d", &temp);
		if (uniqueID(list, temp) >= 0) {
			printf ("ID taken!\n");
			temp = 0;
		} else if (temp <= 0)
			printf (ERROR);
		while(getchar() != '\n'); //clear buffer to prevent infinite loop
	} while (temp <= 0);
	
	list[i].nID = temp;
	inputString("Name", list[i].aName, 20);
	inputString("Address", list[i].aAddress, 30);
	inputString("Contact Number", list[i].aContact, 15);
	inputString("Password", list[i].aPass, 10);
	
	qsort((void*)list, i + 1, sizeof(list[0]), comparator); //sort the list of Users using qsort()
}

/* Passed as a parameter to the qsort() function
   @param p is the pointer to the lefthand element
   @param q is the pointer to the righthand element
*/
int
comparator(const void *p, const void *q)
{
	int l = ((userType*)p)->nID;
	int r = ((userType*)q)->nID;
	return (l - r);
}

/* Handles string inputs, and checks if the input exceeds maximum characters.
   @param info is the question to be displayed for the user to answer.
   @param str is a pointer to where the string will be stored.
   @param nLimit is the maximum number of characters for the input.
*/
void
inputString(char info[], char str[], int nLimit)
{
	char temp[nLimit + 2];
	
	do {
		printf ("%s: ", info);
		fflush(stdin); //clear the buffer
		fgets(temp, nLimit + 2, stdin);
		remEnter(temp);
		
		if (strlen(temp) > nLimit) 
			printf ("Exceeded character limit of %d!\n\n", nLimit);
		
	} while (strlen(temp) > nLimit);
	
	strcpy(str, temp);
}

/* Removes the newline character from the received string.
   @param s is the pointer to the string.
*/
void
remEnter(char s[])
{
	if (s[strlen(s) - 1] == '\n')
		s[strlen(s) - 1] = '\0';
}

/* This compares the chosen ID number to the array using the binary search algorithm.
   @param list is the pointer to the list of users.
   @param nID is the ID number to be checked.
   @return -1 if the ID is not found, the index of the ID in the array otherwise.
*/
int
uniqueID(userType list[], int nKey)
{
	int nEnd, nStart = 0, nMid;
	
	/* Count the number of items in the array */
	nEnd = userLen(list);
	nMid = nEnd / 2;
	
	do {
    	if (nKey > list[nMid].nID)
    		nStart = nMid + 1;

      	else if (nKey < list[nMid].nID)
       		nEnd = nMid - 1;

      	nMid = (nStart + nEnd) / 2;
    } while (nStart < nEnd && nKey != list[nMid].nID);
    
    if (nKey != list[nMid].nID)
        nMid = -1;
	
	return nMid;
}

/* Asks the user for ID and password. It first compares the input ID from the
   		array, then the password if there was an ID match.
   @param list is a pointer to the list of users.
   @return -1 if mismatch, and the index otherwise.
*/
int
logIn(userType list[])
{
	int nID, i;
	char aPass[10];
	
	printf ("User ID: ");
	scanf ("%d", &nID);
	inputString("Password", aPass, 10);
	
	i = uniqueID(list, nID); // retrieves the index of the input ID
	if (i != -1)
		if (strcmp(list[i].aPass, aPass))
			i = -1;
	
	return i;
}

/* Displays the user menu and calls other functions based on the user's input.
   @param listItems is the pointer to the list of items.
   @param listUsers is the pointer to the list of users.
   @param nUserIndex is the index of the user in listUsers.
*/
void
userMenu(itemType listItems[][M_ITEM], userType listUsers[], int nUserIndex)
{
	char cOpt;
	int nUID = listUsers[nUserIndex].nID;
	cartType cart[10];
	
	loadCart(cart, nUID);
		
	do {
		printf ("\n\nWelcome back, %d!\n", nUID);
		printf ("|------------------------------|\n");
		printf ("|           User Menu          |\n");
		printf ("|1 - Sell Menu                 |\n");
		printf ("|2 - Buy Menu                  |\n");
		printf ("|3 - Back to Main Menu         |\n");
		printf ("|------------------------------|\n");
		printf ("Enter Option: ");
		scanf (" %c", &cOpt);
		if(getchar() != '\n') {
			while(getchar() != '\n'); //flush buffer
			cOpt = -1;
		}
		
		switch (cOpt) {
			case '1':
				sellMenu(listItems, nUserIndex, nUID);
				break;
				
			case '2':
				buyMenu(listItems, nUID, cart, listUsers);
				break;
						
			case '3':
				saveCart(cart, nUID);
				break;

			default:
				printf (ERROR);
		}
	} while (cOpt != '3');
	
	if(!userLen(listUsers))
		printf("What the fuck?\n");
}

/* Counts the number of users in the list.
   @param list is the pointer to the list of users.
   @return the number of sellers.
*/
int
userLen(userType list[])
{
	int i;
	
	for (i = 0; i < M_USER && list[i].nID > 0; i++);
	
	return i;
}

/* Counts the number of items in a user's list.
   @param list is the pointer to the list of items of a user.
   @return the number of items.
*/
int
itemLen(itemType list[])
{
	int i;
	
	for (i = 0; i < M_ITEM && list[i].nProID > 0; i++);

	return i;
}

/* Counts the number of items a cart.
   @param list is the pointer to the user's cart.
   @return the number of items in the cart.
*/
int
cartLen(cartType list[])
{
	int i;
	
	for (i = 0; i < M_CART && list[i].nBuyQuan > 0; i++);
	
	return i;
}

/* Continues to display the sell menu until the user exits.
   @param list is the pointer to the list of items.
   @param nUser is the index of the logged in user.
   @param nUID is the ID of the logged in user.
*/
void
sellMenu(itemType list[][M_ITEM], int nUser, int nUID)
{
	char cOpt;
	
	do {
		printf ("\n|---------------------------------|\n");
		printf ("|             Sell Menu           |\n");
		printf ("|1 - Add New Item                 |\n");
		printf ("|2 - Edit Stock                   |\n");
		printf ("|3 - Show My Products             |\n");
		printf ("|4 - Show My Low-Stock Products   |\n");
		printf ("|5 - Back to User Menu            |\n");
		printf ("|---------------------------------|\n");
		printf ("Enter Option: ");
		scanf (" %c", &cOpt);
		if (getchar() != '\n') {
			cOpt = -1;
			while(getchar() != '\n');
		}
		
		switch (cOpt) {
			case '1':
				if (itemLen(list[nUser]) < 20)
					addNewItem(list, nUser, nUID);
				else
					printf ("You have reached the maximum number of items to sell!\n");
				break;
				
			case '2':
			case '3':	
			case '4':
				sortItems(list[nUser]);
				if (!(itemLen(list[nUser])))
					printf ("You currently have no items in your shop!\n");
				else if (cOpt == '2')
					editStocks(list[nUser]);
				else if (cOpt == '3')
					showProducts(list[nUser]);	
				else
					lowStock(list[nUser]);

			case '5':
				break;
			default:
				printf (ERROR);
		}
	} while (cOpt != '5');
	
}

/* Handles the addition items to the user's shop and sorts them by product ID afterwards.
   @param list is the pointer to the list of items.
   @param nUser is the index of the logged in user.
   @param nUID is the ID of the logged in user.
*/
void
addNewItem(itemType list[][M_ITEM], int nUser, int nUID)
{
	int nProID, i = itemLen(list[nUser]);
	
	printf ("Unique Product ID: ");
	scanf ("%d", &nProID);
	while (uniqueProID(list, nProID) >= 0 || !nProID) {
		printf ("Product ID taken!\n");
		printf ("Unique Product ID: ");
		scanf ("%d", &nProID);
	}
	
	list[nUser][i].nProID = nProID;
	list[nUser][i].nID = nUID;
	inputString("Product Name", list[nUser][i].aName, 20);
	inputString("Category", list[nUser][i].aCategory, 15);
	inputString("Description", list[nUser][i].aDescription, 30);
	
	printf ("Quantity: ");
	scanf ("%d", &list[nUser][i].nQuan);
	while (list[nUser][i].nQuan <= 0) {
		printf ("Invalid Input!\n"
		        "Quantity: ");
		while(getchar() != '\n');
		scanf ("%d", &list[nUser][i].nQuan);
	}
	while(getchar() != '\n');
	
	printf ("Price: ");
	scanf ("%f", &list[nUser][i].fPrice);
	while (list[nUser][i].fPrice <= 0) {
		printf ("Invalid Input!\n"
		        "Price: ");
		while(getchar() != '\n');
		scanf ("%f", &list[nUser][i].fPrice);
	}
	while(getchar() != '\n');
	sortItems(list[nUser]);
}

/* Sorts the list of items of a user by their product IDs.
   @param list is the pointer to the list of items of a user.
*/
void
sortItems(itemType list[])
{
	int i, j, index;
	itemType sLeast;
	
	/* Selection Sort */
	for (i = 0; i < itemLen(list); i++) {
		sLeast = list[i];
		index = i;
		
		for (j = i; j < itemLen(list); j++)
			if (list[j].nProID < sLeast.nProID) {
				sLeast = list[j];
				index = j;
			}

		list[index] = list[i];
		list[i] = sLeast;
	}
}

/* Compares the received product ID number to the array.
   @param list is the pointer to the list of items.
   @param nProID is the product ID to be checked.
   @return -1 if the product ID is not found, the index of the ID in the array otherwise.
*/
int
uniqueProID(itemType list[][M_ITEM], int nProID)
{
	int i = 0, check = -1, j;
	
	while(check == -1 && i < M_USER) {
		j = searchByProID(list[i], nProID);
		if (j != -1)
			check = i * M_ITEM + j;
		i++;
	}
	
	return check;
}

/* Asks the user for the product ID of the item to be edited, then calls
   editStocksMenu() when the ID is found.
   @param list is the pointer to the list of items.
*/
void
editStocks(itemType list[])
{
	int nProID = -1, nProIndex;
	char cOpt;
	
	showProducts(list);
	printf ("\nEnter product ID of desired item to edit: ");
	scanf ("%d", &nProID);
	
	if(nProID == -1)
		while(getchar() != '\n');
	
	nProIndex = searchByProID(list, nProID);
	if (nProIndex >= 0) {
		printf ("Product ID found!");
		while (editStocksMenu(&list[nProIndex], &cOpt) != '6');
	} else {
		printf ("Product ID not found.");
	}
	
	printf ("\n\n");
}

/* Performs a search using the binary search algorithm.
   @param list is the pointer to the user's list of items.
   @param nKey is the product ID to be searched for.
   @return the index if the key was found, returns -1 otherwise.
*/
int
searchByProID(itemType list[], int nKey)
{
	int nStart = 0, nEnd, nMid;
	
	/* Count the number of items in the array */
	nEnd = itemLen(list);
	nMid = nEnd / 2;
	
	while (nStart < nEnd && nKey != list[nMid].nProID) {
    	if (nKey > list[nMid].nProID)
    		nStart = nMid + 1;

      	else if (nKey < list[nMid].nProID)
       		nEnd = nMid - 1;

      	nMid = (nStart + nEnd) / 2;
    } 
    
    if (nKey != list[nMid].nProID)
        return -1;
	
	return nMid;
}

/* Main menu for editing stocks information.
   @param sItem is the pointer to the structure of a specific item.
   @return the character of the chosen menu option of the user.
*/
char
editStocksMenu(itemType* sItem, char* cOpt)
{
	int nAdd;
	float fPrice;
	
	showFullDetails(*sItem);
	
	if (*cOpt == -1)
		printf("\nInvalid Input!\n\n");
	printf ("\n|---------------------------------|\n");
	printf ("|            Stocks Menu          |\n");
	printf ("|1 - Replenish Stocks             |\n");
	printf ("|2 - Change Price                 |\n");
	printf ("|3 - Change Item Name             |\n");
	printf ("|4 - Change Category              |\n");
	printf ("|5 - Change Description           |\n");
	printf ("|6 - Finish Editing               |\n");
	printf ("|---------------------------------|\n");
	printf ("Enter Option: ");
	scanf (" %c", cOpt);
	if(getchar() != '\n') {
		*cOpt = -1;
		while(getchar() != '\n'); //clear buffer
	}
	
	switch (*cOpt) {
		case '1':
			do {
				printf ("Quantity to add: ");
				scanf ("%d", &nAdd);
				if (nAdd < 0)
					printf (ERROR);
			} while (nAdd < 0);
			sItem->nQuan += nAdd;
			break;
			
		case '2':
			do {
				printf ("New price: ");
				scanf ("%f", &fPrice);
				if (fPrice <= 0) {
					printf (ERROR);
					while(getchar() != '\n');
				}
			} while (fPrice <= 0);
			sItem->fPrice = fPrice;
			break;
			
		case '3':
			inputString("New item name", sItem->aName, 20);
			break;
			
		case '4':
			inputString("New category", sItem->aCategory, 15);
			break;
			
		case '5':
			inputString("New description", sItem->aDescription, 30);
	
		case '6':
			break;
		
		default:
			*cOpt = -1;
	}
	
	return *cOpt;
}

/* Sorts and diplays all the products being sold by the user.
   @param list is the pointer to the list of the user's products.
*/
void
showProducts(itemType list[])
{
	int i;
		
	printf ("%-10s | %-20s | %-15s | %-10s | %-10s |\n", "Product ID", 
	        "Item Name", "Category", "Unit Price", "Quantity"); //header
	        
	for (i = 0; i < itemLen(list); i++) {
		printf ("%10d | %-20s | %-15s | %10.2f | %10d |\n", list[i].nProID, 
		list[i].aName, list[i].aCategory, list[i].fPrice, list[i].nQuan);
	}
}

/* Displays items with less than 5 stocks left.
   @param list is the pointer to the list of the user's products.
*/
void
lowStock (itemType list[])
{
	int i; 
    char cOpt = '\0';
	
	for (i = 0; i < itemLen(list); i++)
		if (list[i].nQuan < 5) {
			showFullDetails(list[i]);
			cOpt = viewerMode(&i, M_ITEM);
		}
	
	if (cOpt == '\0')
		printf ("You have no low-stock products! :D\n");
	else if (cOpt == 'N')
		printf ("You have no more low-stock products! :D\n");
}

/* Displays every detail of an item.
   @param item is a structure containing the details of an item.
*/
void
showFullDetails (itemType item)
{
	system("cls");
	printf ("Product ID: %d\n", item.nProID);
	printf ("Name: %s\n", item.aName);
	printf ("Category: %s\n", item.aCategory);
	printf ("Unit Price: %.2f\n", item.fPrice);
	printf ("Stocks Left: %d\n", item.nQuan);
	printf ("Description: %s", item.aDescription);
}

/* Navigation for displays needing 'X' or 'N' before displaying the next.
   Assigns the sentinel value to the counter if the user chooses to exit.
   @param i is the pointer to the incrementing value from a loop
   @param nLimit is the sentinel value for the loop that called this function
   @return the character based on the option of the user
*/
char
viewerMode(int* i, int nLimit)
{
	char cOpt;
	
	do {
		printf ("\n\nPress 'N' to view next item or 'X' to exit viewer... ");
		
		switch (cOpt = toupper(getch())) {
            case 'X':
                *i = nLimit; //assign the sentinel value to stop the loop that called this function
            case 'N':
                system("cls");
                break;
            default:
                printf(ERROR);
        }
				
	} while (cOpt != 'N' && cOpt != 'X');
	
	return cOpt;
}

/* Displays the buy menu and calls functions based on the user's input.
   @param listItems is the pointer to the list of items.
   @param nUID is the numeric ID of the user.
   @param cart is the pointer to the cart of the user.
   @param listUsers it the pointer to the list of users.
*/
void
buyMenu(itemType listItems[][M_ITEM], int nUID, cartType cart[], userType listUsers[])
{
	char cOpt;
	int nUsers = userLen(listUsers);
		
	do {
		printf ("\n|---------------------------------------------|\n");
		printf ("|                 Buyer's Menu                |\n");
		printf ("|1 - View All Products                        |\n");
		printf ("|2 - Show All Products of a Specific Seller   |\n");
		printf ("|3 - Search Products by Category              |\n");
		printf ("|4 - Search Products by Name                  |\n");
		printf ("|5 - Add to Cart                              |\n");
		printf ("|6 - Edit Cart                                |\n");
		printf ("|7 - Check Out                                |\n");
		printf ("|8 - Exit Buyer's Menu                        |\n");
		printf ("|---------------------------------------------|\n");
		printf ("Enter Option: ");
		scanf (" %c", &cOpt);
		if(getchar() != '\n') {
			cOpt = -1;
			while(getchar() != '\n');
		}
		
		switch (cOpt) {
			case '1':
				viewAllProducts(listItems, nUID, nUsers);
				break;
				
			case '2':
				viewSpecificSeller(listItems, listUsers);
				break;
				
			case '3':
				viewByCategory(listItems, nUsers);
				break;
				
			case '4':
				viewByName(listItems, nUsers);
				break;
				
			case '5':
				if (cartLen(cart) != M_CART)
					addToCart(listItems, cart, nUID);
				else
					printf ("Cart full! Please edit your cart or check out first before adding more items.\n");
				break;
				
			case '6':
				sortCart(cart);
				checkUpdates(listItems, listUsers, cart);
				while(cartLen(cart) && editCart(listItems, cart) != '4');
				if(!cartLen(cart))
					printf ("No items in cart!\n");
				break;

			case '7':
				if (cartLen(cart))
					checkOut(listItems, cart, listUsers, nUID);
				else
					printf ("No items in cart!\n");
				
			case '8':
				break;
				
			default:
				printf (ERROR);
		}
	} while (cOpt != '8');
}

/* Sorts the items in the user's cart by sellerID first before ProductID
   @param the pointer to the user's cart
*/
void sortCart(cartType cart[])
{
	int i, j, index;
	cartType sLeast;
	
	/* Selection Sort */
	for (i = 0; i < cartLen(cart); i++) {
		sLeast = cart[i];
		index = i;
		
		for (j = i; j < cartLen(cart); j++)
			if ((cart[j].item.nID < sLeast.item.nID) || (cart[j].item.nID == sLeast.item.nID 
			     && cart[j].item.nProID < cart[j].item.nProID)) {
				sLeast = cart[j];
				index = j;
			}

		cart[index] = cart[i];
		cart[i] = sLeast;
	}
}

/* Display all of the products in the store except the user's.
   @param list is the pointer to the list of items.
   @param nUID is the numeric ID of the user.
   @param nUsers is the number of registered users.
*/
void
viewAllProducts(itemType list[][M_ITEM], int nUID, int nUsers)
{
	int i;
    char cOpt = '\0';

    /* Display Sellers*/
    for (i = 0; i < nUsers; i++) {
    	system("cls");
    						/* Skips the user logged in */
    	if (list[i][0].nProID && !(list[i][0].nID == nUID)) {
	        printf ("Seller ID: %d\n\n", list[i][0].nID);
	        showProducts(list[i]);
	    	cOpt = viewerMode(&i, M_USER);
	    }
    }
    
    if (cOpt == '\0')
		printf ("Store is empty :(\n");
	else if (cOpt == 'N')
		printf ("No more sellers!\n");
}

/* Looks for a specific seller based on ID and displays all of the seller's products.
   @param list is the pointer to the list of items.
   @param nUID is the numeric ID of the user.
   @param nUsers is the number registered users.
*/
void
viewSpecificSeller(itemType list[][M_ITEM], userType listUsers[])
{
	int nID, i;
	
	printf ("Input Seller ID: ");
	scanf ("%d", &nID);
	
	i = uniqueID(listUsers, nID);
	if (i != -1) {
		printf ("Seller ID: %d\n\n", nID);
		if(itemLen(list[i]))
			showProducts(list[i]);
		else
			printf("User is not a seller!\n");
	} else {
		printf ("No Seller ID Match!\n");
		while(getchar() != '\n'); //clear buffer
	}
}

/* Looks for items with the same category as searched by the user
   @param list is the pointer to the list of items
   @param nUsers is the number of registered users
*/
void
viewByCategory(itemType list[][M_ITEM], int nUsers)
{
	int i, j;
	char aCategory[15], aTemp[15], cOpt = '\0';
	
	inputString("Input item category", aCategory, 15);
	
	for (i = 0; i < nUsers; i++)
		for (j = 0; j < itemLen(list[i]); j++) {
			strcpy(aTemp, list[i][j].aCategory); //Store item category in a temp
	
			if (!strcmp(aTemp, aCategory)) {
				showFullDetails(list[i][j]);
				printf ("\n\nSeller ID: %d\n", list[i][j].nID);
				cOpt = viewerMode(&i, nUsers);
				if (cOpt == 'X')
					j = M_CART;
			}
		}
	
	if (cOpt == '\0')
		printf ("No results found.\n");
	else if (cOpt == 'N')
		printf ("No more items found with the same category!\n");
}

/* Looks for items based on the name search of the user, if the string search key
   of the user is a substring of the item name, it will still display the item
   @param list is the pointer to the list of items
   @param nUsers is the number of registered users
*/
void
viewByName (itemType list[][M_ITEM], int nUsers)
{
	char aName[20], aTemp[20], cOpt = '\0';
	int i, j;
	
	inputString("Search Key", aName, 20);
	strlwr(aName);
	
	for (i = 0; i < nUsers; i++)
		for (j = 0; j < itemLen(list[i]); j++) {
			strcpy(aTemp, list[i][j].aName);
			strlwr(aTemp);
	
			if (strstr(aTemp, aName) != NULL) {
				showFullDetails(list[i][j]);
				printf ("\n\nSeller ID: %d\n", list[i][j].nID);
				cOpt = viewerMode(&i, nUsers);
				if (cOpt == 'X')
					j = M_CART;
			}
		}
	
	if (cOpt == '\0')
		printf ("No results found.\n");
	else if (cOpt == 'N')
		printf ("No more items found with the same name!\n");
}

/* Handles the addition of an item to cart
   @param list is the pointer to the list of items
   @param cart is the pointer to the user's cart
   @param nUID is the numberic ID of the user
*/
int
addToCart (itemType list[][M_ITEM], cartType cart[], int nUID)
{
	int nProID = -1, i, nQuan, j;
	
	printf ("Product ID: ");
	scanf ("%d", &nProID);
	while(getchar() != '\n'); //clear buffer
	
	if ((i = uniqueProID(list, nProID)) < 0)
		printf ("No Product ID Match!\n\n");
			
	else if (list[i / M_ITEM][i % M_ITEM].nID == nUID)
			printf ("You cannot buy your own stocks!\n");

	else if (!list[i / M_ITEM][i % M_ITEM].nQuan)
			printf ("No More Stocks Left of this Product!\n");
	else {
		// Checks to see if the item was already added to cart before
		for (j = 0; j < cartLen(cart) && nProID != cart[j].item.nProID; j++);
		
		do {
			printf ("Quantity to buy: ");
			scanf ("%d", &nQuan);
			if (nQuan <= 0) {
				printf ("Invalid amount!\n");
				nQuan = -1;
			} else if (nQuan > list[i / M_ITEM][i % M_ITEM].nQuan) {
				printf ("Quantity entered is more than the current stocks!\n");
				nQuan = -1;
			} else if (j != cartLen(cart) && cart[j].nBuyQuan + nQuan > list[i / M_ITEM][i % M_ITEM].nQuan) {
				printf ("Quantity entered, when added to the previous quantity in the cart, is more than the current stocks!\n");
				nQuan = 0;
			}
		} while (nQuan < 0);
			
		printf ("%d %s(s) added to cart!\n", nQuan, list[i / M_ITEM][i % M_ITEM].aName);
			
		cart[j].nBuyQuan += nQuan;
		cart[j].item = list[i / M_ITEM][i % M_ITEM];
		return 1;
	}
	
	return 0;
}

/* Allows the user to edit the items in his cart
   @param list is the pointer to the list of items
   @param cart is the pointer to the user's cart
   @returns the character of the chosen option of the user
*/
char
editCart(itemType list[][M_ITEM], cartType cart[])
{
	int i, nCurID = 0;	
	char cOpt, cOpt2;
	
	for (i = 0; i < cartLen(cart); i++) {
		if (cart[i].item.nID != nCurID) {
			printf ("\nSeller ID: %d\n", cart[i].item.nID);
			printf ("%-10s | %-20s | %-15s | %-10s | %-20s |\n", "Product ID", 
	        "Item Name", "Category", "Unit Price", "Quantity to Buy"); //header
			nCurID = cart[i].item.nID;
		}
		printf ("%10d | %-20s | %-15s | %10.2f | %20d |\n", cart[i].item.nProID, 
		cart[i].item.aName, cart[i].item.aCategory, cart[i].item.fPrice, cart[i].nBuyQuan);
	}
	
	printf ("\n|-----------------------------------------|\n");
	printf ("|              Edit Cart Menu             |\n");
	printf ("|1 - Remove All Items from Seller         |\n");
	printf ("|2 - Remove Specific Item                 |\n");
	printf ("|3 - Edit Quantity                        |\n");
	printf ("|4 - Finish Editing Cart                  |\n");
	printf ("|-----------------------------------------|\n");
	printf ("Enter Option: ");
	scanf (" %c", &cOpt);
	if (getchar() != '\n') {
		cOpt = -1;
		while(getchar() != '\n'); //clear buffer
	}
	
	switch (cOpt) {
        case '1':
            removeSeller(cart);
            break;

        case '2':
        case '3':
        	printf ("Input Product ID: ");
            scanf ("%d", &nCurID);
            //linear search
            for (i = 0; i < cartLen(cart) && cart[i].item.nProID != nCurID; i++);
            
            if (i == cartLen(cart))
            	printf ("No such product ID in cart!\n");
            	
            else if (cOpt == '2')
            	removeCart(cart, i); //remove the input product ID from cart
            	
            else if (cOpt == '3') {
            	do {
            		printf ("Enter new quantity: ");
            		scanf ("%d", &nCurID);
            		
            		if (nCurID < 0) //if new quantity is invalid
            			printf (ERROR);
            		else if (!nCurID) {//if the user input 0
            			do {
            				printf ("Do you wish to remove this item from the cart? (Y/N) ");
            				scanf (" %c", &cOpt2);
            				cOpt2 = toupper(cOpt2);
            			} while (cOpt2 != 'N' && cOpt2 != 'Y');
            			
            			if (cOpt2 == 'Y')//removes if the user input 'y' or 'Y'
            				removeCart(cart, i);
					}	                			
            		else if (nCurID > cart[i].item.nQuan) {
            			printf ("Quantity entered is more than the current stocks!\n");
            			nCurID = -1;
            		}
				} while (nCurID < 0);
				
				if (nCurID)
					cart[i].nBuyQuan = nCurID;
			}
            break;
            
        case '4':
            break;
            
		default:
			printf (ERROR);
	}
	
	return cOpt;
}

/* Asks the user for a seller ID, then removes from the cart
   all the items sold by that seller
   @param cart is the pointer to the user's cart
   @param nUID is the user's ID
*/
void
removeSeller(cartType cart[])
{
	int nID = 0, i;
	
	printf ("Enter Seller ID: ");
	scanf ("%d", &nID);
	
	for (i = 0; i < cartLen(cart) && cart[i].item.nID != nID; i++);
	
	if (i == cartLen(cart)) {
		printf ("Seller ID not found in the cart!\n");
		while(getchar() != '\n');
	}
	else
		do
			removeCart(cart, i);
		while (cart[i].item.nID == nID);
}

/* Removes a cart item and rearranges the cart
   @param cart is the array of items to be bought
   @param nPosition is the index of the item to be removed
*/
void
removeCart(cartType cart[], int nPosition)
{
	int i;
	
	for (i = nPosition; i < cartLen(cart) - 1; i++)
		cart[i] = cart[i + 1];
		
	cart[i].nBuyQuan = cart[i].item.nID = 0; //initialize
}

/* Gets the current date and calls the checkoutMenu()
   @param listItems is the pointer to the list of items
   @param cart is the pointer to user's cart
   @param listUsers iss the pointer to the list of users
   @param nUID is the user's ID
*/
void
checkOut(itemType listItems[][M_ITEM], cartType cart[], userType listUsers[], int nUID)
{
	tranType tran;
	int i;
	
	char cOpt = '\0';
	
	//initialize tran
	tran.nBuyID = nUID;
	tran.fTotal = 0;
	for (i = 0; i < 5; i++)
		tran.item[i].nProID = 0;
	
	printf ("========================Check Out=========================\n");
	
	getDate(&tran.date);
	checkUpdates(listItems, listUsers, cart);
		
	while (cOpt != '4' && cartLen(cart))
		cOpt = checkOutMenu(listItems, cart, listUsers, tran);
	
	if (!cartLen(cart))
		printf("Cart is now empty!\n");
}

/* Gets the date input of the user and validates it.
   @param date is the pointer to a date structure
*/
void
getDate(dateType* date)
{
	int aDayCheck[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	do {
		printf ("\nEnter Year: ");
		scanf ("%d", &date->nYear);
		if (date->nYear < 1)
			printf (ERROR);
	} while (date->nYear < 1);
	
	if ((!(date->nYear % 4) && date->nYear % 100) || !(date->nYear % 400))
		aDayCheck[1] = 29;
	
	do {
		printf ("\nEnter Month: ");
		scanf ("%d", &date->nMonth);
		if (date->nMonth < 1 || date->nMonth > 12)
			printf (ERROR);
	} while (date->nMonth < 1 || date->nMonth > 12);
	
	do {
		printf ("\nEnter Day: ");
		scanf ("%d", &date->nDay);
		if (date->nDay < 1 || date->nDay > aDayCheck[date->nMonth - 1])
			printf (ERROR);
	} while (date->nDay < 1 || date->nDay > aDayCheck[date->nMonth - 1]);
}

/* Checks if there is a change in the quantity of stocks or prices
   with the items inside the user's cart.
   @param list is the pointer to the list of items
   @param listUsers is the pointer to the list of users
   @param cart is the pointer to the user's cart
*/
void
checkUpdates (itemType list[][M_ITEM], userType listUsers[], cartType cart[])
{
	int i, j, k, prompt = 0;
	
	for (i = 0; i < cartLen(cart); i++) {
		j = uniqueID(listUsers, cart[i].item.nID); // retrieves the index of the seller's ID for the row number in the 2D array
		for (k = 0; k < itemLen(list[j]) && list[j][k].nProID != cart[i].item.nProID; k++);
		
		if (cart[i].item.nQuan != list[j][k].nQuan) {
			printf ("Product ID %d has changed its quantity from %d to %d.\n",
			        list[j][k].nProID, cart[i].item.nQuan, list[j][k].nQuan);
			cart[i].item.nQuan = list[j][k].nQuan;

			prompt = 1;
		}
		
		if (cart[i].item.fPrice!= list[j][k].fPrice) {
			printf ("Product ID %d has changed its price from %.2f to %.2f.\n",
			        list[j][k].nProID, cart[i].item.fPrice, list[j][k].fPrice);
			cart[i].item.fPrice = list[j][k].fPrice;
			prompt = 1;
		}
	}
	
	if (prompt)
		printf ("\nYou can still go to the edit cart menu if you wish to make any changes.\n");
}

/* Compares the quantity in cart to the seller's stocks
   @param cart is the pointer to the user's cart
   return 1 if all the quantities are valid, returns 0 otherwise
*/
int
isValidTransaction(cartType cart[])
{
	int status = 1, i;
	
	for (i = 0; i < cartLen(cart); i++) //check if there are any invalid transactions
		if (cart[i].nBuyQuan > cart[i].item.nQuan) {
			printf ("Product %d cannot be checked out due to lack of stocks.\n"
			        "Please modify the quantity in the Edit Cart Menu to purchase.\n", 
					cart[i].item.nProID);
			status = 0;
		}
	
	return status;
}

/* Performs different checkout options depending on the user's input.
   @param listItems is the pointer to the list of items
   @param cart is the pointer to the user's cart
   @param listUsers is the pointer to the list of users
   @param tran is a tranType structure with the user's ID and date input
*/
char
checkOutMenu(itemType listItems[][M_ITEM], cartType cart[], userType listUsers[], tranType tran)
{
	char cOpt;
	int i, j = 1, nCurSeller = 0;
	long int fPos;
	cartType tempCart[M_CART];
	tranType temp;
	FILE* fp = fopen("Transactions.dat", "rb+");
	
	if(!fp)
		fp = fopen("Transactions.dat", "wb+");
	
	if(fp) {
		dateLookup(fp, tran.date);
		fPos = ftell(fp);
		
		printf ("\n|-------------------------------|\n");
		printf ("|         Check Out Menu        |\n");
		printf ("|1 - All                        |\n");
		printf ("|2 - By a Specific Seller       |\n");
		printf ("|3 - A Specific Item            |\n");
		printf ("|4 - Exit Check Out Menu        |\n");
		printf ("|-------------------------------|\n");
		printf ("Enter Option: ");
		scanf (" %c", &cOpt);
		
		switch (cOpt) {
			case '1':
				if (isValidTransaction(cart)) { //runs if there are no invalid transactions
					printf ("\n%-10s | %-10s | %-20s | %-10s | %-11s |\n", "Quantity", 
			        		"Product ID", "Item Name", "Unit Price", "Total Price"); //header
		        	while (cartLen(cart)) {//while there are still items in the cart
			                if (nCurSeller == cart[0].item.nID || j == 1) {
			                	nCurSeller = cart[0].item.nID;
			                	printReceipt(cart[0], 0);
			                	j++;
							} else {
								printf ("Seller ID: %d\n", nCurSeller);
								printf ("Seller Name: %s\n", listUsers[uniqueID(listUsers, nCurSeller)].aName);
								nCurSeller = cart[0].item.nID;
								printReceipt(cart[0], 1);
							}
							transaction(fp, &tran, listItems[uniqueID(listUsers, nCurSeller)], cart, 0, fPos);
		        	}
		        	
		        	printf ("Seller ID: %d\n", nCurSeller);
					printf ("Seller Name: %s\n", listUsers[uniqueID(listUsers, nCurSeller)].aName);
					printf ("Transaction Finished!\n");
				} else
					printf("Transaction was not continued.\n");
				break;
				
			case '2':
				printf ("Input Seller ID: ");
				scanf ("%d", &nCurSeller);
				
				j = 0;
				for (i = 0; i < cartLen(cart) && !j; i++)
					if(cart[i].item.nID == nCurSeller)
						j = 1;
				
				if (j) {
					if (isValidTransaction(tempCart)) {
						printf ("\n%-10s | %-10s | %-20s | %-10s | %-11s |\n", "Quantity", 
			        		    "Product ID", "Item Name", "Unit Price", "Total Price"); //header
						j = 0;
						for (i = 0; i < cartLen(cart); i++)
							while (cart[i].item.nID == nCurSeller) {
								printReceipt(cart[i], j);
								transaction(fp, &tran, listItems[uniqueID(listUsers, nCurSeller)], cart, i, fPos);
							}
								
						printf ("Seller ID: %d\n", nCurSeller);
						printf ("Seller Name: %s\n", listUsers[uniqueID(listUsers, nCurSeller)].aName);
						printf ("Transaction Finished!\n");
					} else
						printf("Transaction was not continued.\n");
				} else
					printf ("Seller ID not found in cart!\n");			
				break;
				
			case '3':
				printf ("Input Product ID: ");
				scanf ("%d", &nCurSeller);
				
				for (i = 0; i < cartLen(cart) && j; i++)
					if (cart[i].item.nProID == nCurSeller) {
						j--;
						if(cart[i].nBuyQuan <= cart[i].item.nQuan) {
							nCurSeller = cart[i].item.nID;
							printReceipt(cart[i], 1);
							transaction(fp, &tran, listItems[uniqueID(listUsers, nCurSeller)], cart, i, fPos);
							printf ("Seller ID: %d\n", nCurSeller);
							printf ("Seller Name: %s\n", listUsers[uniqueID(listUsers, nCurSeller)].aName);
							printf ("Transaction Finished!\n");
						} else
							printf ("Product %d cannot be checked out due to lack of stocks.\n"
				        			"Please modify the quantity in the Edit Cart Menu to purchase.\n"
									"Transaction was not continued.\n", cart[i].item.nProID);
					}
					
				if (j)
					printf ("Product ID not found in cart!\n");
				
			case '4':
				break;
				
			default:
				printf (ERROR);
		}
		//saves the leftover transactions
		if (itemLen(tran.item) > 0) {
			fseek(fp, fPos, SEEK_SET);
			//write to Transactions.dat
			while(fread(&temp, sizeof(tranType), 1, fp) == 1) {
				fseek(fp, -1 * (int)sizeof(tranType), SEEK_CUR);
				fwrite(&tran, sizeof(tranType), 1, fp);
				fflush(fp);
				tran = temp;		
			}
			fwrite(&tran, sizeof(tranType), 1, fp);
			fflush(fp);
		}
	} else
		printf("Error! Transactions.dat was not opened!\n");
	
	return cOpt;
}

/* Saves the transaction of an item to Transactions.dat and updates inventory
   @param fp is the pointer to transcation.dat
   @param tran is the pointer to a structure containing the transaction details
   @param list is the pointer to the list of items
   @param i is the index of the item in the user's cart
   @param fPos is the position where the binary file start to be written
*/
void
transaction(FILE* fp, tranType* tran, itemType list[], cartType cart[], int i, long int fPos)
{
	int j = searchByProID(list, cart[i].item.nProID);
	int k = itemLen(tran->item);
	tranType temp;
	dateType tempDate = tran->date;
	fseek(fp, fPos, SEEK_SET);
	
	//if the seller ID changed, or 5 items were reached
	if (k)
		if (tran->item[k - 1].nID != cart[i].item.nID || k >= 5) {
			//write to Transactions.dat
			while(fread(&temp, sizeof(tranType), 1, fp) == 1) {
				fseek(fp, -1 * (int)sizeof(tranType), SEEK_CUR);
				fwrite(tran, sizeof(tranType), 1, fp);
				fflush(fp);
				*tran = temp;
			}
			fwrite(tran, sizeof(tranType), 1, fp);
			fflush(fp);
			
			//clear transaction structure
			tran->fTotal = 0;
			for (k = 0; k < 5; k++)
				tran->item[k].nProID = 0;
			k = 0;
			tran->date = tempDate;
		}
		
	tran->item[k] = cart[i].item;
	tran->fTotal += cart[i].item.fPrice * cart[i].nBuyQuan;
	
	//update inventory
	list[j].nQuan -= cart[i].nBuyQuan;
	//remove from cart
	removeCart(cart, i);
}

/* Opens Transactions.dat and leads the pointer to a specified date
   @param fp is the pointer to Transactions.dat
   @param key is the date to look for
*/
void dateLookup(FILE* fp, dateType key)
{
	int found = 0;
	tranType temp;
	
	fseek(fp, 0, SEEK_SET);
	while(!found && fread(&temp, sizeof(tranType), 1, fp) == 1) {
		if (temp.date.nYear > key.nYear)
			found = 1;
		else if (temp.date.nYear == key.nYear) {
			if (temp.date.nMonth > key.nMonth)
				found = 1;
			else if (temp.date.nMonth == key.nMonth && temp.date.nDay >= key.nDay)
				found = 1;
		}
	}
	if(found)
		fseek(fp, -(int)sizeof(tranType), SEEK_CUR);
}

/* Displays the receipt.
   @param cart is the structure containing cart item and the quantity.
   @param nNew is a 1 or 0 indicating if the header should be printed or not.
*/
void
printReceipt(cartType cart, int nNew)
{
    if (nNew == 1)
        printf ("\n%-10s | %-10s | %-20s | %-10s | %-11s |\n", "Quantity", 
	        	"Product ID", "Item Name", "Unit Price", "Total Price"); //header

	printf ("%10d | %10d | %-20s | %10.2f | %11.2f |\n", cart.nBuyQuan, 
		cart.item.nProID, cart.item.aName, cart.item.fPrice, cart.nBuyQuan * cart.item.fPrice);
}

/* Saves the cart of the user to a binary file.
   @param cart is a pointer to the structure array of the cart.
   @param nUID is the user's ID.
*/
void
saveCart(cartType cart[], int nUID)
{
	char aUID[20];
	int i;
	FILE* fp;
	
	sprintf(aUID, "%d", nUID); //places the value in nUID as a string to aUID
	strcat(aUID, ".bag");
	fp = fopen(aUID, "wb");
	
	if (cartLen(cart) && fp) {
		for (i = 0; i < cartLen(cart); i++)
			fwrite(&cart[i], sizeof(cartType), 1, fp);
		fclose(fp);
	} else if (fp) {
		fclose(fp);
		remove(aUID); //delete the file
	} else
		printf("Error: .bag file was not created!\n");
}

/* Loads the cart of the user from a binary file.
   @param cart is the pointer to the cart of the user.
   @param nUID is the user's ID.
*/
void
loadCart(cartType cart[], int nUID)
{
	char aUID[20];
	int i = 0;
	FILE* fp;
	
	sprintf(aUID, "%d", nUID); //places the value in nUID as a string to aUID
	strcat(aUID, ".bag");
	
	fp = fopen(aUID, "rb");
	
	if (fp != NULL) {
		while (fread(&cart[i], sizeof(cartType), 1, fp) == 1)
			i++;
		fclose(fp);
	}

	while (i < M_CART) {
		cart[i].nBuyQuan = cart[i].item.nID = 0;
		i++;
	}
}

/* Display the admin menu and calls functions based on the admin's input.
   		Will exit only if the admin inputs '6'.
   	@param listUsers is the pointer to the list of users.
   	@param listItems is the pointer to the list of items.
*/
void
adminMenu(userType listUsers[], itemType listItems[][M_ITEM])
{
	char cOpt;
	int i, j;
	
	system("cls");
	printf ("Hello, Admin.\n");
	do {
		printf ("\n|----------------------------------------------|\n");
		printf ("|                  Admin Menu                  |\n");
		printf ("|1 - Show All Users                            |\n");
		printf ("|2 - Show All Sellers                          |\n");
		printf ("|3 - Show Total Sales in Given Duration        |\n");
		printf ("|4 - Show Sellers Sales                        |\n");
		printf ("|5 - Show Shopaholics                          |\n");
		printf ("|6 - Show Statistics                           |\n");
		printf ("|7 - Back to Main Menu                         |\n");
		printf ("|----------------------------------------------|\n");
		printf ("Enter Option: ");
		scanf (" %c", &cOpt);
		if (getchar() != '\n') {
			cOpt = -1;
			while (getchar() != '\n');
		}
		
		system("cls");
		switch (cOpt) {
			case '1':
				printf ("\n--------------------------------------------All Users---------------------------------------------------\n\n");
				if (userLen(listUsers)) {
					printf ("%-10s | %-10s | %-20s | %-30s | %-15s |\n", "User ID", 
					        "Password", "Name", "Address", "Phone Number"); //header
					        
					for (i = 0; i < userLen(listUsers); i++)
						printf ("%10ld | %-10s | %-20s | %-30s | %15s |\n", listUsers[i].nID, 
						listUsers[i].aPass, listUsers[i].aName, listUsers[i].aAddress, listUsers[i].aContact);
				} else 
					printf ("\nNo registered users!\n");
				break;
				
			case '2':
				printf ("\n-----------------------------------------All Sellers---------------------------------------------------\n\n");
				j = 0;
				for(i = 0; i < userLen(listUsers) && !j; i++)
					if(itemLen(listItems[i]))
						j++;
				
				if (j) {
					printf ("%-10s | %-10s | %-20s | %-30s | %-15s | %-18s |\n", "User ID", 
					        "Password", "Name", "Address", "Phone Number", "# of Items Selling"); //header
					
					for (i = 0; i < userLen(listUsers); i++)
						if (itemLen(listItems[i]))
							printf ("%10ld | %-10s | %-20s | %-30s | %15s | %18d |\n", listUsers[i].nID, 
							         listUsers[i].aPass, listUsers[i].aName, listUsers[i].aAddress, listUsers[i].aContact, itemLen(listItems[i]));
				} else
					printf ("\nNo registered sellers!\n");		
				break;
					
			case '3':
				totalSales();
				break;
					
			case '4':
				sellerSales(listUsers);
				break;
				
			case '5':
				shopaholics(listUsers);
				break;
				
			case '6':
				showStatistics();
				break;
				
			case '7':
				break;
			
			default:
				printf (ERROR);
		}
	} while (cOpt != '7');
}

/* Displays the sum of all the transacations that fall within a date range
   given by the admin.
*/
void
totalSales()
{
	int terminate = 0;
	dateType start, end;
	double dTotal = 0;
	tranType tran;
	FILE* fp = fopen("Transactions.dat", "rb");
	
	if(fp) {
		printf ("START DATE\n");
		getDate(&start);
		do {
			printf ("\nEND DATE\n");
			getDate(&end);
		} while (!isValidEndDate(start, end));
		
		dateLookup(fp, start);
		while (fread(&tran, sizeof(tranType), 1, fp) == 1 && !terminate) //reads all transactions
			if (isWithinRange(start, end, tran.date))
				dTotal += tran.fTotal;
			else
				terminate = 1;
		
		printf ("Total sales from %d/%02d/%02d to %d/%02d/%02d\n", start.nYear, start.nMonth,
			     start.nDay, end.nYear, end.nMonth, end.nDay);
		printf ("\nP %.2lf\n\n", dTotal);
		
		fclose(fp);
	} else
		printf("Transactions.dat was not found!\n");
}

/* Displays all the sellers who sold items and their total revenue
   within a given date range provided by the admin.
   @param listUsers is the pointer to the list of users.
*/
void
sellerSales(userType list[])
{
	struct {
		int nID;
		char aName[21];
		float fTotal;
	} seller[M_USER];
	
	int nElem = 0, i, terminate = 0;
	dateType start, end;
	tranType tran;
	FILE* fp = fopen("Transactions.dat", "rb");
	
	if(fp) {
		printf ("START DATE\n");
		getDate(&start);
		do {
			printf ("\nEND DATE\n");
			getDate(&end);
		} while (!isValidEndDate(start, end));
		
		system("cls");
		printf ("Seller sales from %d/%02d/%02d to %d/%02d/%02d\n", start.nYear, start.nMonth,
			     start.nDay, end.nYear, end.nMonth, end.nDay);

		dateLookup(fp, start);
		while (fread(&tran, sizeof(tranType), 1, fp) == 1 && !terminate)
			if (isWithinRange(start, end, tran.date)) {
				for (i = 0; i < nElem && seller[i].nID != tran.item[0].nID; i++); // looks for an ID match
				
				if (i == nElem) {
					seller[i].nID = tran.item[0].nID;
					strcpy(seller[i].aName, list[uniqueID(list, tran.item[0].nID)].aName);
					seller[i].fTotal = tran.fTotal;
					nElem++;
				} else seller[i].fTotal += tran.fTotal;
			} else
				terminate = 1;
		
		if (nElem) {
			printf ("| %-10s | %-20s | %-12s |\n", "Seller ID", "Seller Name", "Total Sales");
			for (i = 0; i < nElem; i++)
				printf ("| %10d | %-20s | %12.2f |\n", seller[i].nID, seller[i].aName, seller[i].fTotal);
		} else
			printf ("No transactions found!\n");
		
		fclose(fp);
	} else
		printf("Transactions.dat was not found!\n");
}

/* Displays all the users who bought items and their total spending
   within a given date range provided by the admin.
   @param list is the pointer to the list of users.
*/
void
shopaholics(userType list[])
{
	struct {
		int nID;
		char aName[21];
		float fTotal;
	} buyer[M_USER];
	
	int nElem = 0, i, terminate = 0;
	dateType start, end;
	tranType tran;
	FILE* fp = fopen("Transactions.dat", "rb");
	
	if(fp) {
		printf ("START DATE\n");
		getDate(&start);
		do {
			printf ("\nEND DATE\n");
			getDate(&end);
		} while (!isValidEndDate(start, end));
		
		
		system("cls");
		printf ("Shopahollics from %d/%02d/%02d to %d/%02d/%02d\n", start.nYear, start.nMonth,
			     start.nDay, end.nYear, end.nMonth, end.nDay);
		
		dateLookup(fp, start);
		while (fread(&tran, sizeof(tranType), 1, fp) == 1 && !terminate)
			if (isWithinRange(start, end, tran.date)) {
				for (i = 0; i < nElem && buyer[i].nID != tran.nBuyID; i++); // looks for an ID match within buyer array
				
				if (i == nElem) {//no match was found
					buyer[i].nID = tran.nBuyID;
					strcpy(buyer[i].aName, list[uniqueID(list, tran.nBuyID)].aName);
					buyer[i].fTotal = tran.fTotal;
					nElem++;
				} else buyer[i].fTotal += tran.fTotal;
			} else
				terminate = 1;
		
		if (nElem) {
			printf ("| %-10s | %-20s | %-12s |\n", "User ID", "Buyer Name", "Total Sales");
			for (i = 0; i < nElem; i++)
				printf ("| %10d | %-20s | %12.2f |\n", buyer[i].nID, buyer[i].aName, buyer[i].fTotal);
		}
		else printf ("No transactions found!\n");
		
		fclose(fp);
	} else
		printf("Transactions.dat was not found!\n");
}

/* Display the statistics menu and asks the user for input
*/
void
showStatistics()
{
	int nYear = 0, nMonth = 0;
	char cOpt;
	FILE* fp;
	
	do {
		if((fp = fopen("Transactions.dat", "rb"))) {
			printf ("\n|-------------------------------|\n");
			printf ("|         Statistics Menu       |\n");
			printf ("|1 - Yearly                     |\n");
			printf ("|2 - Monthly in a Year          |\n");
			printf ("|3 - Daily in a Month           |\n");
			printf ("|4 - Back to Main Menu          |\n");
			printf ("|-------------------------------|\n");
			printf("Enter Option: ");
			scanf(" %c", &cOpt);
			if (getchar() != '\n') {
				while (getchar() != '\n');
				cOpt = -1;
			}
			
			switch (cOpt) {
				case '1':
					printf("Enter Starting Year: ");
					scanf("%d", &nYear);
					while(nYear <= 0) {
						printf(ERROR);
						printf("Enter Starting Year: ");
						scanf("%d", &nYear);
					}
					
					printf("Enter Ending Year: ");
					scanf("%d", &nMonth);
					while(nMonth < nYear) {
						printf(ERROR);
						printf("Enter Ending Year: ");
						scanf("%d", &nMonth);
					}
					computeYearStats(fp, nYear, nMonth);
					break;
					
				case '2':
					printf("Enter Year: ");
					scanf("%d", &nYear);
					while(nYear <= 0) {
						printf(ERROR);
						printf("Enter Year: ");
						scanf("%d", &nYear);
					}
					computeMonthStats(fp, nYear);
					break;
					
				case '3':
					printf("Enter Year: ");
					scanf("%d", &nYear);
					while(nYear <= 0) {
						printf(ERROR);
						printf("Enter Year: ");
						scanf("%d", &nYear);
					}
					
					printf("Enter Month: ");
					scanf("%d", &nMonth);
					while(nMonth < 0 || nMonth > 12) {
						printf(ERROR);
						printf("Enter Month: ");
						scanf("%d", &nMonth);
					}
					computeDailyStats(fp, nYear, nMonth);
					
					break;
				
				case '4':
					break;
					
				default:
					printf (ERROR);
			}
			fclose(fp);
		} else
			printf("Transactions.dat not found!\n");
	} while (cOpt != '4');
}

/* Gathers annual sales for a range of years
   @param fp is the file pointer to Transactions.dat
   @param nYearStart is the start of the year range
   @param nYearEnd is the end of the year range
*/
void
computeYearStats(FILE* fp, int nYearStart, int nYearEnd)
{
	dateType date;
	tranType temp;
	int i;
	double arrTotal[nYearEnd - nYearStart + 1], largest = 0;
	
	date.nYear = nYearStart;
	date.nMonth = date.nDay = 1;
	
	dateLookup(fp, date);
	
	for (i = 0; i <= nYearEnd - nYearStart; i++) //initialize
		arrTotal[i] = 0;
	
	i = 0;
	while (fread(&temp, sizeof(tranType), 1, fp) == 1 && i <= nYearEnd - nYearStart) {
		if (temp.date.nYear == nYearStart + i)
			arrTotal[i] += temp.fTotal;
		else {
			if (arrTotal[i] > largest)
				largest = arrTotal[i];
			i++;
			fseek(fp, -1 * (int)sizeof(tranType), SEEK_CUR);
		}
	}
	
	if (i <= nYearEnd - nYearStart && arrTotal[i] > largest) //last comparison, just in case eof was reached before comparing
		largest = arrTotal[i];
	
	displayGraph(arrTotal, largest, nYearEnd - nYearStart, nYearStart);
}

/* Gathers monthly data for a specific year
   @param fp is the file pointer to Transactions.dat
   @param nYear is the year of interest
*/
void
computeMonthStats(FILE* fp, int nYear)
{
	dateType date;
	tranType temp;
	int i;
	double arrTotal[12], largest = 0;
	
	date.nYear = nYear;
	date.nMonth = date.nDay = 1;
	
	dateLookup(fp, date);
	
	for (i = 0; i < 12; i++)
		arrTotal[i] = 0; //initialize
	
	i = 0;
	while (fread(&temp, sizeof(tranType), 1, fp) == 1 && i < 12) {
		if (temp.date.nYear == nYear && temp.date.nMonth == i + 1)
			arrTotal[i] += temp.fTotal;
		else {
			if (arrTotal[i] > largest)
				largest = arrTotal[i];
			i++;
			fseek(fp, -1 * (int)sizeof(tranType), SEEK_CUR);
		}
	}
	
	if (i < 12 && arrTotal[i] > largest) //last comparison, just in case eof was reached before comparing
		largest = arrTotal[i];
		
	printf("Statistics for the year %d:\n", nYear);	
	displayGraph(arrTotal, largest, 11, 1);
}

/* Gathers daily data for a specific month
   @param fp is the file poniter to Transactions.dat
   @param nYear is the year of interest
   @param nMonth is the month of interest
*/
void 
computeDailyStats(FILE* fp, int nYear, int nMonth)
{
	dateType date;
	tranType temp;
	int i, nDays;
	double arrTotal[31], largest = 0;
	
	switch(nMonth) {
		case 2:
			nDays = 28;
			break;
			
		case 4:
		case 6:
		case 9:
		case 11:
			nDays = 30;
			break;
			
		default:
			nDays = 31;
	}
	
	date.nYear = nYear;
	date.nMonth = nMonth;
	date.nDay = 1;
	
	dateLookup(fp, date);
	
	for (i = 0; i < nDays; i++)
		arrTotal[i] = 0;
	
	i = 0;
	while (fread(&temp, sizeof(tranType), 1, fp) == 1 && i < nDays) {
		if (temp.date.nYear == nYear && temp.date.nMonth == nMonth && temp.date.nDay == i + 1)
			arrTotal[i] += temp.fTotal;
		else {
			if (arrTotal[i] > largest)
				largest = arrTotal[i];
			i++;
			fseek(fp, -1 * (int)sizeof(tranType), SEEK_CUR);
		}
	}
	
	if (i < nDays && arrTotal[i] > largest) //last comparison, just in case eof was reached before comparing
		largest = arrTotal[i];
		
	printf("Statistics for the Month %d of Year %d:\n", nMonth, nYear);	
	displayGraph(arrTotal, largest, nDays - 1, 1);
}

/* Displays the bar graph along with the high, mean, and low
   @param arr is the pointer to the list of values
   @param dHigh is the highest among the values received
   @param nLimit is the number of elements in the array
   @param nStart is the starting number to be printed (Day, Month, or Year)
*/
void
displayGraph(double arr[], double dHigh, int nLimit, int nStart)
{
	int i, j;
	double dTotal = 0, dLow = arr[0];
	double multiplier = dHigh / 30;
	
	for(i = 0; i <= nLimit; i++) {
		printf("\n%02d | ", nStart + i);
		for(j = 0; j < ceil(arr[i] / multiplier); j++)
			printf("%c", 223);
		
		while(j++ < 31)
			printf(" ");
		printf("%12.2lf", arr[i]);
		
		if(arr[i] < dLow)
			dLow = arr[i];
		
		dTotal += arr[i];
	}
	
	printf("\n\nHigh: %12.2lf\n", dHigh);
	printf("Low:  %12.2lf\n", dLow);
	printf("Mean: %12.2lf\n", dTotal / nLimit + 1);
}

/* Checks if the end date is after or the same as the start date
   @param start is the starting date structure
   @param end is the ending date structure
   @returns 1 if the end date is valid, 0 otherwise
*/
int
isValidEndDate(dateType start, dateType end)
{
	int valid = 1;

	if (start.nYear > end.nYear)
		valid = 0;
	else if (start.nYear == end.nYear) {
		if (start.nMonth > end.nMonth)
			valid = 0;
		else if (start.nMonth == end.nMonth && start.nDay > end.nDay)
			valid = 0;
	}
	
	if (!valid)
		printf("Invalid End Date!\n");
	
	return valid;
}

/* Checks to see if the date is between the start and the end date.
   @param start is the starting date structure.
   @param end is the ending date structure.
   @param date is the date at question.
   @returns 1 if the date is in the range, 0 otherwise.
*/
int
isWithinRange(dateType start, dateType end, dateType date)
{
	if(isValidEndDate(start, date)) {
		if (date.nYear < end.nYear)
			return 1;
		else if (date.nYear == end.nYear) {
			if (date.nMonth < end.nMonth)
				return 1;
			else if(date.nMonth == date.nMonth && date.nDay <= end.nDay)
				return 1;
		}
	}
		
	return 0;
}

/* Saves the user list to Users.txt.
   @param list is the pointer to the list of users.
*/
void
saveUsers(userType list[])
{
	int i;
	FILE* fp = fopen("Users.txt", "w");
	
	if (fp) {
		for (i = 0; i < userLen(list); i++) {
			fprintf (fp, "%ld ", list[i].nID);
			fprintf (fp, "%s\n", list[i].aPass);
			fprintf (fp, "%s\n", list[i].aName);
			fprintf (fp, "%s\n", list[i].aAddress);
			fprintf (fp, "%s\n\n", list[i].aContact);
		}
		fclose(fp);
	} else
		printf("Error: Users.txt was not created!");
}

/* Saves the item list to Items.txt.
   @param fp is the pointer to the file stream
   @param list is the pointer to the list of items.
*/
void
saveItems(userType listUsers[], itemType list[][M_ITEM])
{
	int i, j;
	FILE* fp = fopen("Items.txt", "w");
	
	if (fp) {
		for (i = 0; i < userLen(listUsers); i++)
			for (j = 0; j < itemLen(list[i]); j++) {
				fprintf (fp, "%d ", list[i][j].nProID);
				fprintf (fp, "%d\n", list[i][j].nID);
				fprintf (fp, "%s\n", list[i][j].aName);
				fprintf (fp, "%s\n", list[i][j].aCategory);
				fprintf (fp, "%s\n", list[i][j].aDescription);
				fprintf (fp, "%d ", list[i][j].nQuan);
				fprintf (fp, "%f\n\n", list[i][j].fPrice);
			}
		fclose(fp);
	} else
		printf ("Error: Items.txt was not created!");
}
