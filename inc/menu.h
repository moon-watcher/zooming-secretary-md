#ifndef _MENU_H_
#define _MENU_H_


    typedef struct _Option
    {
        u8    string[40];
        u8*  (*fDirection) (struct _Option *);
        void (*fButton)    (struct _Option *);
        s32   value;
        s32   max;

        // For private use
        u8 x, y, x2;
    }
    Option;


    typedef struct _Menu
    {
        u8      x, y, x2;
        u8      nbOptions;
        s8      currentOption;
        u8      lineHeight;
        Option *options[20];
    }
    Menu;



    void MenuInit           ( Menu *menu );

    void MenuSetX           ( Menu *menu, u8 x );
    void MenuSetY           ( Menu *menu, u8 y );
    void MenuSetX2          ( Menu *menu, u8 x2 );
    void MenuSetPosition    ( Menu *menu, u8 x, u8 y, u8 x2 );
    void MenuSetLineHeight  ( Menu *menu, u8 lineHeight );
    void MenuSetOption      ( Menu *menu, s8 currentOption );

    void MenuAddOption      ( Menu *menu, Option *option );

    void MenuDrawOption     ( Menu *menu, u8 index );
    void MenuDrawValue      ( Menu *menu, u8 index );
    void MenuDraw           ( Menu *menu );

    void MenuLoop           ( Menu *menu );

    void MenuOptionInc      ( Option *option );
    void MenuOptionDec      ( Option *option );
    void MenuOptionFunction ( Option *option );


#endif // _MENU_H_
