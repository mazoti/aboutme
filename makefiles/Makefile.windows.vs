#==================================================== Configuration ====================================================

# Uncomment to enable features or set via command line (e.g., nmake ENABLE_NETWORK=1)
#TRANSLATION = portuguese

ENABLE_ENVIRONMENT_VARIABLES =
ENABLE_SYSTEM_CLOCK =

ENABLE_BATTERY =
ENABLE_BLUETOOTH =
ENABLE_CPU =
ENABLE_FEATURES =
ENABLE_GPU =
ENABLE_HARD_DISKS =
ENABLE_INSTALLED =
ENABLE_MEMORY =
ENABLE_NETWORK =
ENABLE_OPERATING_SYSTEM =
ENABLE_PROCESSES =
ENABLE_RESTORE =
ENABLE_SERVICES =
ENABLE_SCHEDULED_TASKS =
ENABLE_SHARED =
ENABLE_STARTUP =
ENABLE_TRASH =
ENABLE_USB =
ENABLE_WIFI =

#============================================== Compiler and linker flags ==============================================

CXXFLAGS = /EHsc /std:c++latest /W4 /D_WINDOWS

# Debug vs. Release builds
!IFDEF DEBUG
CXXFLAGS = $(CXXFLAGS) /Zi /MDd
LINKERFLAGS = /DEBUG
!ELSE
CXXFLAGS = $(CXXFLAGS) /DNDEBUG /O2 /GL /Gy /Oi /Oy
LINKERFLAGS = /LTCG /RELEASE
!ENDIF

CC = cl
LINKER = link
TARGET = releases\aboutme.exe
OBJ_DIR = objs

# Libraries commonly needed for Windows APIs (adjust as needed)
# kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib ole32.lib
# oleaut32.lib wbemuuid.lib ws2_32.lib iphlpapi.lib
LIBS = 

#==================================================== Common section ===================================================

COMMON_OBJS =

!IFDEF ENABLE_ENVIRONMENT_VARIABLES
COMMON_OBJS = $(COMMON_OBJS) $(OBJ_DIR)\environment_variables.obj
CXXFLAGS    = $(CXXFLAGS) /DENABLE_ENVIRONMENT_VARIABLES
!ENDIF

!IFDEF ENABLE_SYSTEM_CLOCK
COMMON_OBJS = $(COMMON_OBJS) $(OBJ_DIR)\system_clock.obj
CXXFLAGS    = $(CXXFLAGS) /DENABLE_SYSTEM_CLOCK
!ENDIF

#===================================================== Core section ====================================================

CORE_OBJS =

!IFDEF ENABLE_BATTERY
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\battery.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_BATTERY
!ENDIF

!IFDEF ENABLE_BLUETOOTH
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\bluetooth.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_BLUETOOTH
!ENDIF

!IFDEF ENABLE_CPU
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\cpu.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_CPU
!ENDIF

!IFDEF ENABLE_FEATURES
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\features.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_FEATURES
!ENDIF

!IFDEF ENABLE_GPU
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\gpu.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_GPU
!ENDIF

!IFDEF ENABLE_HARD_DISKS
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\hard_disks.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_HARD_DISKS
!ENDIF

!IFDEF ENABLE_INSTALLED
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\installed.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_INSTALLED
!ENDIF

!IFDEF ENABLE_MEMORY
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\memory.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_MEMORY
!ENDIF

!IFDEF ENABLE_NETWORK
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\network.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_NETWORK
!ENDIF

!IFDEF ENABLE_OPERATING_SYSTEM
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\operating_system.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_OPERATING_SYSTEM
!ENDIF

!IFDEF ENABLE_PROCESSES
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\processes.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_PROCESSES
!ENDIF

!IFDEF ENABLE_RESTORE
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\restore.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_RESTORE
!ENDIF

!IFDEF ENABLE_SERVICES
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\services.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_SERVICES
!ENDIF

!IFDEF ENABLE_SHARED
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\shared.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_SHARED
!ENDIF

!IFDEF ENABLE_STARTUP
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\startup.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_STARTUP
!ENDIF

!IFDEF ENABLE_SCHEDULED_TASKS
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\scheduled_tasks.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_SCHEDULED_TASKS
!ENDIF

!IFDEF ENABLE_TRASH
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\trash.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_TRASH
!ENDIF

!IFDEF ENABLE_USB
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\usb.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_USB
!ENDIF

!IFDEF ENABLE_WIFI
CORE_OBJS = $(CORE_OBJS) $(OBJ_DIR)\wifi.obj
CXXFLAGS  = $(CXXFLAGS) /DENABLE_WIFI
!ENDIF

#===================================================== I18n section ====================================================

!IFNDEF TRANSLATION
TRANSLATION = english
!ENDIF

#===================================================== Link section ====================================================

ALL_OBJS = $(OBJ_DIR)\i18n_system.obj $(OBJ_DIR)\i18n.obj $(OBJ_DIR)\common.obj $(COMMON_OBJS) \
           $(OBJ_DIR)\core.obj $(CORE_OBJS) $(OBJ_DIR)\main.obj $(OBJ_DIR)\main_windows.obj

$(TARGET): $(OBJ_DIR) $(ALL_OBJS)
    $(LINKER) $(ALL_OBJS) resources\aboutme.res $(LIBS) /OUT:$(TARGET) $(LINKERFLAGS)

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

