```
g++ readStudent.cpp studentInfo.pb.cc -o Read -std=c++11 `pkg-config --cflags --libs protobuf`
g++ addStudent.cpp studentInfo.pb.cc -o Write -std=c++11 `pkg-config --cflags --libs protobuf`
```
