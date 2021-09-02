##########################################
# SET THESE 6 PATHS CORRECTLY TO COMPILE #
##########################################
BOOST_INC=
BOOST_LIB=
RMATH_INC=
RMATH_LIB=
HTSLD_INC=
HTSLD_LIB=
#########################################################
# EXAMPLES:                                             #
# BOOST_INC=/usr/include/                               #
# BOOST_LIB=/usr/lib/x86_64-linux-gnu/                  #
# RMATH_INC=$(HOME)/Tools/R-3.6.1/src/include           #
# RMATH_LIB=$(HOME)/Tools/R-3.6.1/src/nmath/standalone  #
# HTSLD_INC=$(HOME)/Tools/htslib-1.9                    #
# HTSLD_LIB=$(HOME)/Tools/htslib-1.9                    #
#########################################################
define n


endef

#INSTALL LOCATIONS
#CHANGE prefix TO INSTALL LOCALLY
prefix      = /usr/local
exec_prefix = $(prefix)
bindir      = $(exec_prefix)/bin
datarootdir = $(prefix)/share
mandir      = $(datarootdir)/man
man1dir     = $(mandir)/man1
autocompdir = /etc/bash_completion.d
MKDIR_P = mkdir -p
INSTALL = install -p
INSTALL_DATA    = $(INSTALL) -m 644
INSTALL_EXE    = $(INSTALL) -m 755
INSTALL_DIR     = $(MKDIR_P) -m 755
INSTALL_MAN     = $(INSTALL_DATA)
INSTALL_PROGRAM = $(INSTALL)
INSTALL_SCRIPT  = $(INSTALL_EXE)

#COMPILER MODE C++11
CXX=g++ -std=c++0x

#COMPILER FLAGS
CXXFLAG_REL=-O3
CXXFLAG_DBG=-g
CXXFLAG_WRN=-Wall -Wextra -Wno-sign-compare -Wno-unused-local-typedefs -Wno-deprecated -Wno-unused-parameter

#BASE LIBRARIES
LIB_FLAGS=-lz -lgsl -lbz2 -llzma -lgslcblas -lm -lpthread -lcurl

#FILE LISTS
BFILE=bin/bedntools
SFILE=$(shell find scripts -name *.R)
MFILE=$(shell find doc -name *.1)
HFILE=$(shell find src -name *.h)
TFILE=$(shell find lib -name *.h)
CFILE=$(shell find src -name *.cpp | LC_ALL=C sort)
OFILE=$(shell for file in `find src -name *.cpp | LC_ALL=C sort`; do echo obj/$$(basename $$file .cpp).o; done)
VPATH=$(shell for file in `find src -name *.cpp | LC_ALL=C sort`; do echo $$(dirname $$file); done)
ifneq (, $(shell which git))
 GITVS=$(shell git describe --tags --long --abbrev=10 2>/dev/null)
 ifneq (, $(GITVS))
  $(info Compiling version $(GITVS))
  CXXFLAG_REL+= -DQTLTOOLS_VERSION=\"$(GITVS)\"
  CXXFLAG_DBG+= -DQTLTOOLS_VERSION=\"$(GITVS)\"
 endif
endif

#STATICLY LINKED LIBS
LIB_FILES=$(RMATH_LIB)/libRmath.a $(HTSLD_LIB)/libhts.a $(BOOST_LIB)/libboost_iostreams.a $(BOOST_LIB)/libboost_program_options.a
#INCLUDE DIRS
IFLAG=-Ilib/OTools -isystem lib -I$(RMATH_INC) -I$(HTSLD_INC) -I$(BOOST_INC)

#ONLY FOR MAC STATIC LINKING, ARCHIVES ASSUMED TO BE INSTALLED WITH BREW.IF YOU HAVE THESE IN OTHER LOCATIONS MODIFY THE NEXT 5 LINES 
MZ=/usr/local/opt/zlib/lib/libz.a
MCBLAS=/usr/local/lib/libgslcblas.a
MGSL=/usr/local/lib/libgsl.a
MBZ2=/usr/local/opt/bzip2/lib/libbz2.a
MLZMA=/usr/local/lib/liblzma.a
MCURL=/usr/local/opt/curl/lib/libcurl.a

