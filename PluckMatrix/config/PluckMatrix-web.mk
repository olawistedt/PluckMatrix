# IPLUG2_ROOT should point to the top level IPLUG2 folder from the project folder
# By default, that is three directories up from /Examples/PluckMatrix/config
IPLUG2_ROOT = ../../iPlug2
include ../../iPlug2/common-web.mk

SRC += $(PROJECT_ROOT)/PluckMatrix.cpp
SRC += $(PROJECT_ROOT)/Machine.cpp
SRC += $(PROJECT_ROOT)/Oscillator.cpp
SRC += $(PROJECT_ROOT)/WitechControls.cpp
SRC += $(PROJECT_ROOT)/Patterns.cpp

WAM_SRC += $(IPLUG_EXTRAS_PATH)/Synth/*.cpp

WAM_CFLAGS +=  -I$(IPLUG_SYNTH_PATH)

WEB_CFLAGS += -DIGRAPHICS_NANOVG -DIGRAPHICS_GLES2

WAM_LDFLAGS += -O3 -s EXPORT_NAME="'ModuleFactory'" -s ASSERTIONS=0

WEB_LDFLAGS += -O3 -s ASSERTIONS=0

WEB_LDFLAGS += $(NANOVG_LDFLAGS)
