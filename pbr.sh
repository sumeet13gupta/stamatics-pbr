START=$(date +%s);
mkdir build
cd build
cmake ..
cmake --build .
cd bin/Debug/
start pbr.exe
sleep 2
start out.png
END=$(date +%s);
echo $((END-START))  | awk '{print "Time Elapsed : "int($1/60)" min, "int($1%60)" sec."}'