#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>
#define M_ITEM 20
#define M_USER 100
#define M_CART 10
#define ERROR "Invalid Input!\n"
#define ADPASS "H3LLo?"

typedef struct {
	int nProID, 			// Product ID
	    nID, 				// Seller ID
		nQuan;	 			// Stock Quantity
	float fPrice;			// Item Price
	char aName[22],			// Item Name
		 aCategory[17],
		 aDescription[32];
} itemType;

typedef struct {				
	long int nID;			// User ID
	char aName[22],
		 aContact[17],		// Contact Number			
	     aPass[12], 		// Password
		 aAddress[32];		
} userType;

typedef struct {
	int nYear,
	    nMonth,
		nDay;
} dateType;

typedef struct {
	int nBuyID;				// Buyer's ID
	float fTotal;			// Total Amount of Transaction
	dateType date;
	itemType item[5];
} tranType;

typedef struct {
	int nBuyQuan;			// Quantity bought
	itemType item;
} cartType;

char editCart(itemType[][M_ITEM], cartType[]),
	 editStocksMenu(itemType*, char*),
	 checkOutMenu(itemType[][M_ITEM], cartType[], userType[], tranType),
	 mainMenu(),
	 viewerMode(int*, int);
	 
int addToCart(itemType[][M_ITEM], cartType*, int),
	cartLen(cartType[]),
	comparator(const void *p, const void *q),
	isValidEndDate(dateType, dateType),
	isValidTransaction(cartType[]),
	isWithinRange(dateType, dateType, dateType),
    itemLen(itemType[]),
	logIn(userType[]),
	searchByProID(itemType[], int),
	searchCart(cartType[], int),
	sellerLen(itemType[][M_ITEM]),
	uniqueID(userType[], int),
	uniqueProID(itemType[][M_ITEM], int),
	userLen(userType[]);
	
void addNewItem(itemType[][M_ITEM], int, int),
	 adminMenu(userType[], itemType[][M_ITEM]),
	 buyMenu(itemType[][M_ITEM], int, cartType[], userType[]),
	 checkOut(itemType[][M_ITEM], cartType[], userType[], int),
	 checkUpdates(itemType[][M_ITEM], userType[], cartType[]),
	 computeMonthStats(FILE*, int),
	 computeDailyStats(FILE*, int, int),
	 computeYearStats(FILE*, int, int),
	 dateLookup(FILE*, dateType),
	 displayGraph(double[], double, int, int),
	 editStocks(itemType[]),
	 getDate(dateType*),
	 inputString(char[], char[], int),
	 loadCart(cartType[], int),
	 loadItems(userType[], itemType[][M_ITEM]),
	 loadUsers(userType[]),
	 lowStock(itemType[]),
	 printReceipt(cartType, int),
	 registerUser(userType[]),
	 removeCart(cartType[], int),
	 remEnter(char[]),
     removeSeller(cartType[]),
     saveCart(cartType[], int),
	 saveItems(userType[], itemType[][M_ITEM]),
	 saveUsers(userType[]),
	 sellerSales(userType[]),
	 sellMenu(itemType[][M_ITEM], int, int),
	 shopaholics(userType[]),
	 showFullDetails(itemType),
	 showProducts(itemType[]),
	 showStatistics(),
	 sortItems(itemType[]),
	 sortCart(cartType[]),
	 totalSales(),
	 userMenu(itemType[][M_ITEM], userType[], int),
	 transaction(FILE*, tranType*, itemType[], cartType[], int, long int),
	 viewAllProducts(itemType[][M_ITEM], int, int),
	 viewByCategory(itemType[][M_ITEM], int),
	 viewByName(itemType[][M_ITEM], int),
	 viewSpecificSeller(itemType[][M_ITEM], userType[]);
