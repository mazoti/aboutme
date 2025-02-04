#==================================================== Configuration ====================================================

#TRANSLATION = portuguese

#ENABLE_ENVIRONMENT_VARIABLES =
ENABLE_SYSTEM_CLOCK =

ENABLE_BATTERY =
ENABLE_BLUETOOTH =
ENABLE_CPU =
ENABLE_FEATURES =
ENABLE_GPU =
ENABLE_HARD_DISKS =
ENABLE_INSTALLED =
ENABLE_MEMORY =
#ENABLE_NETWORK =
ENABLE_OPERATING_SYSTEM =
ENABLE_PROCESSES =
ENABLE_RESTORE =
ENABLE_SERVICES =
ENABLE_STARTUP =
ENABLE_TASKS =
ENABLE_USB =
ENABLE_WIFI =

#============================================== Compiler and linker flags ==============================================

CXXFLAGS = /EHsc /std:c++latest /W4 /D_WINDOWS

# Debug version builds faster and checks for memory leaks
!IFDEF DEBUG
CXXFLAGS = $(CXXFLAGS) /Zi /MDd
!ELSE
CXXFLAGS = $(CXXFLAGS) /DNDEBUG /O2 /GL /Gy /Oi /Oy
!ENDIF

CC = cl
LINKER = link
LINKERFLAGS = /LTCG /RELEASE
TARGET = releases\aboutme.exe

#==================================================== Common section ===================================================

COMMON_OBJS =

!IFDEF ENABLE_ENVIRONMENT_VARIABLES
COMMON_OBJS = $(COMMON_OBJS) objs\environment_variables.obj
CXXFLAGS    = $(CXXFLAGS) /DENABLE_ENVIRONMENT_VARIABLES
!ENDIF

!IFDEF ENABLE_SYSTEM_CLOCK
COMMON_OBJS = $(COMMON_OBJS) objs\system_clock.obj
CXXFLAGS    = $(CXXFLAGS) /DENABLE_SYSTEM_CLOCK
!ENDIF

#===================================================== Core section ====================================================

CORE_OBJS =

!IFDEF ENABLE_BATTERY
CORE_OBJS = $(CORE_OBJS) objs\battery.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_BATTERY
!ENDIF

!IFDEF ENABLE_BLUETOOTH
CORE_OBJS = $(CORE_OBJS) objs\bluetooth.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_BLUETOOTH
!ENDIF

!IFDEF ENABLE_CPU
CORE_OBJS = $(CORE_OBJS) objs\cpu.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_CPU
!ENDIF

!IFDEF ENABLE_FEATURES
CORE_OBJS = $(CORE_OBJS) objs\features.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_FEATURES
!ENDIF

!IFDEF ENABLE_GPU
CORE_OBJS = $(CORE_OBJS) objs\gpu.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_GPU
!ENDIF

!IFDEF ENABLE_HARD_DISKS
CORE_OBJS = $(CORE_OBJS) objs\hard_disks.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_HARD_DISKS
!ENDIF

!IFDEF ENABLE_INSTALLED
CORE_OBJS = $(CORE_OBJS) objs\installed.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_INSTALLED
!ENDIF

!IFDEF ENABLE_MEMORY
CORE_OBJS = $(CORE_OBJS) objs\memory.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_MEMORY
!ENDIF

!IFDEF ENABLE_NETWORK
CORE_OBJS = $(CORE_OBJS) objs\network.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_NETWORK
!ENDIF

!IFDEF ENABLE_OPERATING_SYSTEM
CORE_OBJS = $(CORE_OBJS) objs\operating_system.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_OPERATING_SYSTEM
!ENDIF

!IFDEF ENABLE_PROCESSES
CORE_OBJS = $(CORE_OBJS) objs\processes.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_PROCESSES
!ENDIF

!IFDEF ENABLE_RESTORE
CORE_OBJS = $(CORE_OBJS) objs\restore.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_RESTORE
!ENDIF

!IFDEF ENABLE_SERVICES
CORE_OBJS = $(CORE_OBJS) objs\services.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_SERVICES
!ENDIF

!IFDEF ENABLE_STARTUP
CORE_OBJS = $(CORE_OBJS) objs\startup.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_STARTUP
!ENDIF

!IFDEF ENABLE_TASKS
CORE_OBJS = $(CORE_OBJS) objs\tasks.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_TASKS
!ENDIF

