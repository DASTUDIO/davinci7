# davinci7

## 算法介绍

### 功能

* 任意数据转数字（1234567890）
* 允许加密
* 原文可以有多个不同密文  
* 自由扩充定制密文数据大小

### 用途
* 反破解
* 未来压缩
* 隐写术（信息转数字，数字能藏在很多地方，比如某些比例）

## 使用
#### 四个入口点

encode：字符串转为数字

decode：数字还原字符串

crypt： 转为加密数字

decrypt： 加密数字解密还原为字符串

```c
extern "C" int encode(char* input, char* output);
extern "C" int decode(char* input, char* output);
extern "C" int crypt(char* input, char* outpur, char* password);
extern "C" int decrypt(char* input, char* output, char* password);
```
#### 非字符串
有些数据不是字符串，比如图片，我们这样处理

1.获取bytes[]数组

2.把bytes[]数组转化为base64字符串

## 示例

Unity3D里调用：

1.把编译好的库放入Asset/Plugins文件夹 (product文件夹里有已编译的)

2.代码如下

```cs
using UnityEngine;
using System.Runtime.InteropServices;

public class TestCall : MonoBehaviour
{
    [DllImport("libdvc7",EntryPoint ="encode")]
    private static extern string encode(ref byte input,ref byte output);

    void Start()
    {
        byte[] _out = new byte[1024];
        encode(ref System.Text.Encoding.UTF8.GetBytes("hello")[0], ref _out[0]);
        Debug.Log(System.Text.Encoding.UTF8.GetString(_out));
    }

}

```



# 附录

## 原理
#### 假设有3个byte的分别为 

1: 101 111 01 

2: 0 101 001 0 

3: 10 101 111  

#### 从第一个开始 每次取3个bit（3位）高5位补0 变成一个新byte

#### 这样取完3个byte 得到8个新byte

1. 0000 0101
2. 0000 0111
3. 0000 0010
4. 0000 0101 
5. 0000 0001
6. 0000 0010
7. 0000 0101
8. 0000 0111

#### 因为 2^0+2^1+2^2 = 7

#### 所以这样的只有低三位的byte表达的是一个 0-7的数字(int)

#### 这样就完成了把3个比特转化为8个数字

## 优化
让密文体积减少50%
>多位密码时，因为密文都是'0'-'9'的字符,这种字符减去48后，就是0-9的二进制值,这些0-9二进制的byte的高4位都是0，所以可以把两个byte高4位去掉然后把它们拼接成一个新的8位byte，然后把这个新byte带入下一轮(3bytes->8bytes)迭代加密.

## &

## （数学）D7问题
### 环境前提
已知数据通过davinci7算法转化为数字

例如某一段数据转化后的密文为: `142857142857142857142857142857142857142857`

### 提出问题

能否找到最小的两个数 a,b 

使得 a/b 的前n位 刚好为密文 

同时a,b的长度之和小于密文长度

#### 如 :

`142857142857142857142857142857142857142857`   

密文的长度 n 为35

它的的 a,b 分别是 a=1,b=7

### 意义
若此问题得到解决，压缩率就可大幅提升，一段很长的数据可以表示两个数的商，或者多段商的拼接。


### 量子计算
如果拿现在计算机以穷举法匹配结果，效率非常低且不一定是最优解。

如果利用量子计算机并行计算的特性，我想找出最优解不难。 


