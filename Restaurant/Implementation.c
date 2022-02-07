//Decleration of Headers files.
#include "header.h"
//======================================================================================================================
void CreateProducts(p_kitchen pkm,FILE* commands)
{
	char tempSTR[100];
	p_Dish to_check;
	p_Dish new_product;
	Dish temp;
	while (fscanf(commands, "%s%d%d %c", tempSTR, &temp.Quantity ,&temp.Price, &temp.Premium) != EOF)
	{
		to_check = Search_Dish_In_Kitchen(pkm, tempSTR);
		if (to_check==NULL)//if there is no dish with that name in the kitchen create new one
		{
			if (temp.Price > 0)//Price Must be positive number
			{
				if (temp.Quantity > 0)//Quantity Must be positive Number.
				{
					new_product = (p_Dish)malloc(sizeof(Dish));
					if (!new_product)
					{
						DeleteKitchen(pkm);		
						Error_Msg("Memmory allocation ERROR");
					}
					pkm->size++;
					*new_product = temp;
					new_product->next = pkm->head;
					pkm->head = new_product;
					pkm->head->ProductName = (char*)malloc(sizeof(char)*(strlen(tempSTR)+1));//לבדוק מה הגודל
					if (!pkm->head->ProductName)
					{
						DeleteKitchen(pkm);
						Error_Msg("Memmory allocation ERROR");
					}
					strcpy(pkm->head->ProductName, tempSTR);
				}
				else
				{
					printf("\nQuantity must be positive number!\n");
				}
			}
			else
			{
				printf("\nPrice must be positive number!\n");
			}
		}
		else
		{
			printf("\n%s already exist in the kitchen!\n",tempSTR);
		}
	}
	printf("\nDishes Input succeed\n");
}
//======================================================================================================================
void AddItems(p_kitchen pkm,char*name,int add)
{
	p_Dish temp = Search_Dish_In_Kitchen(pkm,name);
	if (temp!=NULL)
	{
		if (add > 0)//If Quantity positive add it to the quantity of the dish.
		{
			temp->Quantity += add;
		}
		else
		{
			printf("\nQuantity must be positive number!\n");
		}
		printf("\nAdd quantity to stock succeed\n");
	}
	else
	{
		printf("\The dish does not exist in the kitchen!\n");
		printf("\nAdd quantity to stock did not succeed\n");
	}

}
//======================================================================================================================
void OrderItem(p_Table pt, p_kitchen pk,int TableNumber,char*ProductName,int Quantity)
{
	int add_price;
	p_Order temp;
	char premium_dish = 'N';
	p_Dish dish_from_kitchen = Search_Dish_In_Kitchen(pk,ProductName);
	p_Order to_update = Search_Dish_In_Order(&pt[TableNumber], ProductName);
	if (Table_In_Resteruant(TableNumber))//Check if table is in the reseraunt
	{
		if (dish_from_kitchen!=NULL)//Check if the dish that the diners want is in the kitchen
		{
			if (Availble_Dish(dish_from_kitchen,Quantity))//Check if there is enough quantity of the dish to sell
			{
				dish_from_kitchen->Quantity -= Quantity;//Update the stock in the kitchen.
				add_price = dish_from_kitchen->Price;
				add_price *= Quantity;
				pt[TableNumber].bill += add_price;
				if (to_update!=NULL)//If the table already ordered that dish, just update the quantity.
				{
					to_update->Quantity += Quantity;
				}
				else//Ordered New dish for the table.
				{
					pt[TableNumber].count_dishes++;
					if (dish_from_kitchen->Premium=='Y')
					{
						pt[TableNumber].Premium ='Y';
					}
					temp = Take_Order(TableNumber, ProductName, Quantity);//create new order of that dish.
					if (temp)
					{
						if (pt[TableNumber].head == NULL)
						{
							temp->next = pt[TableNumber].head;
							pt[TableNumber].tail = temp;
						}
						else
						{
							temp->next = pt[TableNumber].head;
							pt[TableNumber].head->prev = temp;
						}
						pt[TableNumber].head = temp;
						printf("\nTable %2d orderd %s% d Times", TableNumber, ProductName, Quantity);
					}
					else
					{
						Delete_Resteruant(pt,pk);
						DeleteKitchen(pk);
						Error_Msg("Memory allocation Error");
					}
				}
			}
			else
			{
				printf("\nError with Quantity of dish!\n");
			}
		}
		else
		{
			printf("\nThe kitchen does not make this dish!\n");
		}
	}
	else
	{
		printf("\nThe table is not in the resteraunt\n");
	}
}
//======================================================================================================================
BOOL Table_In_Resteruant(int i)
{
	return (i < NUM_TABLE && i >= 0);
}
//======================================================================================================================
BOOL Availble_Dish(p_Dish pd, int Quantity)
{
	return (Quantity > 0 && Quantity <= pd->Quantity);
}
//======================================================================================================================
void Open_Reseraunt(p_Table pt)
{
	int i;
	for ( i = 0; i < NUM_TABLE; i++)
	{
		pt[i].head = NULL;
		pt[i].tail = NULL;
		pt[i].bill = 0;
		pt[i].Premium = 'N';
		pt[i].count_dishes = 0;
	}
}
//======================================================================================================================
p_Order Take_Order( int TableNumber, char*ProductName, int Quantity)
{
	p_Order new_order = (p_Order)malloc(sizeof(Order));
	if (!new_order)
	{
		return NULL;
	}
	new_order->next = NULL;
	new_order->prev = NULL;
	new_order->Product_Name = (char*)malloc(sizeof(char) * (strlen(ProductName) + 1));
	if (!new_order->Product_Name)
	{
		return NULL;
	}
	strcpy(new_order->Product_Name, ProductName);
	new_order->Quantity = Quantity;
	new_order->Table_Number = TableNumber;
	return new_order;
}
//======================================================================================================================
void RemoveItem(p_Table pt, p_kitchen pk, int TableNumber, char* ProductName, int Quantity)
{
	p_Dish dish_from_kitchen = Search_Dish_In_Kitchen(pk, ProductName);
	p_Order dish_from_order = Search_Dish_In_Order(&pt[TableNumber], ProductName);
	if (pt[TableNumber].head != NULL)//check if table orderd something
	{
		if (Table_In_Resteruant(TableNumber))//Check if the table is in the reseraunt
		{
			if (dish_from_kitchen!=NULL)//Check if the kitchen make this dish
			{
				if (dish_from_order!=NULL)//Check if the the table ordered that dish.
				{
					if (Check_Quantity_to_del(dish_from_order,Quantity))//Check if the desired quantity to delete match the quantity in the order.
					{
						if (Quantity < dish_from_order->Quantity)//reduce Quantity of the dish in the order.
						{
							dish_from_order->Quantity -= Quantity;
							pt[TableNumber].bill -= (dish_from_kitchen->Price) * (Quantity);//Update BILL
						}
						else
						{
							Delete_Dish_From_Table(&pt[TableNumber], dish_from_order,dish_from_kitchen,pk);//Delete the dish from the order.
						}
						printf("\nTable %d canceled the order of %d times of %s", TableNumber, Quantity, ProductName);
					}
					else
					{
						printf("\nQuantity Error!\n");
						printf("\nTable %d did not succeed canceling the the order of %d times of %s", TableNumber, Quantity, ProductName);
					}
				}
				else
				{
					printf("\nThe dish is not in the table order\n");
					printf("\nTable %d did not succeed canceling the the order of %d times of %s", TableNumber, Quantity, ProductName);
				}
			}
			else
			{
				printf("\nThe kitchen does not make this dish!\n");
				printf("\nTable %d did not succeed canceling the the order of %d times of %s", TableNumber, Quantity, ProductName);
			}
		}
		else
		{
			printf("\nTable is not exist in the resteraunt!\n");
			printf("\nTable %d did not succeed canceling the the order of %d times of %s", TableNumber, Quantity, ProductName);
		}
	}
	else
	{
		printf("\nThis table didnt make any orders!\n");
		printf("\nTable %d did not succeed canceling the the order of %d times of %s", TableNumber, Quantity, ProductName);
	}
}
//======================================================================================================================
void Delete_Dish_From_Table(p_Table pt, p_Order order,p_Dish from_kitchen,p_kitchen pk)
{
	p_Order to_del;
	int price_to_sub = from_kitchen->Price;
	if (order == pt->head)
	{
		to_del = pt->head;
		pt->head->prev = NULL;
		pt->head = pt->head->next;
	}
	else
	{
		if (order != pt->tail)
		{
			to_del = order;
			order->prev->next = order->next;
			order->next->prev = order->prev;		
		}
		else
		{ 
			to_del = pt->tail;
			pt->tail = order->prev;
			order->prev->next = order->next;	
		}
	}
	price_to_sub *= order->Quantity;
	pt->count_dishes--;
	pt->bill -= price_to_sub;
	free(to_del->Product_Name);
	free(to_del);
	if (pt->count_dishes == 0)
	{
		pt->head = NULL;
		pt->tail = NULL;
		pt->Premium - 'N';
		pt->bill = 0;
	}
	else
	{
		//Update premium feature if neccessary
		if (Check_Premium(pt, pk))
		{
			pt->Premium = 'Y';
		}
		else
		{
			pt->Premium = 'N';
		}
	}
}
//======================================================================================================================
BOOL Check_Premium(p_Table pt,p_kitchen pk)
{
	p_Dish from_kitchen;
	p_Order temp = pt->head;
	BOOL flag = FALSE;
	while (temp != NULL && flag == FALSE)
	{
		from_kitchen = Search_Dish_In_Kitchen(pk, temp->Product_Name);
		if (from_kitchen->Premium == 'Y')
			flag = TRUE;
		temp = temp->next;
	}
	return flag;
}
//======================================================================================================================
BOOL Check_Quantity_to_del(p_Order po, int Quantity)
{
	if (Quantity > 0 && Quantity <= po->Quantity)
		return TRUE;
	else
		return FALSE;
}
//======================================================================================================================
void RemoveTable(p_Table pt,p_kitchen pk,int table_number)
{
	p_Dish from_kitchen;
	if (pt[table_number].head == NULL)
	{
		printf("\nThe Table did not make any orders!\n");
	}
	else
	{
		Print_Receipt(&pt[table_number],pk,table_number);
	}
	//Closing the table.
	while (pt[table_number].head != NULL)
	{
		from_kitchen = Search_Dish_In_Kitchen(pk, pt[table_number].head->Product_Name);
		Delete_Dish_From_Table(&pt[table_number],pt[table_number].head,from_kitchen,pk);
	}
}
//======================================================================================================================
void Print_Receipt(p_Table pt,p_kitchen pk,int table_nubmer)
{

	double final_bill = pt->bill;
	p_Order temp = pt->head;
	if (pt->Premium == 'Y')
	{
		final_bill = pt->bill * 1.1;
		printf("\n******PREMIUM TABLE:*****\n");
	}
	else
	{
		printf("\nRegular Table:\n\n");
	}
	//temp - pt->head;
	printf("Table Number:%d\n", table_nubmer);
	printf("The Waiters were Meitar Elezra and Tuval Zitelbach\n");
	printf("The Table order:\n");
	while (temp!=NULL)
	{
		printf("%-10s%5d\n", temp->Product_Name,temp->Quantity);
		temp = temp->next;
	}
	printf("The final bill is :%.2lf\n", final_bill);
}
//======================================================================================================================
void DeleteKitchen(p_kitchen pkm)
{
	p_Dish temp;
	while (pkm->head != NULL)
	{
		temp = pkm->head;
		pkm->head = pkm->head->next;
		free(temp->ProductName);
		free(temp);
	}
}
//======================================================================================================================
void Delete_Resteruant(p_Table pt)
{
	p_Order temp;
	int i;
	for (i = 0; i < NUM_TABLE; i++)
	{
		while (pt[i].head != NULL)
		{
			temp = pt[i].head;
			pt[i].head = pt[i].head->next;
			free(temp->Product_Name);
			free(temp);
		}
	}
}

