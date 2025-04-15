
_: cuttletoy.o
	cc \
		-o cuttletoy \
		-Wl,--whole-archive \
		cuttletoy.o \
		-lrevision \
		-L/opt/vc/lib/ \
		-lbrcmGLESv2 \
		-lbrcmEGL \
		-lopenmaxil \
		-lbcm_host \
		-lvcos \
		-lvchiq_arm \
		-lpthread \
		-lrt \
		-lm \
		-llo \
		-L/opt/vc/src/hello_pi/libs/ilclient \
		-L/opt/vc/src/hello_pi/libs/vgfont \
		-L/opt/vc/src/hello_pi/libs/revision \
		-Wl,--no-whole-archive \
		-rdynamic

cuttletoy.o: cuttletoy.c
	cc \
		-DSTANDALONE \
		-D__STDC_CONSTANT_MACROS \
		-D__STDC_LIMIT_MACROS \
		-DTARGET_POSIX \
		-D_LINUX \
		-fPIC \
		-DPIC \
		-D_REENTRANT \
		-D_LARGEFILE64_SOURCE \
		-D_FILE_OFFSET_BITS=64 \
		-U_FORTIFY_SOURCE \
		-Wall \
		-g \
		-DHAVE_LIBOPENMAX=2 \
		-DOMX \
		-DOMX_SKIP64BIT \
		-ftree-vectorize \
		-pipe \
		-DUSE_EXTERNAL_OMX \
		-DHAVE_LIBBCM_HOST \
		-DUSE_EXTERNAL_LIBBCM_HOST \
		-DUSE_VCHIQ_ARM \
		-Wno-psabi \
		-I/opt/vc/include/ \
		-I/opt/vc/include/interface/vcos/pthreads \
		-I/opt/vc/include/interface/vmcs_host/linux \
		-I./ \
		-I/opt/vc/src/hello_pi/libs/ilclient \
		-I/opt/vc/src/hello_pi/libs/vgfont \
		-I/opt/vc/src/hello_pi/libs/revision \
		-g \
		-c cuttletoy.c \
		-o cuttletoy.o \
		-Wno-deprecated-declarations

fake: fake.c
	cc fake.c -llo -lm -o fake