#MAC SPECIFIC STUFF
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
 CXXFLAG_REL+= -fvisibility=hidden -fvisibility-inlines-hidden
 CXXFLAG_DBG+= -fvisibility=hidden -fvisibility-inlines-hidden
 autocompdir = /usr/local/etc/bash_completion.d
 ifeq ($(MAKECMDGOALS),static)
   ifeq ("$(wildcard  $(MZ))","")
     $(error Cannot find $(MZ)! $nTry:	brew install zlib$nOr edit the makefile with the correct location of libz.a)
   endif
   ifeq ("$(wildcard  $(MCBLAS))","")
     $(error Cannot find $(MCBLAS)! $nTry:	brew install gsl$nOr edit the makefile with the correct location of libgslcblas.a)
   endif
   ifeq ("$(wildcard  $(MGSL))","")
     $(error Cannot find $(MGSL)! $nTry:	brew install gsl$nOr edit the makefile with the correct location of libgsl.a)
   endif
   ifeq ("$(wildcard  $(MBZ2))","")
     $(error Cannot find $(MBZ2)! $nTry:	brew install bzip2$nOr edit the makefile with the correct location of libbz2.a)
   endif
   ifeq ("$(wildcard  $(MLZMA))","")
     $(error Cannot find $(MLZMA)! $nTry:	brew install xz$nOr edit the makefile with the correct location of liblzma.a)
   endif
   ifeq ("$(wildcard  $(MCURL))","")
     $(error Cannot find $(MCURL)! $nTry:	brew install curl$nOr edit the makefile with the correct location of libcurl.a)
   endif           
 endif
endif

#CHECK IF WE MADE BEFORE INSTALLING
ifeq ($(MAKECMDGOALS),install)
 ifeq ("$(wildcard $(BFILE))","")
   $(error Cannot find $(BFILE)! $nPlease make before you make install)
 endif
endif

#DEFAULT VERSION (SET UP THE VARIABLES IN THE BEGINING OF THE MAKEFILE)
all: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN)
all: LDFLAG=$(CXXFLAG_REL)
all: $(BFILE)

#DEBUG VERSION (SET UP THE VARIABLES IN THE BEGINING OF THE MAKEFILE)
debug: CXXFLAG=$(CXXFLAG_DBG) $(CXXFLAG_WRN)
debug: LDFLAG=$(CXXFLAG_DBG)
debug: $(BFILE)

#STATIC VERSION (SET UP THE VARIABLES IN THE BEGINING OF THE MAKEFILE)
static: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN)
static: LDFLAG=$(CXXFLAG_REL)
ifeq ($(UNAME),Darwin)
#ASSUMES YOU INSTALLED REQUIRED LIBRARIES WITH BREW. SEE ABOVE WHERE THESE VARIABLES ARE SET
static: LIB_FILES+= $(MZ) $(MCBLAS) $(MGSL) $(MBZ2) $(MLZMA) $(MCURL)
static: LIB_FLAGS=-lm -lpthread
else
static: LIB_FLAGS=-Wl,-Bstatic -lz -lgsl -lbz2 -llzma -lgslcblas -lcurl -Wl,-Bdynamic -lm -lpthread
endif
static: $(BFILE)

#STATIC DEBUG VERSION (SET UP THE VARIABLES IN THE BEGINING OF THE MAKEFILE)
static-dbg: CXXFLAG=$(CXXFLAG_DBG) $(CXXFLAG_WRN)
static-dbg: LDFLAG=$(CXXFLAG_DBG)
ifeq ($(UNAME),Darwin)
#ASSUMES YOU INSTALLED REQUIRED LIBRARIES WITH BREW, SEE ABOVE WHERE THESE VARIABLES ARE SET
static-dbg: LIB_FILES+= $(MZ) $(MCBLAS) $(MGSL) $(MBZ2) $(MLZMA) $(MCURL)
static-dbg: LIB_FLAGS=-lm -lpthread
else
static-dbg: LIB_FLAGS=-Wl,-Bstatic -lz -lgsl -lbz2 -llzma -lgslcblas -lcurl -Wl,-Bdynamic -lm -lpthread
endif
static-dbg: $(BFILE)

personal: BOOST_INC=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/boost_1_74_0/
personal: BOOST_LIB=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/boost_1_74_0/stage/lib
personal: RMATH_INC=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/R-3.6.1/src/include
personal: RMATH_LIB=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/R-3.6.1/src/nmath/standalone
personal: HTSLD_INC=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/htslib-1.11
personal: HTSLD_LIB=/Users/nikolaoslykoskoufis/Documents/Programming/Tools/htslib-1.11
personal: static

