Import('env')
env.Replace(FUSESCMD="avrdude $UPLOADERFLAGS -e -Ulock:w:0xFF:m -Uhfuse:w:0xDE:m -Uefuse:w:0xFF:m -Ulfuse:w:0xE2:m")
