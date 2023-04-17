# Makefile pour compiler la HAL

##############################################
#################### HAL #####################
##############################################

HAL_CFLAGS    = $(GENERIC_CFLAGS)
HAL_INCFLAGS  = -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_INCFLAGS += -I$(HAL_TOLOSAT_INCDIR)
HAL_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# HAL compilation
$(HAL_OBJDIR)/%.o : $(HAL_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(GENERIC_CFLAGS) $(HAL_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

$(HAL_LIB) : $(HAL_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "******************************"
	@echo "*****   HAL Build Done   *****"
	@echo "******************************"
	@echo

libhal : $(HAL_LIB)

##############################################
################ HAL TOLOSAT #################
##############################################

HAL_TOLOSAT_CFLAGS    = $(GENERIC_CFLAGS)
HAL_TOLOSAT_INCFLAGS  = -I$(HAL_TOLOSAT_INCDIR)
HAL_TOLOSAT_INCFLAGS += -I$(HAL_INCDIR) -I$(HAL_INCDIR)/Legacy 
HAL_TOLOSAT_INCFLAGS += -I$(CMSIS_INCDIR) -I$(CMSIS_INCDIR_DEVICE) -D $(CHIP)

# HAL compilation
$(HAL_TOLOSAT_OBJDIR)/%.o : $(HAL_TOLOSAT_SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(GENERIC_CFLAGS) $(HAL_TOLOSAT_INCFLAGS) $(GENERIC_DBGCFLAGS) $^ -o $@ 

$(HAL_TOLOSAT_LIB) : $(HAL_TOLOSAT_OBJS)
	mkdir -p $(@D)
	$(AR) rcs $@ $^
	@echo "********************************"
	@echo "**   HAL TOLOSAT Build Done   **"
	@echo "********************************"
	@echo

libhal-tolosat : $(HAL_TOLOSAT_LIB)