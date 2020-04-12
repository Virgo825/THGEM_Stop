### 要求: 
要正常编译该程序，需要 <font color=orange>Geant4 10</font> 版本。

### 介绍
- 模拟中子与 B<sub>4</sub>C 反应。
- 保存 alpha 与 <sup>7</sup>Li 粒子信息以及初电离电子的信息到 root 文件，以中子波长与探测器参数来命名 root 文件。
- 可以通过mac文件来修改探测器结构，包括尺寸、材料以及层数，也可以通过设置中子波长来设置中子能量。
- 可以通过自带的 <font color=blue>gun</font> 来修改入射粒子信息。


### 编译运行
```bash
mkdir build
cd build
cmake ..
make -j4
```
以 batch mode 运行程序, 在终端输入 
```bash
./thgem -m ./run.mac
```
程序也可以以可视化的方式来运行，在终端输入
```bash
./thgem
```
然后点击 <font color=magenta>run</font> 按钮.
