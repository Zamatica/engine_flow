
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define bool    int
#define true    1
#define false   0


typedef struct Engine_Data Engine_Data;
typedef struct Command_Holder Command_Holder;

typedef void (*Command_Func)(Engine_Data * engine);

typedef struct Command_Holder
{
	const char * command;
	Command_Func response;
	Command_Holder * next;
} Command_Holder;


typedef struct Engine_Data
{
	char * input;
	bool running;
	Command_Holder * commands;
} Engine_Data;



bool StrCmp(const char *str1, const char *str2)
{
	while (!(*str1 ^ *str2++))
		if (!*str1++)
			return true;
	
	return false;
}


void Commmand_Exit(Engine_Data * engine)
{
	engine->running = false;
}


void Add_Commands(Engine_Data * engine, const char * command, Command_Func response)
{
	Command_Holder * head = engine->commands;

	Command_Holder * new_command = (Command_Holder *)malloc(sizeof(Command_Holder));
	new_command->command = command;
	new_command->response = response;
	new_command->next = NULL;

	if (head == NULL)
	{
		engine->commands = new_command;
		return;
	}

	while (head->next)
		head = head->next;
	
	head->next = new_command;
}


void Input(Engine_Data * engine)
{
	Command_Holder * head = engine->commands;

	while (head)
	{
		if (StrCmp(head->command, engine->input))
		{
			head->response(engine);
			return;
		}
		head = head->next;
	}

	if (head == NULL)
	{
		printf("Unkown Command.\n");
	}
}


void Engine_Free(Engine_Data ** engine)
{
	free((*engine)->input);
	while((*engine)->commands)
	{
		Command_Holder * tmp = (*engine)->commands->next;
		free((*engine)->commands);
		(*engine)->commands = tmp;
	}
	*engine = NULL;
}


Engine_Data * Engine_New(void)
{
	Engine_Data * engine = (Engine_Data *)malloc(sizeof(Engine_Data));
	engine->input = malloc(sizeof(char) * 4096);
	engine->commands = NULL;
	engine->running = true;
	return engine;
}


void Engine()
{
	Engine_Data * engine = Engine_New();

	Add_Commands(engine,    "Continue",   Commmand_Exit);
	Add_Commands(engine,    "New",        Commmand_Exit);
	Add_Commands(engine,    "Save",       Commmand_Exit);
	Add_Commands(engine,    "Exit",       Commmand_Exit);

	while (engine->running)
	{
		printf("Select something you want to do: \n");
		printf("     - Continue \n");
		printf("     - New      \n");
		printf("     - Save     \n");
		printf("     - Exit     \n");
		printf("  >> ");

		scanf("%s", engine->input);
		Input(engine);
	}

	Engine_Free(&engine);
}


int main()
{
	Engine();

	return 0;
}






