# Symbolization
---

自符号化语言标准,本仓库中根据此标准创建实现`Symbolization`,
用于实现一些神奇的所见即所得的特性,
其由解释器按行解释

# EBNF文法描述

IDENTIFIER 是标识符,
STRING 是以双引号包括的字符串字面量,
NUMBER 是数值字面量,
CHAR 是以单引号包括的单字符字面量,
`build in type`与`keyword`将在后文中提到

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

Survival    = "static"
              | "global"
              | "local" .

DefineVariable      = [ Survival ] Term
                      IDENTIFIER [ RightHand ].

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

不与关键字重复的模式`[A-Za-z_][A-Za-z0-9_]*`

- `STRING` 字符串字面量

模式`"((\\\S)|[^\\"])*?"`

- `CHAR` 单字符字面量

模式`'((\\\S)|[^\\"])'`

- `NUMBER` 数值字面量

模式`[+-]?(0|([0-9]+))(\.[0-9]+)?`

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

## 关键字

关键字包括`内置类型`和以下内容

- 行为声明
`function` 函数定义
`struct` 结构定义
`require ... from ...` 从(文件)导入(命名空间),
被导入的命名空间内的全体定义可以在本空间被得到,
具有传递性
`catch ... from` 从(域)中捕获(errorId)异常并在(域)中处理

- 行为限定
`unsign` 取消符号, 接收一个整型的类型返回其对应的无符号类型

- 逻辑
`if` `else` `elif` 判断
`goto` 跳转
`while` `for` `foreach` 循环
    - while(conditions){}
    - for(init;conditions;transfer)
    - foreach(type var-name:enumerate)

- 类函数
`address` 取地址, 接收一个任意类型对象返回一个指针
`typeof` 取类型, 接收一个任意类型对象返回其类型的原始符号
`typeid` 取类型, 接收一个任意类型对象返回uint32
`instanceof` 取对象, 接收一个对象返回其原始符号
`instanceid` 取对象, 接收一个对象返回uint32
`symbolof` 取符号, 接收一个符号返回一个符号对象
`assert` 断言, 接收一个整数不返回
`throw` 抛出异常, 形参列表为 **(int32 errorId, string message)** 不返回

- 特定符号
一些特定符号无法以某些形式重定义

## 类型

### 类型转换

#### 显式类型转换

当存在函数定义满足

- 函数名是目标类型
- 只有一个形参且类型是被转换类型(可以是动态绑定类型)

时被转换类型可以显式类型转换至目标类型

```
function Type1(Type2 right) -> Type1
{
    ...
    return Ret;
}
Type2 a = Type1(Type1Instance);
```

部分内置类型已经内置定义了向其他内置类型显式转换的函数, 如
- 字符串类型与数值类型的双向转换
- 数值类型内部所有类型
- 指针类型与整型

#### 隐式类型转换

部分内置类型能够隐式类型转换到其他内置类型

```
```

### 内置基础类型

以下类型未特殊说明均不可隐式转换

- `int8` `int16` `int32` `int64` 有符号整型

分别对应n位的有符号整数类型, 可以隐式向高位有符号整型类型转换,
可以向浮点数类型隐式转换

- `uint8` `uint16` `uint32` `uint64` 无符号整型

分别对应n位的无符号整数类型, 可以隐式向高位无符号整型类型转换,
可以向浮点数类型隐式转换

- `float` `double` 浮点数类型

分别对应32位单精度浮点数, 64位双精度浮点数,
float可以向double隐式转换

- `string` `string16` `string32` `string64` 字符串类型

字符串是单字符的顺序结构,
分别对应使用intn作为单字符类型的字符串,
无后缀的string使用的单字符类型总是等于一个字节长度

- `type*` `intptr` 指针类型

在类型后跟星号表示使用其指针类型,
使用address获取对象的地址

- `type[]` 数组类型

特指位于栈上的数组类型

- `symbol` 符号类型

保存其在源代码中的属性

- `void` 空类型

仅用于指示函数不返回值

### 动态绑定类型

