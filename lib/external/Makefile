include $(NVT_PRJCFG_MODEL_CFG)

EXTLIB = ${LIBRARY_DIR}/external
SHELL = /bin/bash
MULTI_CORES = $(shell grep -c ^processor /proc/cpuinfo)
INSTALL_DIR =${EXTLIB}/__install
MAKEFILE_LIST = libxml2 fuse freetype sqlite3 ffmpeg libnl opencv openssl wolfssl curl zbar

XML2 := libxml2-2.9.3
FUSE := fuse-2.9.4
FREETYPE := freetype-2.3.5
SQLITE3 :=sqlite3-3.10.0
FFMPEG := ffmpeg-3.0
LIBNL := libnl-3.2.27
OPENCV := opencv-3.2.0
OPENSSL := openssl-1.0.2d
WOLFSSL := wolfssl-3.10.0
CURL := curl-7.45.0
ZBAR := zbar-0.10
LIBICONV := libiconv-1.15

unexport CC
unexport CPP
unexport CXX
unexport LD
unexport AR
unexport NM
unexport STRIP
unexport OBJCOPY
unexport OBJDUMP


CC="${CROSS_COMPILE}gcc"
CXX="${CROSS_COMPILE}g++"
AR="${CROSS_COMPILE}ar"
LD="${CROSS_COMPILE}ld"
RANLIB="${CROSS_COMPILE}ranlib"

check_exist = if [ ! -e $(1)/.nvt_finish ]; then rm -rf $(1); tar -jxvf $(1).tar.bz2; fi

.PHONY: $(MAKEFILE_LIST)
all: $(MAKEFILE_LIST)

list:
	@echo $(MAKEFILE_LIST)

init:
	@if [ ! -e "${INSTALL_DIR}" ]; then \
		mkdir -p ${INSTALL_DIR}; \
	fi
	@if [ -z "${CROSS_COMPILE}" ]; then \
		echo "Please execute \"source build/envsetup.sh\" firstly.  Stop."; exit 1; \
	fi

libxml2: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${XML2}/.nvt_finish ]; then \
		$(call check_exist, ${XML2}); \
		cd ${XML2}; ./configure --prefix=${INSTALL_DIR} CC=${CC} CXX=${CXX} AR=${AR} --host=${NVT_HOST} --without-python --without-lzma --target=${NVT_HOST}; make -j${MULTI_CORES}; \
	fi
	@cd ${XML2}; make install; cd -;
	@rm -rf ${INSTALL_DIR}/lib/cmake ${INSTALL_DIR}/lib/xml2Conf.sh ${INSTALL_DIR}/lib/pkgconfig
	@touch ${XML2}/.nvt_finish

