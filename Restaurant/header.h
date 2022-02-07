#ifndef header
#define header
//Avoid Visual Studio 2019 warnings
#define _CRT_SECURE_NO_WARNINGS
//==============================================================================================
//Declertaion of headers files
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
//Decleration of constants using macro
#define NUM_TABLE 15
//==============================================================================================
//Decleration of structures.
//Struct to present dish in the menu.
typedef struct Dish
{
	char* ProductName;
	int Price;
	int Quantity;
	char Premium;
	struct Dish* next;
}Dish,*p_Dish;
//***************************************
//Struct to present the kitchhen of the resteraunt.
typedef struct Kitchen_manager
{
	p_Dish head;
	int size;
}kitchen,*p_kitchen;
//***************************************
//Struct to present order of costumer.
typedef struct Order
{
	int Table_Number;
	char* Product_Name;
	int Quantity;
	struct Order* next;
	struct Order* prev;
}Order, * p_Order;
//Struct to present the 
typedef struct Table_Manager
{
	int bill;
	char Premium;
	int count_dishes;
	p_Order head;
	p_Order tail;
}Table,*p_Table;
typedef enum BOOL {FALSE,TRUE}BOOL;
//==============================================================================================
//Decleration of Functions
//==============================================================================================
//Create List of Dishes in the kitchen from text file.
void CreateProducts(p_kitchen ,FILE* f);

//Add quantity to specific dish in the kitchen.
void AddItems(p_kitchen, char* name, int add);

//Take order of specific dish to specific table 
void OrderItem(p_Table , p_kitchen , int , char* , int );

//Paritall canceling of dish in table's order.
void RemoveItem(p_Table,p_kitchen,int, char*, int);

//Close table and give him the check.
void RemoveTable(p_Table, p_kitchen,int);
//==============================================================================================
//Decleration of sub Functions.
//==============================================================================================

//Return pointer to dish in the kitchen with the same name.
p_Dish Search_Dish_In_Kitchen(p_kitchen, char*);

//Return pointer to order in the table with the same name of dish.
p_Order Search_Dish_In_Order(p_Table, char*);

//Creat Order element.
p_Order Take_Order(int, char*, int);

//Check if the index of the table is in the resteraunt.
BOOL Table_In_Resteruant(int);

//Check if there is enough quantity of the dish in the kitchen in order to take order.
BOOL Availble_Dish(p_Dish,int);

//Check if the quantity of the dish in the order is positive or smaller or equal to the quantity that has been ordered.
BOOL Check_Quantity_to_del(p_Order, int);

//Check if the table's order contains premium dishes.
BOOL Check_Premium(p_Table,p_kitchen);

//Print ERROR message and exit the program with an error.
void Error_Msg(char*);

//Initallize resteraunt setting.
void Open_Reseraunt(p_Table);

//Delet all the dishes in the kitchen.
void DeleteKitchen(p_kitchen);

//Delete all the tables in the resteraunt.
void Delete_Resteruant(p_Table,p_kitchen);

//Delete a dish from table's order.
void Delete_Dish_From_Table(p_Table,p_Order,p_Dish,p_kitchen);

//Print the recepit of the table order.
void Print_Receipt(p_Table, p_kitchen,int);

//Print the menu of the program
void Print_Menu();
//==============================================================================================
#endif