!IFDEF ENABLE_USB
CORE_OBJS = $(CORE_OBJS) objs\usb.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_USB
!ENDIF

!IFDEF ENABLE_WIFI
CORE_OBJS = $(CORE_OBJS) objs\wifi.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_WIFI
!ENDIF

#===================================================== I18n section ====================================================

!IFNDEF TRANSLATION
TRANSLATION = english
!ENDIF

#===================================================== Link section ====================================================

$(TARGET): objs objs\main_windows.obj
	$(LINKER) $(LINKERFLAGS)  \
		objs\i18n_system.obj  \
		objs\i18n.obj         \
		objs\common.obj       \
		$(COMMON_OBJS)        \
		objs\core.obj         \
		$(CORE_OBJS)          \
		objs\main.obj         \
		objs\main_windows.obj \
		resources\aboutme.res \
		/OUT:$(TARGET)

objs:
	if not exist objs mkdir objs

#================================================ Common objects section ================================================

objs\common.obj: src\common\common.cppm
	copy src\common\common.cppm objs\common.ixx
	$(CC) $(CXXFLAGS) /c objs\common.ixx /Foobjs\common.obj

objs\environment_variables.obj: objs\common.obj objs\i18n_system.obj objs\i18n.obj
	$(CC) $(CXXFLAGS) /c src\common\environment_variables.cpp /Foobjs\environment_variables.obj

objs\system_clock.obj: objs\common.obj objs\i18n_system.obj objs\i18n.obj
	$(CC) $(CXXFLAGS) /c src\common\system_clock.cpp /Foobjs\system_clock.obj

#================================================ Core objects section =================================================

objs\battery.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\battery.cpp /Foobjs\battery.obj

objs\bluetooth.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\bluetooth.cpp /Foobjs\bluetooth.obj

objs\cpu.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\cpu.cpp /Foobjs\cpu.obj

objs\features.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\features.cpp /Foobjs\features.obj

objs\gpu.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\gpu.cpp /Foobjs\gpu.obj

objs\hard_disks.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\hard_disks.cpp /Foobjs\hard_disks.obj

objs\installed.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\installed.cpp /Foobjs\installed.obj

objs\memory.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\memory.cpp /Foobjs\memory.obj

objs\network.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\network.cpp /Foobjs\network.obj

objs\operating_system.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\operating_system.cpp /Foobjs\operating_system.obj

objs\processes.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\processes.cpp /Foobjs\processes.obj

objs\restore.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\restore.cpp /Foobjs\restore.obj

objs\services.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\services.cpp /Foobjs\services.obj

objs\startup.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\startup.cpp /Foobjs\startup.obj

objs\tasks.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\tasks.cpp /Foobjs\tasks.obj

objs\usb.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\usb.cpp /Foobjs\usb.obj

objs\wifi.obj: objs\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\wifi.cpp /Foobjs\wifi.obj

objs\core.obj:
	copy src\core\core.cppm objs\core.ixx
	$(CC) $(CXXFLAGS) /c objs\core.ixx /Foobjs\core.obj

#================================================= I18n objects section ================================================

objs\i18n.obj:
	copy src\i18n\$(TRANSLATION)\all.cppm objs\i18n.ixx
	$(CC) $(CXXFLAGS) /c objs\i18n.ixx /Foobjs\i18n.obj

objs\i18n_system.obj:
	copy src\i18n\$(TRANSLATION)\windows.cppm objs\i18n_system.ixx
	$(CC) $(CXXFLAGS) /c objs\i18n_system.ixx /Foobjs\i18n_system.obj

#================================================= Main object section =================================================

objs\main.obj: objs\i18n_system.obj objs\i18n.obj objs\common.obj $(COMMON_OBJS) objs\core.obj $(CORE_OBJS)
	$(CC) $(CXXFLAGS) /c src\core\main.cpp /Foobjs\main.obj

objs\main_windows.obj: objs\main.obj
	$(CC) $(CXXFLAGS) /c src\main_windows.cpp /Foobjs\main_windows.obj

#==================================================== Clean section ====================================================

clean:
	if exist $(TARGET) del $(TARGET)
	if exist objs rd /S /Q objs
	if exist *.ifc del *.ifc
	if exist *.pdb del *.pdb
	if exist release\aboutme.pdb del release\aboutme.pdb
	if exist release\aboutme.ilk del release\aboutme.ilk

.PHONY: clean