fuse: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${FUSE}/.nvt_finish ]; then \
		$(call check_exist, ${FUSE}); \
		cd ${FUSE}; ./configure --host=${NVT_HOST} CC=${CC} LD=${LD} AR=${AR}; \
		make -j${MULTI_CORES}; \
	fi
	cd ${FUSE}; make install DESTDIR=${EXTLIB}/${FUSE}/install; cp -arvf ${EXTLIB}/${FUSE}/install/usr/local/* ${INSTALL_DIR};
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@touch ${FUSE}/.nvt_finish

freetype: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${FREETYPE}/.nvt_finish ]; then \
		$(call check_exist, ${FREETYPE}); \
		cd ${FREETYPE}; mkdir result; CC=${NVT_HOST}-gcc; ./configure --prefix=${INSTALL_DIR} --host=${NVT_HOST}; \
		make -j${MULTI_CORES}; \
	fi
	@cd ${FREETYPE}; make install;
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@touch ${FREETYPE}/.nvt_finish

sqlite3: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${SQLITE3}/.nvt_finish ]; then \
		$(call check_exist, ${SQLITE3}); \
		cd ${SQLITE3}; make CC=${CC} AR=${AR} STRIP="${CROSS_COMPILE}strip" -j${MULTI_CORES}; \
	fi
	@cd ${SQLITE3}; make INSTALL_DIR=${INSTALL_DIR} install;
	@touch ${SQLITE3}/.nvt_finish

ffmpeg: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${FFMPEG}/.nvt_finish ]; then \
		$(call check_exist, ${FFMPEG}); \
		cd ${FFMPEG}; ./configure --prefix=${INSTALL_DIR} --cross-prefix=${CROSS_COMPILE} --target-os=linux --arch=${ARCH} --disable-everything --enable-muxer=mpegts --enable-muxer=mp4 --disable-encoders --disable-decoders --enable-protocol=file --enable-small --disable-static --enable-shared --enable-cross-compile --disable-zlib --disable-mipsfpu --disable-ffmpeg --disable-ffserver --disable-ffprobe --disable-swscale --disable-swresample --disable-avdevice --disable-avfilter --disable-debug; \
		make -j${MULTI_CORES}; \
	fi
	@cd ${FFMPEG}; make install;
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@touch ${FFMPEG}/.nvt_finish

libnl: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${LIBNL}/.nvt_finish ]; then \
		$(call check_exist, ${LIBNL}); \
		cd ${LIBNL}; ./configure --prefix=${INSTALL_DIR} --host=${NVT_HOST} CC=${CC} LD=${LD} AR=${AR}; \
		make -j${MULTI_CORES}; \
	fi
	@cd ${LIBNL}; make install;
	@cp ${INSTALL_DIR}/lib/libnl-3.so ${INSTALL_DIR}/lib/libnl.so ; cp ${INSTALL_DIR}/lib/libnl-genl-3.so ${INSTALL_DIR}/lib/libnl-genl.so
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig ${INSTALL_DIR}/lib/libnl
	@touch ${LIBNL}/.nvt_finish

opencv: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${OPENCV}/.nvt_finish ]; then \
		$(call check_exist, ${OPENCV}); \
		cd ${OPENCV}; make CC=${CC} CXX=${CXX} LD=${LD} AR=${AR} SYSROOT_PATH=${SYSROOT_PATH}; \
	fi
	@cd ${OPENCV}; make INSTALL_DIR=${INSTALL_DIR} install;
	@touch ${OPENCV}/.nvt_finish

openssl: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${OPENSSL}/.nvt_finish ]; then \
		$(call check_exist, ${OPENSSL}); \
		cd ${OPENSSL}; CROSS_COMPILE="" CC=${CC} LD=${LD} AR=${AR} RANLIB=${RANLIB} ./Configure shared linux-generic32 -DHAVE_CRYPTODEV -DUSE_CRYPTODEV_DIGESTS -DOPENSSL_SSL_CLIENT_ENGINE_AUTO --prefix=${INSTALL_DIR} -I$(SUPPLEMENT_DIR)/crypto/cryptodev-linux/; make; \
	fi
	@cd ${OPENSSL}; make install;
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig ${INSTALL_DIR}/lib/engines
	@touch ${OPENSSL}/.nvt_finish

wolfssl: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${WOLFSSL}/.nvt_finish ]; then \
		$(call check_exist, ${WOLFSSL}); \
		cd ${WOLFSSL}; CROSS_COMPILE="" CC=${CC} LD=${LD} AR=${AR} RANLIB=${RANLIB} ./configure --enable-ecc --enable-fastmath --enable-sslv3 --enable-oldtls --disable-filesystem --disable-chacha --disable-dh --disable-des3 --disable-sha512 --disable-poly1305 --disable-crypttests --disable-examples --host=${NVT_HOST} --build=i686-linux --prefix=${INSTALL_DIR}; \
		make -j$(MULTI_CORES); \
	fi
	@cd ${WOLFSSL}; make install;
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@touch ${WOLFSSL}/.nvt_finish

curl: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
ifeq ($(NVT_CURL_SSL),NVT_CURL_SSL_OPENSSL)
	@if [ -z `cat ${CURL}/.nvt_finish | grep NVT_CURL_SSL_OPENSSL` ]; then \
		$(call check_exist, ${CURL}); \
		cd ${CURL}; CPPFLAGS="-I${INSTALL_DIR}/include -I${INCLUDE_DIR}" LDFLAGS="-L${LIBRARY_DIR} -L${INSTALL_DIR}/lib -Wl,-rpath-link=$(INSTALL_DIR)/lib" LIBS="-lssl" ./configure --with-ssl --prefix=${INSTALL_DIR} --with-ca-path="/etc/ssl/certs" --host=${NVT_HOST}; make -j${MULTI_CORES}; \
	fi
else
	if [ -z `cat ${CURL}/.nvt_finish | grep NVT_CURL_SSL_WOLFSSL` ]; then \
		$(call check_exist, ${CURL}); \
		cd ${CURL}; CPPFLAGS="-I${INSTALL_DIR}/include -I${INCLUDE_DIR}" LDFLAGS="-L${LIBRARY_DIR} -L${INSTALL_DIR}/lib -Wl,-rpath-link=$(INSTALL_DIR)/lib" LIBS="-lwolfssl" ./configure --without-ssl --with-cyassl --enable-ipv6 --prefix=${INSTALL_DIR} --host=${NVT_HOST}; make -j${MULTI_CORES}; \
	fi
endif
	@cd ${CURL}; make install;
	@cd ${INSTALL_DIR}/bin; rm curl-config
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@echo $(NVT_CURL_SSL) > ${CURL}/.nvt_finish

zbar: init libiconv
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${ZBAR}/.nvt_finish ]; then \
		$(call check_exist, ${ZBAR}); \
		cd ${ZBAR}; CROSS_COMPILE="" CC=${CC} LD=${LD} AR=${AR} RANLIB=${RANLIB} ./configure --prefix=${INSTALL_DIR} --disable-pthread --disable-video --without-imagemagick --without-qt --without-python --without-gtk --disable-dependency-tracking --host=${NVT_HOST}; make -j${MULTI_CORES}; \
	fi
	cd ${ZBAR}; make install;
	@rm -rf ${INSTALL_DIR}/lib/pkgconfig
	@touch ${ZBAR}/.nvt_finish

libiconv: init
	@echo ">>>>>>>>>>>>>>>>>>> $@ compiling >>>>>>>>>>>>>>>>>>>"
	@if [ ! -f ${LIBICONV}/.nvt_finish ]; then \
		$(call check_exist, ${LIBICONV}); \
		cd ${LIBICONV}; ./configure --prefix=${INSTALL_DIR} --host=${NVT_HOST} CC=${CC} LD=${LD} AR=${AR}; \
		make -j${MULTI_CORES}; \
	fi
	@cd ${LIBICONV}; make install;
	@touch ${LIBICONV}/.nvt_finish

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ install>>>>>>>>>>>>>>>>>>>"
	@cp -arvf ${INSTALL_DIR}/include ${LINUX_BUILD_TOP}
	@cp -arvf ${INSTALL_DIR}/lib ${LINUX_BUILD_TOP}

clean:
	@echo ">>>>>>>>>>>>>>>>>>> Remove >>>>>>>>>>>>>>>>>>>"
	@rm -rf ${INSTALL_DIR}
	@rm -rf ${XML2} ${FUSE} ${FREETYPE} ${FFMPEG} ${LIBNL} ${SQLITE3} ${OPENCV} ${OPENSSL} ${CURL} ${WOLFSSL} ${ZBAR} ${LIBICONV}
