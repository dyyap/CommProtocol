# Visual Studio 15 2017 Protobuf Setup Guide

Visual Studio has a setup that is very different from any other C++ IDE, it compiles Protobuf
in the IDE but it takes a couple of steps
## Environment Setup

Download [x86 Cmake](https://cmake.org/download/)

Download [Git](https://git-scm.com/downloads)

You will also need a .proto file if you do not have one initially tutorial is on [Protobuf](https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md)


Installation
=================
Create a Visual Studio Project

Open x64 Native Tools Command Prompt for VS 2017 with Admin

Follow [Protobuf](https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md) setup until
right before Cmake Configuration

Scroll down to Zlib support and follow steps
 
Continue to Cmake Configuration until the Creation of the Solution file

Visual Studio Solution File Configuration
==============================================

To create the Visual Studio Studio File:


     C:\Path\to\protobuf\cmake\build>mkdir solution & cd solution
     C:\Path\to\protobuf\cmake\build\solution>cmake -G "Visual Studio 14 2015 Win64" ^
     -DCMAKE_INSTALL_PREFIX=../../../../install ^
     ../..

Then go to this folder

    C:\Path\to>\protobuf\cmake\build\solution
    
and open CMakeCache.txt and change

    Visual Studio 14 2015

to
    
    Visual Studio 15 2017
    
To Create the new Visual Studio files 

    
     C:\Path\to\protobuf\cmake\build>mkdir solution & cd solution
     C:\Path\to\protobuf\cmake\build\solution>cmake -G "Visual Studio 15 2017 Win64" ^
     -DCMAKE_INSTALL_PREFIX=../../../../install ^
     ../..
     
The proceed back to [Protobuf](https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md) setup 
and complete installation

Making the Header and CC files
===============================

In a file explorer copy the .proto file into 

     C:\Path\to>protobuf\cmake\build\release>

Go to the release folder in Command prompt

    C:\Path\to>protobuf\cmake\build\release>
    
Compile the CC and Header files

    protoc -I=. --cpp_out=. insertfilenamehere.proto
    
The header and cc file are then made in release folder

Building Protobuf in Visual Studio
================================================

## Using Empty Project

Open the project and right click on the Project Name in the Solution Explorer (should be bold)

Copy the pb.cc and the pb.h files into the project, you should get a lot of errors at this point

Go to ProjectName -> Configurations Manager on the top right and change everything to x64
Protobuf 3 only works for x64 for some reason

Go to ProjectName -> Configurations Properties -> VC++ Directories -> Include Directories -> <Edit>

Go to File Explorer and find the src file in protobuf and copy into include files
  
      C:\Path\to>\protobuf\src
      
   
Go to ProjectName -> Debugging -> Command Arguments -> <Edit> and add the Project name
  
      C:\Users\RandomUser\source\repos\RandomProject
      
Search for libprotobuf library and copy into project

      libprotobuf.lib
      
Go to ProjectName -> C/C++ -> Code Generation -> Runtime Library and change the Runtime Library to MultiThreaded 

      
      
For all of the #include libraries used, copy into the Header files

## Using Windows Console Application

Do all of the steps as Empty Console Application

Go to ProjectName -> C/C++ -> Precompiled Headers and change the header to the protoc created header

Then go to the pch.cpp files and pch.h files and change accordingly


