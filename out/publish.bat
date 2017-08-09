ren @echo off
set name=zooming-secretary-md
set unit=h:

@mkdir %unit%\%name%
@mkdir ..\roms

@set file=%date:~6,4%%date:~3,2%%date:~0,2% - %name%.bin

@del ..\roms\"%file%"
@del C:\Users\Daniel\"Box Sync"\zooming-secretary-md\roms\"%file%"
@del "%unit%\%name%\%file%"

copy rom.bin ..\roms\"%file%"
copy rom.bin C:\Users\Daniel\"Box Sync"\zooming-secretary-md\roms\"%file%"
copy rom.bin "%unit%\%name%\%file%"

echo %ERRORLEVEL%

pause