使用未声明的符号作为`Type`语素,
在词义分析中将作为动态绑定类型,
可以在函数形参和声明/定义变量时使用

变量声明中的动态绑定类型将在首次赋值时被确定,
函数形参的动态绑定类型在每次赋予实参时被更新

```
Type t1, t2;
{
      //index的类型是var, var符号的值是int32
      var index = 1;
      t1 = Type(var);
}
{
      //str的类型是var, var符号的值尚未确定
      var str;
      //var符号的值在初次赋值中确定为对象的类型
      str = "string";
      t2 = Type(var);
}
assert(t1!=t2);
```

## 变量

### 生存域

变量是对象在代码中的指示器,
当所有指向对象的变量都被销毁时对象才被销毁

- `static` 静态

静态生存的变量不会在离开当前域时被销毁,
仅在初次到达变量定义时赋值

- `global` 全局

全局生存的变量不会在离开当前域时被销毁,
仅在程序启动时赋值

- `local` 局部(默认)

局部生存的变量在离开当前域时被销毁,
在每次到达变量定义时赋值

### 可见性

- `public` 完全公开

任何位置的任何内容都可以获得与操作该字段

- `internal` 内禀公开

在本命名空间内的任何内容都可以获得与操作该字段

- `protected` 保护公开

任何位置的本结构函数都可以获得与操作该字段

- `private` 完全私有

在本命名空间内的本结构函数都可以获得与操作该字段

## 结构

结构仅作为数据的对象,
内部仅能够定义变量字段,
字段的定义顺序与其字段在内存模型中的顺序相同

如结构
```cpp
struct Base
{
    int32 a = 0;
    int32 b = 0;
}
```
其内存布局即为
```text
-----------------------------------------------------
| header... | 32 bit for a | 32 bit for b | tail... |
-----------------------------------------------------
```

与结构名同名且返回值为该结构的函数为结构的构造函数,
在找不到构造函数时将使用默认构造函数,
默认构造函数使用结构中的字段依次作为形参,
默认构造函数仅在具有与默认构造函数相同形参的构造函数具有定义时被覆盖
```
function Base() -> Base
{
    return Base(0, 0);
}
function Base(int32 a) -> Base
{
    return Base(a, 0);
}
```

在**结构名前加~ 且 返回值为void 且 形参数量为0**的函数
为该结构的析构函数,
在找不到析构函数时使用默认析构函数,
无论是否定义了析构函数, 默认析构函数都会在释放内存前被调用
```
function ~Base() -> void
{

}
```

此时Base类在c++中的表示如下
```cpp
struct Base
{
    //字段定义
    int32_t a, b;
    //默认构造函数
    Base(int32_t a, int32_t b): a(a), b(b) {}
    //分别对应上述两条函数
    Base(): Base(0, 0) {}
    Base(int32_t a): Base(a, 0) {}
    ~Base(){}
};
```

## 函数

### 函数的定义

使用关键字`function`定义函数,
使用一个标识符作为函数名词,
后跟函数形参列表与函数返回值类型,
最终定义函数体

```
function FunctionName(...) -> Type { ... }
```

### 从属于类型的函数

函数从属于首个形参的类型(如果存在的话),
例如
```
function +(Vector2 a, Vector2 b) -> Vector2
{
    return Vector2(a.x+b.x, a.y+b.y);
}
```
可以通过以下两种形式调用
```
Vector2 c = a + b;//操作符形式调用
Vector2 d = +(a, b);//普通函数形式调用
```

这提供了更加自然的代码环境
```
an instance can call by foo;
```
通过这些内容, 这条语句将是合法的,
并且instance将会作为实参传递给函数foo进行调用
```
function an(Any right) -> Any
{
    return right;
}
function can(Any self) -> Any
{
    return right;
}
function call(Any self, callType how_call, functional func) -> Ret
{
    //等同于
    //return how_call(self, func);
    return self how_call func;
}
function by(Any self, functional func)
{
    //等同于
    //return func self;
    //也等同于
    //return self func
    return func(self);
}
function foo(instanceType instance) -> instanceType
{
    //any work
    return instance;
}
```


