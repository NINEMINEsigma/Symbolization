# Symbolization
---

这是一门基于c++编写的语言, 用于实现一些神奇的所见即所得的特性,
其由解释器按行解释

# EBNF文法描述

ordinary_symbol = < IDENTIFIER`[A-Za-z]\w*` >
keyword_symbol = < KEY`single char` >

# 标准

## 定义

只存在两种最基础的语素, 一种是符号, 一种是字符串字面量

- 符号

```text
define, one, open, sadsafas, ;, '', 1, ...
```
符号由不被空格等不可见符号与保留符号分隔的单个单词与保留符号所组成

前者为`[A-Za-z]\w*`所匹配(虽然不被限制, 但请尽量不要使用双下划线开头的符号),

后者将在随后解释

- 字符串

```text
"define", "one", "open", "asd asda s", ";", "\"", "1", ...
```
字符串的字面量如c的字符串一样, 某些字符需要使用转义字符表达

字符串字面量为`"[^"]*"`所匹配

## 类型

以下列举的类型种类为内置类型, 均不可互相隐式转换

- 数值

数值字面量为`([+-]?)\d+`所匹配的模式, 默认为数值, 分母为1, 符号为正

数值可以表示绝对值最大为2^65-1, 最小精度为1/(2^65-1)的有理数

使用两个数值和分号也可以产生一个数值字面量,
使用数字字面量的生成模式为`\d+?/\d+?`
```cpp
1/1
+3/2
-5/9
```
注意`([+-]?)\d+?.\d+?`所匹配的不被视作一个可作为数值的符号
```cpp
1.5 != 3/2; //这不是一个数值, 1.5是一个符号
```
并且
```cpp
1.5/2.5 != (3/2)/(5/2); //1.5和2.5都是符号, 不是数值
```
请使用`\d+?\.\d+?f`所匹配的模式来产生一个数值字面量
```cpp
1.5f == 3/2; //1.5f是一个数值
```

- 字符/字符串

字符串字面量为`"[^"]*"`所匹配的模式

字符串的可枚举最小单元为字符, 字符是分母固定为1且符号固定为正的数值类型

在符号没有被[声明](#__build_in_define)前, 非数值字面量的符号内容是符号自身的字符串, 这意味着当text没有被声明时以下两种写法等价
```cpp
string a = text; //使用默认内容的未声明符号
string b = "text"; //使用字符串字面量
```

- 指针

指针字面量为`0x\d+`

指针指向内存中

- 规则

规则是一类特殊的表达式, 在其被定义后将会影响后续的语法或其他内容

关于规则的更多内容在下文被讲述

## 内置符号

未特殊说明的内置符号都可以在标准库std中找到对应的无__build_in前缀版本

<a id="__build_in_line_annotation"></a>
<a id="__build_in_start_annotation"></a>
<a id="__build_in_end_annotation"></a>
- 操作符 `行注释` __build_in_line_annotation , std中定义为双斜线`//`
- 操作符 `开始注释域` __build_in_start_annotation , std中定义为`/*`
- 操作符 `结束注释域` __build_in_end_annotation , std中定义为`*/`

使用方法:
```cpp
// 行注释后直到行尾都视为注释
/* 注释域中所有内容都视为注释 */
```

<a id="__build_in_end_sentence"></a>
- 操作符 `结束语句` __build_in_end_sentence , std中定义为分号`;`

```cpp
; //单个分号所组成的空语句是合法的
```

<a id="__build_in_start_expression"></a>
<a id="__build_in_end_expression"></a>
- 操作符 `开始局部表达式` __build_in_start_expression , std中定义为`(`
- 操作符 `结束局部表达式` __build_in_end_expression , std中定义为`)`

使用方法:
```cpp
expression = (expression);
```

<a id="__build_in_throw"></a>
<a id="__build_in_error"></a>
- 操作符 `抛出错误` __build_in_throw
- 操作符 `定义错误` __build_in_error

使用方法:
```cpp
throw error(error_id, error_message)
```
使用一个数值类型的id与字符串类型的信息定义错误后抛出

<a id="__build_in_define"></a>
<a id="__build_in_undefine"></a>
- 操作符 `声明/定义` __build_in_define
- 操作符 `取消声明/定义` __build_in_undefine

使用方法:
```cpp
define symbol; //声明一个符号已被使用
define symbol other-symbol; //定义一个符号为另一个符号的别名
undefine symbol; //取消一个符号的声明/定义
```
声明一个已声明的符号或取消声明一个未声明的符号都会抛出一个错误

<a id="__build_in_start_region"></a>
<a id="__build_in_end_region"></a>
<a id="__build_in_namespace"></a>
- 操作符 `开始域` __bulid_in_start_region , std中定义为左大括号`{`
- 操作符 `结束域` __build_in_end_region , std中定义为右大括号`}`
- 操作符 `命名空间命名` __build_in_namespace

namespace的使用方法:
```cpp
namespace name
```
namespace操作符所构成的语句将作用于随后的第一个符号

开始域与结束域必须配套使用
```cpp
// 命名一个域
namespace name
{

}
// 一个匿名的域
{

}
```
可以使用`::`(__build_in_root_namespace)从根命名空间按绝对路径取出域,
否则将以当前命名空间为根域, 使用`.`(__build_in_member_touch)从域中取出成员
```cpp
namespace a
{
    b; //错误, 此时命名空间::a.b还没有被声明
    std; //在此处的std是标准库, 即::std
    namespace b
    {

    }
    namespace std
    {

    }
    std; //在此处的std是a下的命名空间, 即::a.std
}
a.b; //
```

<a id="__build_in_try"></a>
<a id="__build_in_catch"></a>
- 操作符 `尝试` __build_in_try
- 操作符 `捕获` __build_in_catch

<a id="__build_in_if"></a>
- __build_in_if

```cpp
__build_in_if(expression)
{

}
else
{

}
```

检查一个符号是否被声明:
```
```
