# 😀 从实模式切换到保护模式

在实模式下，操作系统需要**初始化段表（如`GDT`）和描述符表寄存器（如`GDTR`）**。在初始化完成后，操作系统通过将**0号控制寄存器（`CR0`）中的`PE`位置为1**的方式，来通知机器进入保护模式。在此之前，`CR0`中的`PE`初始化为0。`CR0`寄存器的结构请自行参阅i386手册的相关内容。

（具体内容会在后面实验里有直观体验。）