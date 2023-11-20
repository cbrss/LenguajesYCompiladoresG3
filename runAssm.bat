PATH=D:\compilador\LenguajesYCompiladoresG3;

asm\TASM\tasm asm\numbers.asm
asm\TASM\tasm final.asm
asm\TASM\tlink /3 /x /v /k final.obj numbers.obj
final.exe
del final.obj 
del numbers.obj 
del final.exe



