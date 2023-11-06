PATH=D:\compilador\LenguajesYCompiladoresG3;

tasm number.asm
tasm final.asm
tlink final.obj number.obj
final.exe
del final.obj 
del number.obj 
del final.exe
del final.map