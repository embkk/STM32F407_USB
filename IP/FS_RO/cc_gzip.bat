@ECHO OFF
Echo CC.bat       Converting %2\%3.%4
%1\Bin2C.exe %2\%3.%4 %3
XCopy %3.* Generated\%3.* /Y /Q
del %3.c
del %3.h
IF EXIST %2\%3.%4 (
  del %2\%3.%4
)

