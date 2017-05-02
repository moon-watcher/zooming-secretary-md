typedef struct _Option
{
    u8    x;
    u8    x2;
    u8    y;
    u8   *string;
    u8*  (*funct) (struct _Option *);
    void (*exec) (struct _Option *);
    s32   value;
    s32   nbElements;
}
Option;




typedef struct _Menu
{
    u8 x;
    u8 y;
    u8 x2;
    u8 nbOptions;
    s8 currentOption;
    u8 lineHeight;
    Option **options;
}
Menu;

