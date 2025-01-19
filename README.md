# 项目说明

本项目为2024年秋季《形式语言与自动机》课程的实验作业。

## 软件依赖

- 必需：
  - cmake >= 3.15.0
  - g++ >= 9.3.0
- 可选:
  - just >= 0.9.0 (运行脚本)
  - catch2 >= 3.0.0 (测试框架)
  - pytest >= 8.3.2 (测试框架)
  - doxygen >= 1.9.2 (文档生成)

## 项目结构

本项目的结构如下：

```plain
root
    |- CMakeLists.txt
    |- README.md
    |- fla-project
        |- app          // 命令行接口
        |- docs         // 软件文档生成
        |- include      // 库头文件
        |- src          // 库源文件
        |- test         // 库测试文件
    |- python       // pytest测试脚本
        |- ...
    |- justfile     // just一键运行脚本
    |- pda          // pda 文件
        |- case.pda
        |- ...
    |- tm           // tm 文件
        |- case1.tm
        |- case2.tm
        |- ...
```

## 编译方法

本项目可通过如下方式编译：

- bash 命令行：

  ```bash
  cmake -S . -B build
  cmake --build build
  ```

- just 脚本：

  ```just
  just build
  ```

编译完成后将在 bin 目录下生成可执行文件`fla`, 使用方式如下：

```bash
Usage:  fla [-h|--help]
        fla [-v|--verbose] <pda> <input>
        fla [-v|--verbose] <tm> <input>
```

## 测试

本项目可通过如下方式进行测试,请确保环境中包含 catch2 或着 pytest:

- catch2 :

  ```bash
  cmake -S . -B build
  cmake --build build --target test
  ```

  或者

  ```just
  just test
  ```

- pytest:

  ```bash
  pytest ./python
  ```

  或者

  ```just
  just pytest
  ```
