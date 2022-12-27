# ChatService
基于luwq2022/ServiceBase项目，实现的一个聊天室项目

项目开发的IDE是VSCode，代码中.vscode配置可能需要根据本地环境调整。

ServiceBase的lib库目前没有包含在项目中，需要自行下载编译luwq2022/ServiceBase

项目提供了CMakeLists文件，下载后先使用CMake生成makefile，再使用make命令生成目标文件

执行CMake前最好先创建build文件夹

mac上执行
cmake -G 'Unix Makefiles'
make

windows系统执行
cd ./build | cmake .. -G 'MinGW Makefiles'
cd ./build | make