#================================================ Common objects section ================================================

$(OBJ_DIR)\common.obj: src\common\common.cppm
	copy src\common\common.cppm $(OBJ_DIR)\common.ixx
	$(CC) $(CXXFLAGS) /c $(OBJ_DIR)\common.ixx /Fo$(OBJ_DIR)\common.obj

$(OBJ_DIR)\environment_variables.obj: src\common\environment_variables.cpp $(OBJ_DIR)\common.obj $(OBJ_DIR)\i18n_system.obj $(OBJ_DIR)\i18n.obj
	$(CC) $(CXXFLAGS) /c src\common\environment_variables.cpp /Fo$@

$(OBJ_DIR)\system_clock.obj: src\common\system_clock.cpp $(OBJ_DIR)\common.obj $(OBJ_DIR)\i18n_system.obj $(OBJ_DIR)\i18n.obj
	$(CC) $(CXXFLAGS) /c src\common\system_clock.cpp /Fo$@

#================================================ Core objects section =================================================

$(OBJ_DIR)\core.obj: src\core\core.cppm
	copy src\core\core.cppm $(OBJ_DIR)\core.ixx
	$(CC) $(CXXFLAGS) /c $(OBJ_DIR)\core.ixx /Fo$@

$(OBJ_DIR)\battery.obj: src\core\windows\battery.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\battery.cpp /Fo$@

$(OBJ_DIR)\bluetooth.obj: src\core\windows\bluetooth.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\bluetooth.cpp /Fo$@

$(OBJ_DIR)\cpu.obj: src\core\windows\cpu.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\cpu.cpp /Fo$@

$(OBJ_DIR)\features.obj: src\core\windows\features.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\features.cpp /Fo$@

$(OBJ_DIR)\gpu.obj: src\core\windows\gpu.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\gpu.cpp /Fo$@

$(OBJ_DIR)\hard_disks.obj: src\core\windows\hard_disks.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\hard_disks.cpp /Fo$@

$(OBJ_DIR)\installed.obj: src\core\windows\installed.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\installed.cpp /Fo$@

$(OBJ_DIR)\memory.obj: src\core\windows\memory.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\memory.cpp /Fo$@

$(OBJ_DIR)\network.obj: src\core\windows\network.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\network.cpp /Fo$@

$(OBJ_DIR)\operating_system.obj: src\core\windows\operating_system.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\operating_system.cpp /Fo$@

$(OBJ_DIR)\processes.obj: src\core\windows\processes.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\processes.cpp /Fo$@

$(OBJ_DIR)\restore.obj: src\core\windows\restore.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\restore.cpp /Fo$@

$(OBJ_DIR)\services.obj: src\core\windows\services.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\services.cpp /Fo$@

$(OBJ_DIR)\shared.obj: src\core\windows\shared.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\shared.cpp /Fo$@

$(OBJ_DIR)\startup.obj: src\core\windows\startup.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\startup.cpp /Fo$@

$(OBJ_DIR)\scheduled_tasks.obj: src\core\windows\scheduled_tasks.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\scheduled_tasks.cpp /Fo$@

$(OBJ_DIR)\trash.obj: src\core\windows\trash.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\trash.cpp /Fo$@

$(OBJ_DIR)\usb.obj: src\core\windows\usb.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\usb.cpp /Fo$@

$(OBJ_DIR)\wifi.obj: src\core\windows\wifi.cpp $(OBJ_DIR)\core.obj
	$(CC) $(CXXFLAGS) /c src\core\windows\wifi.cpp /Fo$@

#================================================= I18n objects section ================================================

$(OBJ_DIR)\i18n.obj: src\i18n\$(TRANSLATION)\all.cppm
	copy src\i18n\$(TRANSLATION)\all.cppm $(OBJ_DIR)\i18n.ixx
	$(CC) $(CXXFLAGS) /c $(OBJ_DIR)\i18n.ixx /Fo$@

$(OBJ_DIR)\i18n_system.obj: src\i18n\$(TRANSLATION)\windows.cppm
	copy src\i18n\$(TRANSLATION)\windows.cppm $(OBJ_DIR)\i18n_system.ixx
	$(CC) $(CXXFLAGS) /c $(OBJ_DIR)\i18n_system.ixx /Fo$@

#================================================= Main object section =================================================

$(OBJ_DIR)\main.obj: src\core\main.cpp $(OBJ_DIR)\i18n_system.obj $(OBJ_DIR)\i18n.obj $(OBJ_DIR)\common.obj $(COMMON_OBJS) $(OBJ_DIR)\core.obj $(CORE_OBJS)
	$(CC) $(CXXFLAGS) /c src\core\main.cpp /Fo$@

$(OBJ_DIR)\main_windows.obj: src\main_windows.cpp $(OBJ_DIR)\main.obj
	$(CC) $(CXXFLAGS) /c src\main_windows.cpp /Fo$@

#==================================================== Clean section ====================================================

clean:
	if exist $(TARGET) del $(TARGET)
	if exist $(OBJ_DIR) rd /S /Q $(OBJ_DIR)
	if exist *.ifc del *.ifc
	if exist *.pdb del *.pdb
	if exist releases\aboutme.pdb del releases\aboutme.pdb
	if exist releases\aboutme.ilk del releases\aboutme.ilk

.PHONY: clean
