@rem Turn off echoing of commands.
@echo off

rem Make sure local variables don't spill into the global environment.
setlocal EnableExtensions

rem Make sure we're in the src directory.
pushd "%~dp0src"

rem Treat the first parameter to this batch script as a target.
set target=%1

rem If a target is specified, call the subroutine
if "%target%"=="" (call :build) else (call :%target%)
goto :end


:build
  rem Compiler flags:
  rem   /nologo Don't print tedious MS logo stuff in the beginning
  rem   /DEBUG  Compile in debug mode.
  rem   /Z7     Create debug symbols as .pdb files
  rem   /TC     Compile as C, not C++
  set CFLAGS=/nologo /DEBUG /Z7 /TC

  rem Compile the sources.
  cl %CFLAGS% main.c cpu.c gfx.c interconnect.c util.c
exit /B


:clean
  rem Delete all intermediate files.
  del *.obj *.exe *.o *.a *.lib *.pdb *.ilk
exit /B


:end
rem Restore previous directory.
popd