//======================================================================================================================
p_Dish Search_Dish_In_Kitchen(p_kitchen pkm, char*Dish_Name)
{
	p_Dish temp = pkm->head;
	BOOL flag = FALSE;
	while (temp != NULL && flag == FALSE)
	{
		if (strcmp(temp->ProductName, Dish_Name) == 0)
		{
			flag = TRUE;
		}
		else
		{
			temp = temp->next;
		}
	}
	return temp;
}
//======================================================================================================================
p_Order Search_Dish_In_Order(p_Table pt, char*Dish_Name)
{
	p_Order temp = pt->head;
	BOOL flag = FALSE;
	while (temp != NULL && flag == FALSE)
	{
		if (strcmp(temp->Product_Name, Dish_Name) == 0)
		{
			flag = TRUE;
		}
		else
		{
			temp = temp->next;
		}
	}
	return temp;
}
//======================================================================================================================
void Print_Menu()
{
	printf("\n*********METZ - RESERAUNT*********\n");
	printf("Welcome to our Resteruant Please follow upcomming instruction\n");
	printf("Instruction FIle:\n");
	printf("1  Inputing Dishes to the kitchen\n");
	printf("2  Adding Stock of specific Dish\n");
	printf("3  Order Dishes to specific Table\n");
	printf("4  Partial canceling of order\n");
	printf("5  Closing specific Tabe\n");
	printf("Please Press 0 To Complete one instruction from the instruction file\n");
	printf("Press Any key to End the program\n");
	printf("Have Good Time in our resteraunt\n");
}
//======================================================================================================================
/*Function Name:Error_msg
Input:string
Output:void
Algorithm: print given string and exit the program with error.
*/
void Error_Msg(char* msg)
{
	printf("%s\n", msg);
	exit(1);
}