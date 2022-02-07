//Decleration of Header files
#include "header.h"
int main()
{
	FILE* commands, * manot;
	kitchen km;
	Table resteraunt[NUM_TABLE];
	Open_Reseraunt(resteraunt);
	km.head = NULL;
	km.size = 0;
	char product_Name[100], read_instraction;
	int quntity, table_number, action;
	BOOL first_action = TRUE;
	commands = fopen("Instructions.txt", "rt");
	if (!commands)
	{
		printf("ERROR!Cannot Open the file!!");
		exit(1);
	}
	Print_Menu();
	read_instraction = _getche();
	if (fscanf(commands, "%d", &action) != EOF)
	{
		if (action != 1)
		{
			printf("\nCannot do anything without Kitchen!!\n");
			fclose(commands);
			return 0;
		}
		else
		{
			manot = fopen("Manot.txt", "rt");
			if (!manot)
			{
				Error_Msg("ERROR!Cannot Open the file!!");
			}
			Print_Menu();
			CreateProducts(&km, manot);
			fclose(manot);
			read_instraction = _getche();
			while (read_instraction == '0')
			{
				if (fscanf(commands, "%d", &action) != EOF)
				{
					switch (action)
					{
					case 2:
						fscanf(commands, "%s%d", product_Name, &quntity);
						AddItems(&km, product_Name, quntity);
						break;
					case 3:
						fscanf(commands, "%d%s%d", &table_number, product_Name, &quntity);
						OrderItem(resteraunt, &km, table_number, product_Name, quntity);
						break;
					case 4:
						fscanf(commands, "%d%s%d", &table_number, product_Name, &quntity);
						RemoveItem(resteraunt, &km, table_number, product_Name, quntity);
						break;
					case 5:
						fscanf(commands, "%d", &table_number);
						RemoveTable(resteraunt, &km, table_number);
						break;
					default: printf("\nWrong Action Input\n");
						break;
					}
					Print_Menu();
				}
				else
				{
					printf("\n\n================END OF FILE=================\n\n");
					printf("\nPlease enter any key beside zero\n");
				}
				read_instraction = _getche();
			}
		}
		fclose(commands);
		Delete_Resteruant(resteraunt, &km);
		DeleteKitchen(&km);
		return 0;
	}
}