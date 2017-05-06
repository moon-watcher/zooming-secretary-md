@echo off
set name=zooming-secretary-md
set unit=h:

mkdir %unit%\%name%
mkdir ..\roms



set file=%date:~6,4%%date:~3,2%%date:~0,2% - %name%-debug.bin

del ..\roms\"%file%"
rem del C:\Users\Daniel\"Box Sync"\AbbayeMD2016\roms\"%file%"
rem del C:\Users\Daniel\Dropbox\Abadia64\roms\"%file%"
rem del C:\Users\Daniel\Dropbox\"Abbaye Des Morts Megadrive&Genesis"\"Rom Tests"\"%file%"
rem del "%unit%\%name%\%file%"

copy rom.bin ..\roms\"%file%"
rem copy rom.bin C:\Users\Daniel\"Box Sync"\AbbayeMD2016\roms\"%file%"
rem copy rom.bin C:\Users\Daniel\Dropbox\Abadia64\roms\"%file%"
rem copy rom.bin C:\Users\Daniel\Dropbox\"Abbaye Des Morts Megadrive&Genesis"\"Rom Tests"\"%file%"
copy rom.bin "%unit%\%name%\%file%"

rem echo %ERRORLEVEL%

rem pause