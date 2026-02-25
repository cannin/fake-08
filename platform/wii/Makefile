#-------------------------------------------------------------------------------
.SUFFIXES:
#-------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#-------------------------------------------------------------------------------
# pkg-config for libraries flags
#-------------------------------------------------------------------------------
PKGCONF		:=	$(DEVKITPRO)/portlibs/wii/bin/powerpc-eabi-pkg-config

#-------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
#-------------------------------------------------------------------------------
TARGET		:=	FAKE-08
BUILD		:=	build

SOURCES   = ../../source ../../libs/z8lua ../../libs/utf8-util ../../libs/lodepng ../../libs/simpleini ../../libs/miniz
INCLUDES  = ../../include ../../libs/z8lua ../../libs/utf8-util ../../libs/lodepng ../../libs/simpleini ../../libs/miniz

SOURCES		:=	$(SOURCES) source
INCLUDES	:=	$(INCLUDES)
LIBRARIES	:=	SDL2_gfx SDL2_image SDL2_mixer SDL2_ttf sdl2 \
			freetype2 zlib libpng libjpeg

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
CC = $(CXX)

CFLAGS		:=	-g -Wall -O2 -ffunction-sections -MMD -MP -DVER_STR=\"$(APP_VERSION)\" \
			-ffast-math \
			$(MACHDEP)

CFLAGS		+=	$(INCLUDE) -D__WII__ -DGEKKO

CFLAGS		+=	`$(PKGCONF) --cflags $(LIBRARIES)`

CXXFLAGS	:=	$(CFLAGS) -std=gnu++17

ASFLAGS		:=	-g $(MACHDEP)
LDFLAGS		:=	-g $(MACHDEP) -Wl,-Map,$(notdir $*.map)

LIBS		:=	`$(PKGCONF) --libs $(LIBRARIES)`
LIBS		+= -logc -lm

#-------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level
# containing include and lib
#-------------------------------------------------------------------------------
LIBDIRS		:=	$(PORTLIBS)


#-------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#-------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#-------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

#-------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#-------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#-------------------------------------------------------------------------------
	export LD	:=	$(CC)
#-------------------------------------------------------------------------------
else
#-------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------

export SRCFILES		:=	$(CPPFILES) $(CFILES) $(SFILES)
export OFILES		:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
				$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
				-I$(CURDIR)/$(BUILD)

export LIBPATHS		:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean all

#-------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD): $(SRCFILES)
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#-------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).dol $(TARGET).elf

#-------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS		:=	$(OFILES:.o=.d)

#-------------------------------------------------------------------------------
# main targets
#-------------------------------------------------------------------------------
all		:	$(OUTPUT).dol

$(OUTPUT).dol	:	$(OUTPUT).elf
	@elf2dol $(OUTPUT).elf $(OUTPUT).dol

$(OUTPUT).elf	:	$(OFILES)
	@echo linking $(OUTPUT).elf
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $(OUTPUT).elf

-include $(DEPENDS)

#-------------------------------------------------------------------------------
endif
#-------------------------------------------------------------------------------
