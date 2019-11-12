#include "lcd_menu.h"
#include <stdbool.h>

#define ENTRIES_PER_PAGE                ((uint8_t) 3)

static Menu *activeMenu = NULL;
static bool optionIsRunning = false;

void menu_setMainMenu(Menu *const menu)
{
    if (menu) {
        activeMenu = menu;
        menu->parentMenu = NULL;
    }
}

static void renderingScrollBar(Menu * const menu)
{
#define SCROLLBAR_MAX_HANDLE_HEIGHT     ((uint8_t) 27)
#define SCROLLBAR_HANDLE_WIDTH          ((uint8_t) 2)
#define SCROLLBAR_HANDLE_ANCHOR_X       ((uint8_t) 123)
#define SCROLLBAR_HANDLE_ANCHOR_Y       ((uint8_t) 2)
#define SCROLLBAR_FRAME_TOP_LEFT_X      ((uint8_t) 121)
#define SCROLLBAR_FRAME_TOP_LEFT_Y      ((uint8_t) 0)
#define SCROLLBAR_FRAME_BOTTOM_RIGHT_X  ((uint8_t) 127)
#define SCROLLBAR_FRAME_BOTTOM_RIGHT_Y  ((uint8_t) 31)

    uint8_t handleHeight = SCROLLBAR_MAX_HANDLE_HEIGHT, handleAnchor = SCROLLBAR_HANDLE_ANCHOR_Y;

    if(menu->numberOfEntries > ENTRIES_PER_PAGE){
        handleHeight /= (menu->numberOfEntries - 2);
        handleAnchor += handleHeight * menu->pageAnchor;
    }

    lcd_setFrame(SCROLLBAR_FRAME_TOP_LEFT_X, SCROLLBAR_FRAME_TOP_LEFT_Y, SCROLLBAR_FRAME_BOTTOM_RIGHT_X, SCROLLBAR_FRAME_BOTTOM_RIGHT_Y);
    lcd_setBar(SCROLLBAR_HANDLE_ANCHOR_X, handleAnchor, SCROLLBAR_HANDLE_ANCHOR_X + SCROLLBAR_HANDLE_WIDTH, handleAnchor+handleHeight);
}


static void renderingMenuPage(Menu * const menu)
{
    char tempLcdString[MENU_CHARACTERS_PER_LINE] = {};

    if(menu->cursorPosition > menu->pageAnchor + 2)
        menu->pageAnchor++;
    else if(menu->cursorPosition < menu->pageAnchor)
        menu->pageAnchor--;

    uint8_t numberOfRows = ENTRIES_PER_PAGE;
    if(menu->numberOfEntries < ENTRIES_PER_PAGE)
        numberOfRows = menu->numberOfEntries;

    for(uint8_t rowIndex = 0; rowIndex < numberOfRows; rowIndex++){
        MenuEntry * currentEntry = menu->list[rowIndex + menu->pageAnchor];
        bool contrastIsInverted = (rowIndex + menu->pageAnchor == menu->cursorPosition);
        char featureSymbol = ' ';

        if(currentEntry->type == MENU_SUBMENU)
            featureSymbol = '>';

        sprintf(tempLcdString, "%-18s %c", currentEntry->description, featureSymbol);
        lcd_setString(2, 8+8*rowIndex, tempLcdString, LCD_FONT_8, contrastIsInverted);
    }
}

static void renderingMenu(Menu * const menu)
{
    lcd_clear();
    lcd_setSymbol8(0, 0, LCD_MENU_SYMBOL, false);
    lcd_setString(14, 0, menu->description, LCD_FONT_8, false);
    renderingMenuPage(menu);
    renderingScrollBar(menu);
    lcd_show();
}


static menuUserDrawItem2 userDrawItemCallback2;
static void* userData;


void menu_registerDrawMenuItem2(menuUserDrawItem2 func, void* data){
	// register this function pointer
	userDrawItemCallback2 = func;
	userData = data;
}


static void renderingAction(MenuEntry *const entry)
{
    lcd_clear();
    //$DND ?? where ?? lcd_runningMan(120, 0, false);
    lcd_setSymbol8(0,0, LCD_PLAY_SYMBOL, false);
    lcd_setString(14,0,entry->description, LCD_FONT_8, false);
    lcd_setFrame(0, 8, 127, 31);
    userDrawItemCallback2((int) entry->event,userData);	
    lcd_show();
}

void menu_show()
{
    if (!activeMenu)
        return;

    if (optionIsRunning)
        renderingAction(activeMenu->list[activeMenu->cursorPosition]);
    else
        renderingMenu(activeMenu);
}

menu_event menu_update(menu_navigation navigation)
{
    if (!activeMenu)
        return MENU_NO_EVENT;

    MenuEntry *currentEntry = activeMenu->list[activeMenu->cursorPosition];


    if (optionIsRunning) {
        if (navigation == MENU_SELECT) {
            optionIsRunning = false;
            return MENU_STOP_EVENT;
        }
    } else {
        switch (navigation) {
        case MENU_SELECT:
            if (currentEntry->type == MENU_EVENT) {
                optionIsRunning = true;
                return currentEntry->event;
            } break;

        case MENU_UP:
            if(activeMenu->cursorPosition > 0)
                activeMenu->cursorPosition--;
            break;

        case MENU_DOWN:
            if(activeMenu->cursorPosition < activeMenu->numberOfEntries-1)
                activeMenu->cursorPosition++;
            break;

        case MENU_BACKWARD:
            if (activeMenu->parentMenu)
                activeMenu =  activeMenu->parentMenu;
            break;

        case MENU_FORWARD:
            if (currentEntry->type == MENU_SUBMENU) {
                Menu *parentMenu = activeMenu;
                activeMenu = currentEntry->submenu;
                activeMenu->parentMenu = parentMenu;
            } break;

        default: break;
        }
    }
    return MENU_NO_EVENT;
}