personal-dbg: BOOST_INC=/usr/include/
personal-dbg: BOOST_LIB=/usr/lib/x86_64-linux-gnu/
personal-dbg: RMATH_INC=$(HOME)/Tools/R-3.6.1/src/include
personal-dbg: RMATH_LIB=$(HOME)/Tools/R-3.6.1/src/nmath/standalone
personal-dbg: HTSLD_INC=$(HOME)/Tools/htslib-1.9
personal-dbg: HTSLD_LIB=$(HOME)/Tools/htslib-1.9
personal-dbg: debug

yggdrasil: BOOST_INC=/home/users/l/lykoskou/Tools/boost_1_71_0/
yggdrasil: BOOST_LIB=/home/users/l/lykoskou/Tools/boost_1_71_0/stage/lib/
yggdrasil: RMATH_INC=/home/users/l/lykoskou/Tools/R-3.6.1/src/include/
yggdrasil: RMATH_LIB=/home/users/l/lykoskou/Tools/R-3.6.1/src/nmath/standalone/
yggdrasil: HTSLD_INC=/home/users/l/lykoskou/Tools/htslib-1.9/
yggdrasil: HTSLD_LIB=/home/users/l/lykoskou/Tools/htslib-1.9/
yggdrasil: static

baobab: BOOST_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/boost_1_71_0/
baobab: BOOST_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/boost_1_71_0/stage/lib/
baobab: RMATH_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/R-3.6.1/src/include/
baobab: RMATH_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/R-3.6.1/src/nmath/standalone/
baobab: HTSLD_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/htslib-1.9/
baobab: HTSLD_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/htslib-1.9/
baobab: static

baobab-dbg: BOOST_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/boost_1_71_0/
baobab-dbg: BOOST_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/boost_1_71_0/stage/lib/
baobab-dbg: RMATH_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/R-3.6.1/src/include/
baobab-dbg: RMATH_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/R-3.6.1/src/nmath/standalone/
baobab-dbg: HTSLD_INC=/srv/beegfs/scratch/groups/funpopgen/Tools/htslib-1.9/
baobab-dbg: HTSLD_LIB=/srv/beegfs/scratch/groups/funpopgen/Tools/htslib-1.9/
baobab-dbg: static-dbg

hbb: BOOST_INC=/hbb/include
hbb: BOOST_LIB=/hbb/lib
hbb: RMATH_INC=/hbb/include
hbb: RMATH_LIB=/hbb/lib64
hbb: HTSLD_INC=/hbb/include
hbb: HTSLD_LIB=/hbb/lib
hbb: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN) -fvisibility=hidden
hbb: LDFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN) -fvisibility=hidden -static-libstdc++ -L/hbb/lib
hbb: LIB_FLAGS=-Wl,-Bstatic -lz -lgsl -lbz2 -llzma -lgslcblas -lcurl -lssl -lcrypto -Wl,-Bdynamic -lm -lpthread -ldl -lrt
hbb: $(BFILE)

