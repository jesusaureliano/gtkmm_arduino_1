##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=animate_1
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo
ProjectPath            :=C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/animate_1
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=aurel
Date                   :=08/12/2022
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=C:/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/msys64/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/msys64/mingw64/bin/windres.exe
LinkOptions            :=  $(shell pkg-config --libs gtkmm-4.0)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)wsock32 $(LibrarySwitch)boost_system-mt $(LibrarySwitch)boost_thread-mt $(LibrarySwitch)boost_chrono-mt 
ArLibs                 :=  "libwsock32" "boost_system-mt" "boost_thread-mt" "boost_chrono-mt" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/msys64/mingw64/bin/g++.exe
CC       := C:/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -std=c++17 -Wall $(shell pkg-config --cflags gtkmm-4.0) -pthread $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/parametrosdlg.cpp$(ObjectSuffix) $(IntermediateDirectory)/traza.cpp$(ObjectSuffix) $(IntermediateDirectory)/monitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/plot.cpp$(ObjectSuffix) $(IntermediateDirectory)/axes.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/areadibujar.cpp$(ObjectSuffix) $(IntermediateDirectory)/mainwindow.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/parametrosdlg.cpp$(ObjectSuffix): parametrosdlg.cpp $(IntermediateDirectory)/parametrosdlg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/parametrosdlg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/parametrosdlg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/parametrosdlg.cpp$(DependSuffix): parametrosdlg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/parametrosdlg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/parametrosdlg.cpp$(DependSuffix) -MM parametrosdlg.cpp

$(IntermediateDirectory)/parametrosdlg.cpp$(PreprocessSuffix): parametrosdlg.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/parametrosdlg.cpp$(PreprocessSuffix) parametrosdlg.cpp

$(IntermediateDirectory)/traza.cpp$(ObjectSuffix): traza.cpp $(IntermediateDirectory)/traza.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/traza.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/traza.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/traza.cpp$(DependSuffix): traza.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/traza.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/traza.cpp$(DependSuffix) -MM traza.cpp

$(IntermediateDirectory)/traza.cpp$(PreprocessSuffix): traza.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/traza.cpp$(PreprocessSuffix) traza.cpp

$(IntermediateDirectory)/monitor.cpp$(ObjectSuffix): monitor.cpp $(IntermediateDirectory)/monitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/monitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/monitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/monitor.cpp$(DependSuffix): monitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/monitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/monitor.cpp$(DependSuffix) -MM monitor.cpp

$(IntermediateDirectory)/monitor.cpp$(PreprocessSuffix): monitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/monitor.cpp$(PreprocessSuffix) monitor.cpp

$(IntermediateDirectory)/plot.cpp$(ObjectSuffix): plot.cpp $(IntermediateDirectory)/plot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/plot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/plot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/plot.cpp$(DependSuffix): plot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/plot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/plot.cpp$(DependSuffix) -MM plot.cpp

$(IntermediateDirectory)/plot.cpp$(PreprocessSuffix): plot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/plot.cpp$(PreprocessSuffix) plot.cpp

$(IntermediateDirectory)/axes.cpp$(ObjectSuffix): axes.cpp $(IntermediateDirectory)/axes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/axes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/axes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/axes.cpp$(DependSuffix): axes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/axes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/axes.cpp$(DependSuffix) -MM axes.cpp

$(IntermediateDirectory)/axes.cpp$(PreprocessSuffix): axes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/axes.cpp$(PreprocessSuffix) axes.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/areadibujar.cpp$(ObjectSuffix): areadibujar.cpp $(IntermediateDirectory)/areadibujar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/areadibujar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/areadibujar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/areadibujar.cpp$(DependSuffix): areadibujar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/areadibujar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/areadibujar.cpp$(DependSuffix) -MM areadibujar.cpp

$(IntermediateDirectory)/areadibujar.cpp$(PreprocessSuffix): areadibujar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/areadibujar.cpp$(PreprocessSuffix) areadibujar.cpp

$(IntermediateDirectory)/mainwindow.cpp$(ObjectSuffix): mainwindow.cpp $(IntermediateDirectory)/mainwindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/aurel/Documents/ProjectosC++/CodeLite/Gtkmm4-Cairo/animate_1/mainwindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mainwindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mainwindow.cpp$(DependSuffix): mainwindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mainwindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mainwindow.cpp$(DependSuffix) -MM mainwindow.cpp

$(IntermediateDirectory)/mainwindow.cpp$(PreprocessSuffix): mainwindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mainwindow.cpp$(PreprocessSuffix) mainwindow.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


