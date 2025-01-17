mkdir installing_here
cd installing_here
cmake ../.. -DCMAKE_INSTALL_PREFIX=../installed_here
cmake --build . --target install
cd ..
