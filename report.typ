#set text(
  font: ("libertinus serif", "FZKai-Z03S"),
  size: 11pt,
)

#set page(
  paper: "a4",
  margin: (x: 1.8cm, y: 1.5cm),
)

#set par(
  justify: true,
  leading: 0.52em,
  first-line-indent: 0em,
)

#align(
  center,
  text(17pt)[
    FLA 实验报告
  ],
)

#grid(
  columns: 1fr,
  align(center)[
    韩加瑞 #h(0.5em) 221900156\
  ],
)


= 实验分析与设计思路

本次实验要求实现一个命令行工具，能够解析下推自动机（pda）和图灵机（tm）文件格式并模拟运行。此外还要求对于特定的问题，构造相应的pda和tm文件。

根据上述要求，本项目分为两个部分，分别为 fla 库和 main 文件。

fla 库中包含 Simulator 抽象类，其中包含了两个抽象方法 parse 和 run，分别用于解析文件和模拟运行。而PDASimulator 和 TMSimulator 类分别用于解析 pda 和 tm 文件，并模拟运行。

main 文件实现了命令行界面，并利用 fla 库中的类进行解析和运行。

本项目具体结构请参阅 README.md 文件。

= 实验完成情况

本项目完成了所有基本要求，并且实现了一些额外的功能。

1. pda 与 tm 文件格式的解析：
  本项目支持pda与tm文件的基本格式，并在此基础上支持在任意位置插入任意数量的空格（需要确保插入空格后不改变语义，如`=``,`前后或行首行尾）。
  此外对于解析错误的文件，本项目会给出尽可能详细的错误信息，不仅包含简单的格式错误（如状态名称包含特殊字符或），还包括可能的语义错误（如试图转移到不存在的状态或者pda存在重复的转移）。

2. pda 与 tm 文件的模拟运行：
  本项目支持pda与tm模拟运行的基础要求，并在此基础上完成了pda以及tm的 verbose 模式，即在模拟运行时输出每一步的详细信息。此外，若在模拟运行时产生错误，也会给出尽可能准确的错误信息。

3. pda和tm程序构造：
  本项目完成了所有的pda和tm程序构造要求，其文件位于相应的目录。

4. 测试框架：
  本项目实现了一个简单的测试框架，用于测试pda和tm的模拟运行以及命令行界面。具体使用方法请参考 README.md 文件。

= 待解决的问题

1. 本项目的测试框架尚不完善，缺少对于verbose模式的测试。

2. 本项目 fla 库对应的文档尚未实现，使用doxygen生成的文档无法阅读。

3. tm和pda中检测重复转移的算法不够高效

= 建议

1. 对于verbose模式的输出不应该追求格式一致，只需要检测普通模式下的输出是否正确即可。verbose模式下的输出格式应当根据具体项目结构决定。

2. 尽可能使用更新的c++标准，例如17和20已经被普遍支持。

3. 项目框架较为简略，希望能够提供较为现代的cpp项目框架，利于同学们学习最佳实践，具体可参考
#link("https://github.com/TheLartians/ModernCppStarter", "ModernCppStarter")。

