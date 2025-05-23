# Symbolization
---

自符号化语言标准,本仓库中根据此标准创建实现`Symbolization`,
用于实现一些神奇的所见即所得的特性,
其由解释器按行解释

# EBNF文法描述

IDENTIFIER 是标识符, 
STRING 是以双引号包括的字符串字面量, 
NUMBER 是数值字面量, 
CHAR 是以单引号包括的单字符字面量

```
Program     = { Namespace } .

Namespace   = "namespace" IDENTIFIER
              "{"
                    [ { Requirement } ]
                    [ { Content } ]
              "}" .

Requirement = "require" IDENTIFIER 
              [ "from" STRING ] ";" .

Content     = Function
              | Structure .

Function    = "function" IDENTIFIER DefineParameters
              "->" Term
              "{"
                    [ { Production ";" } ]
              "}".

Structure   = "struct" IDENTIFIER 
              [ "inherit" { IDENTIFIER } ]
              "{"
                    { [ Visibility ]
                    DefineVariable ";" }
              "}" .

DefineParameters    = "(" {
                      Term IDENTIFIER 
                      { [ "," Term IDENTIFIER ] } 
                      } ")" .

Visibility  = "private"
              | "internal"
              | "protected"
              | "public" .
              
DefineVariable      = Term IDENTIFIER RightHand .

Term        = ( IDENTIFIER { "." IDENTIFIER } ) 
              | < build-in type > .
              
Production  = Assignment
              | DefineVariable 
              | Return 
              | Expression 
              | Block .

Assignment  = IDENTIFIER { RightHand } .

Return      = "return" Expression .

Expression  = [ "(" ] Phrase [ ")" ] .

Block       = ( IDENTIFIER 
                | < keyword > ) 
              "(" [ Expression ] 
                [ ";" { Expression } ] ")"
              "{" 
                    [ { Production } ] 
              "}" .

RightHand   = "=" Expression .

Phrase      = Subject [ Predicate Object ]
              | Predicate [ Object ] .

Subject     = Term 
              | STRING 
              | NUMBER 
              | CHAR .

Predicate   = Term 
              | < keyword > .
              | < build in function > .

Object      = Subject
              | Parameters .

Parameters  = "("
              ( Subject 
                | "default" )
              { [ "," ( Subject 
                        | "default" ) ] }
              ")" .

Annotation  = ( "//" < single line annotation > )
              | ( "/*" < annotation > "/*" ) .
```

- `IDENTIFIER`标识符

不与关键字重复的模式`^[^0-9\s][^\s]*$`

- `STRING` 字符串字面量

模式`^"([^"\\]|\\.)*"$`

- `CHAR` 单字符字面量

模式`^([^'\\]|\\.)'$`

- `NUMBER` 数值字面量

模式`^[+-]?(0|[1-9][0-9]*)\.([0-9]*)?$`

# 例子

```
namespace Graphics 
{
      //导入标准库, 使得本命名空间可以使用标准库中的内容
      require std;

      //定义结构
      struct Point3D
      {
            //定义字段, 默认可见性为public
            float x = 0;
            float y = 0;
            float z = 0;
            //定义私有字段
            float w = 0;
      }

      /*
      Point3D a = Point3D(default, default, default);
      Point3D b = Point3D(default, default, default);
      Point3D c = a + b;函数的操作符调用形式
      Point3D d = +(a, b);普通函数调用形式
      */
      function + (Point3D a, Point3D b) -> Point3D
      {
            return Point3D(a.x+b.x,
                           a.y+b.y,
                           a.z+b.z);
      }
}
```

# 标准

## 类型

### 内置基础类型

以下类型未特殊说明均不可隐式转换

- `int8` `int16` `int32` `int64` 整数类型

分别对应n位的整数类型

- `float` `double` 浮点数类型

分别对应32位单精度浮点数, 64位双精度浮点数

- `string` 字符串类型

单字符的类型是int32, 字符串是单字符的顺序结构

- `intptr` 指针类型

在32/64位平台上分别使用int32/int64作为真实类型,
使用address获取对象的地址

- `symbol` 符号类型

符号类型不可声明不可定义, 
未定义的标识符即为符号类型, 值为其标识符自身

- `void` 空类型

指示函数不返回值

### struct结构

结构仅作为数据的对象, 内部仅能够定义变量字段

#### 可见性

- `public` 完全公开

任何位置的任何内容都可以获得与操作该字段

- `internal` 内禀公开

在本命名空间内的任何内容都可以获得与操作该字段

- `protected` 保护公开

任何位置的本结构函数都可以获得与操作该字段

- `private` 完全私有

在本命名空间内的本结构函数都可以获得与操作该字段


