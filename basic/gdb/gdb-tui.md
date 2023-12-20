# gdb tui——gdb图形化界面

gdb调试有一个让很多人头疼的问题，很多Linux用户或者从其他平台转过来习惯了有强大的源码显示窗口的调试器可能对gdb用list命令显示源码的方式非常不习惯，主要是因为gdb在调试的时候不能很好的展示源码。gdb里面可以用list命令显示源码，但是list命令显示没有代码高亮，也不能一眼定位到正在执行的那行代码在整个代码中的位置。可以毫不夸张的说，这个问题是阻止很多人长期使用gdb的最大的障碍。如此的不便，以至于GNU都想办法解决了——使用gdb自带的gdbtui。

## 开启gdb TUI模式

### 方法一：使用gdbtui或者gdb-tui开启一个调试。

> 方法一：使用gdbtui或者gdb-tui开启一个调试。

## 方法二：直接使用gdb调试代码，在需要的时候使用切换键Ctrl + x + a调出gdbtui。

## gdb TUI模式下有4个窗口

默认情况下，gdbtui模式下会显示command窗口和source窗口。TUI模式下还有其他窗口，如下面列举的四个常用的：

- (cmd) command命令窗口，可以键入调试命令。
- (src) source源代码窗口， 显示当前行、断点等信息。
- (asm) assembly汇编代码窗口。
- (reg) register寄存器窗口。

可以通过layout + 窗口类型命令来进行选择自己需要的窗口，例如在cmd窗口输入layout asm则可以切换到汇编代码窗口。

layout命令还可以用来修改窗口布局，可以在cmd窗口中输入help layout，常见的有：

```text
Usage: layout prev | next | <layout_name> 
Layout names are:
   src   : Displays source and command windows.
   asm   : Displays disassembly and command windows.
   split : Displays source, disassembly and command windows.
   regs  : Displays register window. If existing layout
           is source/command or assembly/command, the 
           register window is displayed. If the
           source/assembly/command (split) is displayed, 
           the register window is displayed with 
           the window that has current logical focus.
```

另外，可以通过winheight命令修改各个窗口的大小。如下所示：

```text
(gdb) help winheight
Set the height of a specified window.
Usage: winheight <win_name> [+ | -] <#lines>
Window names are:
src  : the source window
cmd  : the command window
asm  : the disassembly window
regs : the register display

##将代码窗口的高度扩大5行代码
winheight src + 5
##将代码窗口的高度减小4代码
winheight src - 4
```

## 解决tui窗口不自动更新内容的问题——space命令
当前gdb tui窗口放大或者缩小以后，gdbtui窗口中的内容不会自己刷新以适应新的窗口尺寸，我们可以通过space键强行让gdbtui窗口刷新。

## 窗口焦点切换
在默认设置下，方向键和PageUp/PageDown都是用来控制gdbtui的src窗口的，所以，我们常用的上下键用来显示前一条命令和后一条命令的功能就没有了，不过这个时候我们可以通过Ctrl + n/ctrl + p来获取这个功能。

注意：当我们通过方向键调整了gdbtui的src窗口以后，可以通过update命令重新把焦点定位到当前执行的代码上。

我们可以通过focus命令来调整焦点位置，默认情况下是在src窗口，通过focus next命令， 焦点就移到cmd窗口了，这时候就可以像以前一样，通过方向键来切换到上一条命令和下一条命令。同理，也可以使用focus prev切回到src窗口。如果焦点不在src窗口上，我们就不能通过方向键来浏览源码了。

```text
(gdb) help focus  
help focus
Set focus to named window or next/prev window.
Usage: focus {<win> | next | prev}
Valid Window names are:
src  : the source window
asm  : the disassembly window
regs : the register display
cmd  : the command window
```
