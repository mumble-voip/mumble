qmake -recursive main.pro CONFIG+="release static no-elevation no-g15 no-asio" CONFIG-=sse2 DEFINES+="NO_UPDATE_CHECK NO_CRASH_REPORT PLUTOVR_BUILD"
