@echo off
goto :CopyPDBDataOnBuild
==========================================================================================
CopyPDBDataOnBuild
  Usage:
    This script sets copies the required PDB files for the project into the "Build Directory"

  Notes:
   

==========================================================================================
:CopyPDBDataOnBuild

echo xcopy "%~dp0..\Uog\lib\%1"*.pdb %2 /s /y /d /r 
xcopy "%~dp0..\deps\Uog\lib\%1"*.pdb %2 /s /y /d /r 

@echo off
