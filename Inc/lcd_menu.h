/**
* @brief Add MENU_LED_BLUE_EVENT
* @date 27.04.2018
* @author
*/

#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "lcd_driver.h"

#define MENU_MAX_ENTRIES 10
#define MENU_CHARACTERS_PER_LINE 22
#define MENU_OPTIONS_PER_PAGE 3

typedef enum{MENU_NOP, MENU_UP, MENU_DOWN, MENU_BACKWARD, MENU_FORWARD, MENU_SELECT}menu_navigation; /* TODO name */
//$dnd1 add MENU_LED_BLUE_EVENT
typedef enum{MENU_NO_EVENT, MENU_STOP_EVENT /* TO EXPAND */}menu_event;
typedef enum{MENU_EVENT, MENU_SUBMENU}menu_entryType;

typedef struct Menu_ Menu; /* Forward declaration */

typedef struct MenuEntry_{
    char const *description;
    menu_entryType type;
    union{
        Menu *submenu;
        menu_event event;
		//$DND
		char const *liveData;
    };
}MenuEntry;

typedef struct Menu_{
    /* public: */
    char const *description;
    uint8_t numberOfEntries;
    MenuEntry **list;
    /* private: */
    Menu *parentMenu;
    uint8_t cursorPosition;
    uint8_t pageAnchor;
}Menu;

void menu_setMainMenu(Menu *const menu);
void menu_show(void);
menu_event menu_update(menu_navigation navigation);

typedef void (*menuUserDrawItem)(int);
// a function that passes the menu event id as parameter
typedef void (*menuUserDrawItem2)(int,void*);

void menu_registerDrawMenuItem(menuUserDrawItem func);
void menu_registerDrawMenuItem2(menuUserDrawItem2 func, void* data);
	

#endif // LCD_MENU_H