epfl: BOOST_INC=/home/lykoskou/Tools/boost_1_71_0
epfl: BOOST_LIB=/home/lykoskou/Tools/boost_1_71_0/stage/lib
epfl: RMATH_INC=/home/lykoskou/Tools/R-3.6.1/src/include
epfl: RMATH_LIB=/home/lykoskou/Tools/R-3.6.1/src/nmath/standalone
epfl: HTSLD_INC=/home/lykoskou/Software/htslib-1.9/include
epfl: HTSLD_LIB=/home/lykoskou/Software/htslib-1.9/lib
epfl: GSL_LIB=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/gsl-2.5-s4457uk5oaqsr52mx5ticuk6oed7cihy/lib
epfl: GSL_INC=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/gsl-2.5-s4457uk5oaqsr52mx5ticuk6oed7cihy/include
epfl: ZLIB_INC=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/zlib-1.2.11-ylrp4rhsafljqpno2gpjiyk2szwaq4by/include
epfl: ZLIB_LIB=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/zlib-1.2.11-ylrp4rhsafljqpno2gpjiyk2szwaq4by/lib
epfl: BZIP_LIB=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/bzip2-1.0.6-mdlqofhyr4gjncpwmlofgybh5cmjadr4/lib
epfl: BZIP_INC=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/bzip2-1.0.6-mdlqofhyr4gjncpwmlofgybh5cmjadr4/include
epfl: XZ_LIB=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/xz-5.2.4-ubniyi5stffuzxiguxwakp77xefd24w3/lib
epfl: XZ_INC=/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/xz-5.2.4-ubniyi5stffuzxiguxwakp77xefd24w3/include
epfl: CURL_INC=/home/lykoskou/Tools/curl-7.78.0/lib/include
epfl: CURL_LIB=/home/lykoskou/Tools/curl-7.78.0/lib
epfl: SSL_INC=/home/lykoskou/Tools/openssl-1.1.1l/include
epfl: SSL_LIB=/home/lykoskou/Tools/openssl-1.1.1l/lib
epfl: CXXFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN) -fvisibility=hidden
epfl: LDFLAG=$(CXXFLAG_REL) $(CXXFLAG_WRN) -fvisibility=hidden -static-libstdc++ -L/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/gsl-2.5-s4457uk5oaqsr52mx5ticuk6oed7cihy/lib/ -L/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/bzip2-1.0.6-mdlqofhyr4gjncpwmlofgybh5cmjadr4/lib/ -L/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/zlib-1.2.11-ylrp4rhsafljqpno2gpjiyk2szwaq4by/lib -L/ssoft/spack/humagne/v1/opt/spack/linux-rhel7-x86_S6g1_Mellanox/gcc-7.4.0/xz-5.2.4-ubniyi5stffuzxiguxwakp77xefd24w3/lib/ -L/home/lykoskou/Tools/openssl-1.1.1l -L/home/lykoskou/Tools/curl-7.78.0/lib/
epfl: LIB_FLAGS=-Wl,-Bstatic -lz -lgsl -lbz2 -llzma -lgslcblas -Wl,-Bdynamic -lm -lpthread
epfl: $(BFILE)



install:
	$(INSTALL_DIR) $(DESTDIR)$(bindir) $(DESTDIR)$(man1dir) $(DESTDIR)$(autocompdir)
	$(INSTALL_PROGRAM) $(BFILE) $(DESTDIR)$(bindir)
	$(INSTALL_SCRIPT) $(SFILE) $(DESTDIR)$(bindir)
	$(INSTALL_MAN) $(MFILE) $(DESTDIR)$(man1dir)
	$(INSTALL_DATA) doc/QTLtools_bash_autocomplete.bash $(DESTDIR)$(autocompdir)

uninstall:
	rm $(DESTDIR)$(bindir)/QTLtools
	rm $(DESTDIR)$(bindir)/qtltools_*.R
	rm $(DESTDIR)$(man1dir)/QTLtools*.1
	rm $(DESTDIR)$(autocompdir)/QTLtools_bash_autocomplete.bash
	rmdir --ignore-fail-on-non-empty -p $(DESTDIR)$(bindir)
	rmdir --ignore-fail-on-non-empty -p $(DESTDIR)$(man1dir)
	rmdir --ignore-fail-on-non-empty -p $(DESTDIR)$(autocompdir)

#COMPILATION RULES
$(BFILE): $(OFILE)
	$(CXX) $^ $(LIB_FILES) -o $@ $(LIB_FLAGS) $(LDFLAG)

obj/BedCorrect.o: src/BedCorrect.cpp $(HFILE) $(TFILE) $(CFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/data.o: src/common/data.cpp src/common/data.h src/common/filter.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/reheader_%o: reheader_%.cpp reheader_data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)

obj/correct_%.o: correct_%.cpp correct_data.h $(TFILE)
	$(CXX) -o $@ -c $< $(CXXFLAG) $(IFLAG)
	
obj/percentage_%.o: percentage_%.cpp percentage_data.h $(TFILE)
	$(CXX) -o $@ -c $< ${CXXFLAG} $(IFLAG)

obj/exclude_%.o: exclude_%.cpp exclude_data.h  $(TFILE)
	$(CXX) -o $@ -c $< ${CXXFLAG} $(IFLAG)

obj/include_%.o: include_%.cpp include_data.h  $(TFILE)
	$(CXX) -o $@ -c $< ${CXXFLAG} $(IFLAG)

obj/vcf2bed_%.o: vcf2bed_%.cpp vcf2bed_data.h $(TFILE)
	$(CXX) -o $@ -c $< ${CXXFLAG} $(IFLAG)

obj/merge_%.o: merge_%.cpp merge_data.h $(TFILE)
	$(CXX) -o $@ -c $< ${CXXFLAG} $(IFLAG)

clean: 
	rm -f obj/*.o $(BFILE)

clean-correct:
	rm -f obj/correct_*.o $(BFILE)
