ndk-build -j NDK_DEBUG=0 && cp ./libs/arm64-v8a/libmain.so /sdcard/Android/data/net.zhiji.snake/files/ && echo "\033[32mCopyed OK!\033[0m" && exit

# if error
echo "\033[31mb3i.sh execute failed!\033[0m"
