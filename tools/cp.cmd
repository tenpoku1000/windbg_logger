
mkdir bin
mkdir bin\Target\KmdfDriver

copy /y ..\INSTALL.md .\bin\INSTALL.md
copy /y ..\README.md .\bin\README.md
copy /y "C:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe" .\bin\devcon.exe

copy /y ..\bin\Target\UmCmd.exe .\bin\Target\UmCmd.exe

copy /y ..\bin\Target\KmdfDriver\KmdfDriver\KmdfDriver.inf .\\bin\Target\KmdfDriver\KmdfDriver.inf
copy /y ..\bin\Target\KmdfDriver\KmdfDriver\KmdfDriver.sys .\\bin\Target\KmdfDriver\KmdfDriver.sys
copy /y ..\bin\Target\KmdfDriver\KmdfDriver\KmdfDriver.cat .\\bin\Target\KmdfDriver\KmdfDriver.cat

