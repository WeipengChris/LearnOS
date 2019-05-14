制作软盘的过程中遇到的问题：

进入grub命令行后执行：device (fd0) /dev/loop0 
结果显示 File not found!
原因：需要在root用户下进入grub，才能找到loop中的文件
