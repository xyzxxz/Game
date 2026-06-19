---
name: "cpp-style-guide"
description: "C++ 代码规范智能体。提供全面的 C++ 编码规范指导，涵盖命名、格式、注释、现代 C++ 特性、头文件管理等。代码缩进为 4 空格。当用户编写、审查、重构 C++ 代码，或询问 C++ 编码风格与规范时调用此 skill。"
---

# C++ 代码规范智能体

本 skill 提供完整的 C++ 编码规范指导，适用于 C++17/20/23 项目。规范强调一致性、可读性、安全性和现代 C++ 最佳实践，同时明确禁止使用不安全特性。

> **核心规则：代码缩进统一为 4 空格，禁止使用 Tab 字符。**

**设计目标：**

1. **为读者优化**——代码被阅读的时间远多于被编写的时间
2. **保持一致性**——统一风格让自动化工具和代码审查生效，减少无谓争论
3. **避免危险构造**——预防表面无害、实则风险极高的 C++ 特性陷阱
4. **避免未定义行为**——不使用未定义、未声明或由实现定义的行为
5. **安全性优先**——内存安全、类型安全、并发安全贯穿始终

---

## 1. 缩进与空白

### 1.1 缩进规则

| 规则 | 说明 |
|------|------|
| 缩进单位 | **4 空格**，严禁使用 Tab 字符 |
| 编辑器配置 | 设置 Tab 键输出 4 空格 (expand tab) |
| 续行缩进 | 参数续行缩进 8 空格（2 级缩进），或对齐到上一行参数起始位置 |
| 访问控制标签 | `public:` / `protected:` / `private:` 缩进 4 空格 |
| `namespace` 内部 | **不缩进** namespace 内的代码 |
| `namespace` 结尾 | 右大括号后加注释 `// namespace <名称>` |
| 预处理指令 | `#` 号始终顶格，嵌套 `#if` 内部可缩进其内容 |
| `case` 标签 | `case` 与 `switch` 对齐，`case` 体内代码缩进 4 空格 |

```cpp
namespace my_project {

class MyClass {
public:
    MyClass();

    void DoSomething(int param1,
            const std::string& param2);  // 续行缩进 8 格

private:
    int value_;
};

void MyClass::DoSomething(int param1,
        const std::string& param2) {
    if (param1 > 0) {
        DoOneThing();
        DoAnotherThing();
    } else {
        LogError("invalid param1: {}", param1);
    }

    switch (param1) {
    case 0:
        HandleZero();
        break;
    case 1:
        HandleOne();
        break;
    default:
        HandleDefault();
        break;
    }
}

}  // namespace my_project
```

### 1.2 空行规则

- 函数 / 方法定义之间留一个空行
- 逻辑上独立的代码块之间留一个空行
- include 分组之间用空行分隔
- namespace 开始/结束与内容之间留空行

```cpp
#include <cstdint>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "my_project/my_header.h"
#include "my_project/other_header.h"

namespace my_project {

class Processor {
public:
    void ProcessA();

    void ProcessB();

private:
    int count_ = 0;
};

}  // namespace my_project
```

### 1.3 空格规则

| 场景 | 规则 | 示例 |
|------|------|------|
| `if` / `for` / `while` / `switch` | 关键字后留空格 | `if (cond)` |
| 函数调用 | 函数名与 `(` 之间不留空格 | `foo(x, y)` |
| 逗号与分号 | 逗号后留空格 | `f(a, b, c)` |
| 二元运算符 | 运算符两侧各留一个空格 | `a + b`, `x == y` |
| 一元运算符 | 与操作数之间不留空格 | `!flag`, `++i`, `*ptr` |
| 成员访问 | `.` / `->` 两侧不留空格 | `obj.method()`, `ptr->field` |
| 指针与引用声明 | `*` / `&` 紧贴类型 | `int* ptr`, `const std::string& ref` |
| 模板尖括号 | `>` 之前不留空格 | `std::vector<int>` |
| 初始化列表 | `:` 前留空格，后不留；逗号后留空格 | `Foo() : a_(1), b_(2) {}` |
| 范围 for | `:` 两侧各留空格 | `for (auto& x : vec)` |
| `return` 语句 | 不要加不必要的括号 | `return result;` |
| 类型转换 | 转换表达式后留空格 | `static_cast<int>(x)` |
| C 风格转换 | **禁止使用** | 用命名转换替代 |

---

## 2. 文件与命名规范

### 2.1 文件名

| 项目 | 约定 |
|------|------|
| 源文件扩展名 | `.cc`（推荐）或 `.cpp`（项目内保持一致） |
| 头文件扩展名 | `.h` |
| 文件名风格 | `snake_case`，如 `http_client.h`、`xml_parser.cc` |

### 2.2 命名总览表

| 元素 | 风格 | 示例 |
|------|------|------|
| 类名 / 结构体名 / 枚举名 / 概念名 | PascalCase（大驼峰） | `HttpClient`, `XmlParser`, `Sortable` |
| 函数名 / 方法名 | PascalCase（大驼峰），动词开头 | `GetName()`, `ProcessRequest()` |
| 变量名 | snake_case（全小写+下划线） | `user_id`, `buffer_size` |
| **成员变量** | snake_case + 后缀 `_` | `count_`, `name_` |
| 常量（constexpr / const） | k + PascalCase | `kMaxSize`, `kDefaultTimeout` |
| 枚举值 | k + PascalCase | `kRed`, `kFileNotFound` |
| 宏定义 | UPPER_SNAKE_CASE + 项目前缀 | `MY_PROJECT_LOG_ERROR(msg)` |
| 命名空间 | snake_case | `my_project`, `network_utils` |
| 类型别名 (using) | PascalCase | `using BufferPtr = std::shared_ptr<Buffer>` |
| 模板参数 | PascalCase 或大写字母 | `typename T`, `typename KeyType` |
| 全局变量 | 尽量避免；`g_` + snake_case | `g_config_path` |
| 静态变量 | 尽量避免；`s_` + snake_case | `s_instance_count` |

### 2.3 详细示例

```cpp
// 常量
constexpr int kMaxConnections = 100;
constexpr double kPi = 3.141592653589793;

// 枚举
enum class Color {
    kRed,
    kGreen,
    kBlue,
};

enum class ErrorCode {
    kOk = 0,
    kFileNotFound = 1,
    kPermissionDenied = 2,
};

// 类与函数
class HttpClient {
public:
    explicit HttpClient(std::string base_url);
    ~HttpClient() = default;

    HttpResponse Get(const std::string& path);
    HttpResponse Post(const std::string& path, const std::string& body);
    bool IsConnected() const;
    void SetTimeoutMs(int timeout_ms);

private:
    std::string base_url_;
    int timeout_ms_ = 5000;
    bool is_connected_ = false;
};
```

### 2.4 命名建议

- **语义明确**：避免缩写（业界通用缩写如 `http`、`xml`、`id`、`rpc` 除外）
- **函数用动词**：`OpenFile()`, `ParseJson()`, `ValidateInput()`
- **布尔函数** 加 `Is` / `Has` / `Can` / `Should` 前缀
- **禁止匈牙利命名法**：不用 `iCount`、`strName`、`pBuffer`
- **禁止以下划线开头**（`_name`）——`_Name` 和 `__name` 是标准保留标识符
- **全局作用域中避免过于简短的名字**

---

## 3. 头文件规范

### 3.1 自包含头文件

> 每个 `.h` 文件必须能够独立编译，不依赖特定的 include 顺序。

```cpp
// my_class.h — 自包含
#pragma once

#include <string>
#include <vector>

namespace my_project {

class MyClass {
public:
    void Process(const std::vector<std::string>& items);
};

}  // namespace my_project
```

### 3.2 头文件保护

```cpp
// 方案 A：简洁推荐
#pragma once

// 方案 B：传统 include guard
#ifndef MY_PROJECT_HTTP_CLIENT_H_
#define MY_PROJECT_HTTP_CLIENT_H_
// ...
#endif  // MY_PROJECT_HTTP_CLIENT_H_
```

### 3.3 Include 顺序

```
1. 对应的主头文件（.cc 对应的 .h）
   （空行）
2. C 系统头文件           ← <cstdint>, <cstring>
   （空行）
3. C++ 标准库头文件       ← <algorithm>, <string>, <vector>
   （空行）
4. 第三方库头文件         ← <fmt/core.h>
   （空行）
5. 本项目头文件           ← "my_project/config.h"
```

各组内部按字母序排列。

```cpp
// my_class.cc — 示例
#include "my_project/my_class.h"

#include <cstdint>
#include <cstring>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "my_project/error_codes.h"
#include "my_project/logging.h"
```

### 3.4 按需包含（Include What You Use）

> 每个使用的符号必须有直接对应的 `#include`，**禁止依赖传递包含**。

```cpp
// ✓ 直接包含所需头文件
#include "bar.h"

void Foo() {
    Bar b;              // 直接包含 bar.h
    std::string s;      // 直接包含 <string>
}

// ✗ 依赖传递包含（当上游头文件变更时会导致编译失败）
#include "foo.h"        // foo.h 内部 include 了 bar.h

void Foo() {
    Bar b;              // 依赖传递包含 —— 不稳定
}
```

### 3.5 前置声明

> **尽量避免前置声明，直接 include 所需头文件。**

| 场景 | 推荐 |
|------|------|
| 绝大多数情况 | 直接 `#include` |
| std:: 命名空间的符号 | **绝对禁止**前置声明（导致未定义行为） |
| 跨项目引用 | 优先 `#include` |

### 3.6 内联函数

> 只在函数极短（≤10 行）且无循环/switch 时内联。

### 3.7 头文件内容组织

```cpp
#pragma once

#include <memory>
#include <string>

namespace my_project {

// 前向声明（仅当确实不能 #include 时）
class ConnectionPool;

// 常量声明
constexpr int kDefaultPort = 8080;

// 类声明
class HttpClient {
public:
    // 1. 类型定义
    using Ptr = std::shared_ptr<HttpClient>;

    // 2. 构造 / 析构
    HttpClient();
    explicit HttpClient(std::string host);
    ~HttpClient();

    // 3. 拷贝 / 移动语义（Rule of Five）
    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    HttpClient(HttpClient&&) noexcept = default;
    HttpClient& operator=(HttpClient&&) noexcept = default;

    // 4. 公共方法，按逻辑分组
    // --- 连接管理 ---
    bool Connect();
    void Disconnect();
    bool IsConnected() const;

    // --- 请求方法 ---
    HttpResponse Get(const std::string& path);
    HttpResponse Post(const std::string& path, const std::string& body);

private:
    void EnsureConnected();

    std::string host_;
    int port_ = kDefaultPort;
    bool connected_ = false;
};

}  // namespace my_project
```

---

## 4. 类与结构体

### 4.1 声明顺序

> **访问控制顺序：`public` → `protected` → `private`**
> 每个区内顺序：类型定义 → 构造函数 → 析构函数 → 拷贝/移动 → 方法 → 数据成员

```cpp
class MyClass {
public:
    using Ptr = std::shared_ptr<MyClass>;

    MyClass();
    explicit MyClass(int value);
    ~MyClass();

    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;
    MyClass(MyClass&&) noexcept = default;
    MyClass& operator=(MyClass&&) noexcept = default;

    int GetValue() const;
    void SetValue(int value);

protected:
    // protected 内容

private:
    int value_ = 0;
};
```

### 4.2 struct vs class

```cpp
// struct：仅用于被动承载数据的简单聚合类型（POD 类）
struct Point {
    double x = 0.0;
    double y = 0.0;
};

struct Config {
    std::string host = "localhost";
    int port = 8080;
    bool use_ssl = false;
};

// class：用于有不变式（invariant）或封装逻辑的对象
class ConnectionManager {
public:
    void Initialize(const Config& config);
private:
    Config config_;
};
```

> 不要在 struct 中混合 public 和 private 数据成员。

### 4.3 构造 / 析构最佳实践

```cpp
class Buffer {
public:
    // 单参数构造必须加 explicit
    explicit Buffer(size_t size);
    Buffer(size_t size, uint8_t fill_byte);

    // 虚析构：当类可能被继承时必须
    virtual ~Buffer() = default;

    // Rule of Five：显式声明拷贝/移动/析构关系
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;

private:
    std::vector<uint8_t> data_;
};
```

### 4.4 继承

```cpp
class Base {
public:
    Base() = default;
    virtual ~Base() = default;

    virtual void Process() = 0;
    virtual int GetPriority() const { return 0; }

protected:
    void CommonOperation();
};

class Derived : public Base {
public:
    using Base::Base;

    void Process() override;            // 必须加 override
    int GetPriority() const override;
};
```

- 多重继承慎用；避免菱形继承
- 继承层次不宜过深
- 派生类不应重新定义与基类同名的非虚函数

### 4.5 运算符重载

- 禁止重载 `&&`、`||`、`,`（逗号）运算符
- 禁止重载取地址运算符 `&`
- 赋值运算符应返回 `T&`（非 const 左值引用）
- 拷贝/移动赋值运算符不应为虚函数
- C++20：优先用 `= default` 的 `<=>` 自动生成比较运算符

---

## 5. 函数

### 5.1 函数声明

```cpp
// 短函数
int GetId() const { return id_; }

// 长参数列表：每行一个参数
void LongFunctionName(
        int first_param,
        const std::string& second_param,
        std::vector<double>* output);

// 返回类型后置：仅当必要（模板中类型依赖参数）时使用
template <typename T, typename U>
auto Add(T t, U u) -> decltype(t + u);
```

### 5.2 参数规范

```cpp
// 输入参数：const 引用（小对象按值）
void ProcessData(const std::vector<int>& data);
void SetValue(int value);

// 输出参数：非 const 指针
void GetResults(std::vector<Result>* out);

// 输入/输出参数：非 const 指针
void AppendData(std::string* buffer);

// 优先返回值，而非输出参数
std::vector<Result> ComputeResults();     // ✓
void ComputeResults(std::vector<Result>* out);  // 仅在性能必要时使用

// 多态对象禁止按值传递（会导致对象切片）
void ProcessShape(const Shape& shape);    // ✓
void ProcessShape(Shape shape);           // ✗ 切片
```

### 5.3 函数体规范

```cpp
bool HttpClient::SendRequest(const HttpRequest& request,
        HttpResponse* response) {
    // 1. 前置条件
    assert(request.IsValid());
    if (!connected_) {
        if (!Connect()) return false;
    }

    // 2. 核心逻辑
    auto raw_response = SendRaw(request.ToBytes());
    if (!raw_response.ok()) {
        LOG_ERROR("Request failed: {}", raw_response.error_message());
        return false;
    }

    // 3. 结果处理
    *response = HttpResponse::Parse(raw_response.data());
    return true;
}
```

### 5.4 其他规范

- **编写短函数**：超过 40 行应考虑拆分
- **函数重载**：确保语义一致
- **默认参数**：优先用重载替代；虚函数禁用默认参数
- **const 成员函数**：不修改对象的成员函数必须加 `const`

---

## 6. 现代 C++ 特性

### 6.1 auto

```cpp
// ✓ 类型显而易见时
auto it = map.find(key);
auto ptr = std::make_shared<Foo>(args...);
auto result = static_cast<Derived*>(base);
auto [key, value] = *map.begin();           // C++17 结构化绑定
for (auto& x : data) { /* ... */ }

// ✗ 类型不明显时
auto x = GetValue();      // 不清楚返回类型
int x = GetValue();       // 明确

// 显式写 * 和 & 表明意图
auto* ptr = GetObject();  // ✓
auto& ref = GetRef();     // ✓
```

### 6.2 智能指针

```cpp
// unique_ptr 优先（独占所有权）
auto buffer = std::make_unique<Buffer>(1024);
class Owner {
    std::unique_ptr<Resource> resource_;
};

// shared_ptr 仅在确实需要共享时使用
auto config = std::make_shared<Config>();

// C++ 代码禁止 malloc/free
auto obj = std::make_unique<MyClass>(args);   // ✓
MyClass* obj = new MyClass(args);             // 避免
```

### 6.3 nullptr

```cpp
// 永远使用 nullptr，禁止 NULL 或 0 表示空指针
int* ptr = nullptr;   // ✓
int* ptr = NULL;      // ✗
int* ptr = 0;         // ✗
```

### 6.4 强类型枚举

```cpp
// ✓ 优先 enum class
enum class Status {
    kPending,
    kRunning,
    kCompleted,
};

// ✗ 避免 C 风格枚举（污染命名空间）
```

### 6.5 const 与 constexpr

```cpp
// 编译期常量优先用 constexpr
constexpr int kBufferSize = 4096;

// const 成员函数：常量成员函数不应返回非常量指针/引用
class MyClass {
public:
    int GetValue() const { return value_; }
    const std::string& GetName() const { return name_; }
private:
    int value_;
    std::string name_;
};
```

### 6.6 类型转换

```cpp
// ✓ C++ 命名转换，禁止 C 风格转换
int x = static_cast<int>(f);
Derived* d = dynamic_cast<Derived*>(base);
// reinterpret_cast 极少使用，需充分理由

// ✗ 禁止
int x = (int)f;          // C 风格
int x = int(f);          // 函数风格
```

### 6.7 Lambda 表达式

```cpp
// 显式捕获列表，禁止 [=] 和 [&] 全默认捕获
auto func = [this, &result](const Item& item) {
    result.push_back(this->Transform(item));
};

// 长 Lambda 采用与普通函数相同的格式
std::sort(items.begin(), items.end(),
        [](const Item& a, const Item& b) {
            return a.priority < b.priority;
        });
```

### 6.8 移动语义

```cpp
class Buffer {
public:
    // 移动构造必须 noexcept
    Buffer(Buffer&& other) noexcept
        : data_(std::move(other.data_)),
          size_(other.size_) {
        other.size_ = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }

private:
    std::vector<uint8_t> data_;
    size_t size_ = 0;
};
```

### 6.9 C++ 版本特性速查

#### C++17

| 特性 | 建议 |
|------|------|
| 结构化绑定 `auto [a, b] = ...` | 推荐 |
| `if constexpr` | 按需 |
| `std::optional` | 可空值场景推荐 |
| `std::string_view` | 只读字符串参数推荐 |
| `[[nodiscard]]` | 重要返回值必须加 |
| `[[maybe_unused]]` | 按需 |
| `std::variant` | 替代 union |
| 折叠表达式 | 可变参数模板推荐 |

#### C++20

| 特性 | 建议 |
|------|------|
| 概念 (Concepts) | 约束模板推荐 |
| 范围 (Ranges) | 管道式数据处理 |
| `std::span` | 替代指针+长度 |
| `[[likely]]` / `[[unlikely]]` | 性能热点可选 |
| `<=>` 三路比较 | 自动生成比较运算符 |
| `std::format` | 替代 printf / stringstream |
| 模块 (Modules) | 新项目可考虑 |
| `std::bit_cast` | 安全的位重解释替代 `reinterpret_cast` |

#### C++23

| 特性 | 建议 |
|------|------|
| `std::expected<T, E>` | **错误处理首选**，替代异常和输出参数 |
| `std::print` / `std::println` | 格式化输出，替代 `printf` / `cout` |
| `std::flat_map` / `std::flat_set` | 小数据集的连续存储有序容器 |
| `std::mdspan` | 多维数据视图，安全替代裸指针+步长 |
| `std::generator<T>` | 协程生成器，惰性序列 |
| `std::ranges::to<T>()` | 将 range 管道结果收集到容器 |
| `std::views::zip` / `enumerate` | 并行迭代 / 带索引迭代 |
| `std::optional::and_then` / `or_else` / `transform` | Monadic 操作链 |
| `std::expected::and_then` / `or_else` / `transform` | Monadic 错误链 |
| `if consteval` | 编译期分支判断 |
| `[[assume(expr)]]` | 编译器优化提示（慎用，UB 来源） |
| 显式对象参数（推导 this） | 简化 CRTP 和递归 Lambda |
| 多维下标 `operator[]` | 自然的 `mdspan` 访问语法 |
| `static operator()` / `static operator[]` | 无状态函数对象 |

---

## 7. 注释规范

### 7.1 注释风格

```cpp
// 单行注释：优先使用 //

/*
 * 多行注释：用于函数/类的文档说明
 * 每行以 * 开头对齐
 */

int timeout_ = 30;  // 尾部注释：留至少 2 空格
```

### 7.2 文件头注释

```cpp
// http_client.h
//
// HTTP 客户端封装，支持 GET/POST 请求，自动处理重连和超时。
//
// 使用示例：
//   HttpClient client("https://api.example.com");
//   auto response = client.Get("/v1/users");
```

### 7.3 类注释

```cpp
// HttpClient 是 HTTP 客户端同步接口封装。
//
// 特性：
// - 自动处理连接复用
// - 支持请求超时设置
// - 线程安全
//
class HttpClient { /* ... */ };
```

### 7.4 函数注释

```cpp
// 向指定路径发送 GET 请求。
//
// Args:
//     path: 请求路径（相对于 base URL），如 "/v1/users"
//
// Returns:
//     包含状态码和响应体的 HttpResponse 对象。
//
// Throws:
//     NetworkError: 连接失败或超时
//
HttpResponse Get(const std::string& path);
```

### 7.5 TODO 注释

```cpp
// TODO(username): 使用连接池替代每次新建连接
// FIXME(username): 大文件下载可能 OOM，需改为流式
// HACK(username): 临时绕过 SSL 证书校验，待证书配置就绪后移除
// NOTE(username): 此处假定输入已通过校验
```

### 7.6 变量注释

```cpp
// 用于记录当前活跃连接数。仅主线程可写。
int active_connections_ = 0;

constexpr int kMaxRetryAttempts = 3;  // 失败重试最大次数
```

---

## 8. 安全编码规范

> 涵盖内存安全、类型安全、整数与缓冲区安全、异常安全和并发安全。

### 8.1 内存与资源管理

#### R1：使用 RAII 管理所有资源

```cpp
// ✓ 资源由对象生命周期管理
class FileHandler {
public:
    explicit FileHandler(const std::string& path) : file_(fopen(path.c_str(), "r")) {
        if (!file_) throw std::runtime_error("Cannot open file");
    }
    ~FileHandler() { if (file_) fclose(file_); }
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

private:
    FILE* file_;
};

// ✗ 裸资源管理——中间 return 或抛异常会导致泄漏
```

#### R2：禁止 C 风格内存管理

```cpp
// ✓ C++ 中禁止 malloc/free
auto buffer = std::make_unique<uint8_t[]>(size);
std::vector<uint8_t> buffer(size);        // 更推荐

// ✗
uint8_t* buffer = (uint8_t*)malloc(size); // 禁止
free(buffer);                              // 禁止
```

#### R3：禁止重复释放 / 释放后使用

```cpp
// ✓ 智能指针自动管理
std::unique_ptr<Resource> res = std::make_unique<Resource>();
res.reset();

// ✗
delete r;
delete r;                 // 双重释放
r->DoSomething();         // 释放后使用
```

#### R4：移动后重置状态

```cpp
// ✓ 移动后原对象应处于可析构状态
std::string src = "hello";
std::string dst = std::move(src);
src.clear();              // 显式重置
```

#### R5：禁用变长数组（VLA）

```cpp
// ✗ VLA — 不可移植，有栈溢出风险
void Process(int n) {
    int data[n];          // 禁止
}

// ✓
void Process(int n) {
    std::vector<int> data(n);                  // 堆分配
    auto data_ptr = std::make_unique<int[]>(n);  // 智能指针
}
```

> 局部数组不宜过大，过大时应使用堆分配。

#### R6：禁止 0 大小内存分配

```cpp
// ✗ 实现定义行为
int* p = new int[0];

// ✓
if (size > 0) {
    auto p = std::make_unique<int[]>(size);
}
```

### 8.2 类型与转换安全

#### S1：使用安全类型转换

```cpp
// ✓ C++ 命名转换
int64_t large = static_cast<int64_t>(small);
const auto& ref = const_cast<const Type&>(non_const);
auto* base = dynamic_cast<Base*>(derived);

// ✗ 禁用 C 风格和函数风格转换
```

#### S2：不要转换掉真正的 const

```cpp
// ✗ 未定义行为
void Bad(const std::string& s) {
    const_cast<std::string&>(s) = "modified";  // UB
}
```

#### S3：用 std::byte 或 unsigned char 处理原始字节

```cpp
// ✓
std::vector<std::byte> buffer(1024);          // C++17
std::vector<unsigned char> buffer(1024);      // C++14 兼容

// ✗
std::vector<char> buffer(1024);  // char 有符号性不确定
```

#### S4：不可返回局部对象的地址或引用

```cpp
// ✗ 悬垂引用
const std::string& Bad() {
    std::string local = "hello";
    return local;               // 悬垂
}

// ✓ 返回值优化（RVO/NRVO）
std::string Good() {
    std::string local = "hello";
    return local;
}
```

### 8.3 整数与缓冲区安全

#### B1：防范整数溢出

```cpp
// ✓ 检查溢出
#include <limits>

bool SafeAdd(int32_t a, int32_t b, int32_t* result) {
    if ((b > 0 && a > std::numeric_limits<int32_t>::max() - b) ||
        (b < 0 && a < std::numeric_limits<int32_t>::min() - b)) {
        return false;
    }
    *result = a + b;
    return true;
}

// ✓ 使用 size_t 索引容器
for (size_t i = 0; i < vec.size(); ++i) { /* ... */ }
```

#### B2：防范缓冲区溢出

```cpp
// ✓ 使用安全、长度感知的 API
std::string dest;
dest.assign(src, max_len);

std::vector<uint8_t> buf(max_size);
std::copy_n(src, len, buf.begin());

std::format_to_n(buf, max_len, "{}", value);   // C++20

// ✗ 禁用危险函数
strcpy(dst, src);      // 无边界检查
strcat(dst, src);      // 无边界检查
sprintf(buf, "%s", s); // 无边界检查
gets(buf);             // 绝对禁止（C 中已移除）
```

#### B3：防范除 0 和空指针解引用

```cpp
// ✓ 检查除数
if (b == 0.0) {
    throw std::invalid_argument("division by zero");
}
double result = a / b;

// ✓ 判空
void Process(const Config* config) {
    if (!config) return;
    DoWork(*config);
}
```

### 8.4 异常安全

#### E1：析构函数不可抛异常

```cpp
~ResourceHandler() noexcept {
    try {
        Cleanup();
    } catch (...) {
        LOG_ERROR("Cleanup failed");
        // 不传播异常
    }
}
```

#### E2：移动构造/赋值必须 noexcept

```cpp
class MyClass {
public:
    MyClass(MyClass&& other) noexcept = default;
    MyClass& operator=(MyClass&& other) noexcept = default;
};
```

#### E3：按引用捕获异常

```cpp
// ✓
try {
    DoWork();
} catch (const std::exception& e) {
    LOG_ERROR("{}", e.what());
}

// ✗ 按值捕获会切片
catch (std::exception e) { /* ... */ }
```

#### E4：禁止抛指针/nullptr

```cpp
// ✗
throw new std::runtime_error("err");
throw nullptr;

// ✓ 按值抛出，按引用捕获
throw std::runtime_error("err");
```

#### E5：重新抛出用空 throw

```cpp
// ✓ 保留原始异常类型
try {
    DoWork();
} catch (const std::exception& e) {
    LOG_ERROR("{}", e.what());
    throw;    // 保留原始异常
}

// ✗ throw e; ——会切片，丢失原始类型
```

#### E6：C++23 错误处理首选 std::expected

```cpp
// ✓ C++23：std::expected 替代异常和错误码
std::expected<Result, ErrorCode> Compute() {
    if (invalid_input) return std::unexpected(ErrorCode::kInvalid);
    return Result{/* ... */};
}

// 使用 monadic 链
auto final = Compute()
    .and_then([](Result r) { return Transform(r); })
    .or_else([](ErrorCode e) { return HandleError(e); })
    .transform([](auto v) { return Format(v); });

// 检查
if (auto result = Compute(); result.has_value()) {
    UseValue(*result);
} else {
    LOG_ERROR("Failed: {}", static_cast<int>(result.error()));
}
```

### 8.5 宏安全

```cpp
// ✓ 表达式和参数必须加括号
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// ✓ 多语句宏必须用 do-while(0) 包裹
#define LOG_AND_RETURN(msg)  \
    do {                      \
        LOG(msg);             \
        return false;         \
    } while (0)

// ✗ 禁止用宏定义常量或类型 —— 改用 constexpr / using
#define BUFFER_SIZE 1024    // → constexpr int kBufferSize = 1024;
#define INT_PTR int*        // → using IntPtr = int*;

// ✗ 宏参数不应有副作用
MAX(++i, j);               // i 可能被递增两次
```

### 8.6 并发安全

```cpp
// ✓ 使用 std::mutex / std::scoped_lock 保护共享数据
class ThreadSafeCounter {
public:
    void Increment() {
        std::scoped_lock lock(mutex_);
        ++count_;
    }
    int Get() const {
        std::scoped_lock lock(mutex_);
        return count_;
    }
private:
    mutable std::mutex mutex_;
    int count_ = 0;
};

// ✓ 简单同步用 std::atomic
std::atomic<bool> running_{false};

// ✓ C++20: std::jthread / std::stop_token 用于可协作取消的线程
```

### 8.7 禁用特性清单

| 禁用特性 | 替代方案 |
|----------|----------|
| `malloc` / `free` | `new` / `delete` 或智能指针 |
| `strcpy` / `strcat` / `sprintf` / `gets` | `std::string`, `std::format`, `snprintf` |
| C 风格强制转换 | `static_cast<>`, `dynamic_cast<>` |
| C 风格枚举 `enum { A, B }` | `enum class` |
| 变长数组 (VLA) | `std::vector`, `std::make_unique<T[]>` |
| `setjmp` / `longjmp` | C++ 异常或 `std::expected` |
| `NULL` / `0` 作为空指针 | `nullptr` |
| `volatile`（非 MMIO/信号场景） | `std::atomic` |
| `restrict` 指针 | `__restrict`（极少数情况） 或 `std::span` |
| 在 `std` 命名空间中声明 | 直接 include 标准头文件 |
| 头文件中使用 using-directive | 限定名或函数内 using-declaration |
| 动态异常声明 `throw()` | `noexcept` |
| `reinterpret_cast` 用于类型双关 | `std::bit_cast` (C++20) |

---

## 9. 格式规范

### 9.1 行宽

```
最大行宽：120 字符
```

### 9.2 大括号风格

```cpp
// 左大括号不换行（Attach 风格）

if (condition) {
    DoSomething();
} else {
    DoOtherthing();
}

while (running) {
    Process();
}

void Foo() {
    // ...
}

class Bar {
    // ...
};

namespace my_project {
// ...
}  // namespace my_project
```

### 9.3 初始化列表

```cpp
// 简单列表放一行
MyClass::MyClass(int a, int b) : a_(a), b_(b) {}

// 复杂列表每行一个
MyClass::MyClass(int a, int b, const std::string& c)
    : a_(a),
      b_(b),
      c_(c),
      buffer_(1024) {}
```

### 9.4 return 语句

```cpp
return result;        // ✓
return (result);      // ✗ 不必要的括号
return (a + b) * c;   // ✓ 表达式内部括号可以
```

### 9.5 布尔表达式

```cpp
// 在逻辑运算符处换行
if (a_long_condition_1 &&
        a_long_condition_2 ||
        a_long_condition_3) {
    DoSomething();
}
```

### 9.6 变量声明与初始化

```cpp
// 每个变量单独声明
int width;
int height;              // ✓
int width, height;       // ✗

// 声明时立即初始化
int count = 0;           // ✓
auto name = std::string("hello");
int count;               // ✗ 未初始化

// 推荐使用 {} 初始化
int count{0};
std::string name{"hello"};
// 注意：auto x{0} 推导为 initializer_list；auto x = 0 仍用 =
```

### 9.7 条件与循环

```cpp
// 不用不必要的括号
if (a > b) { /* ... */ }      // ✓
if ((a > b)) { /* ... */ }   // ✗

// 显式比较（布尔变量除外）
if (ptr != nullptr) { /* ... */ }   // ✓
if (ptr) { /* ... */ }              // ✗
if (!vec.empty()) { /* ... */ }     // ✓
if (!is_valid) { /* ... */ }        // ✓ (bool 可以直接 !)

// 前置 ++
for (int i = 0; i < n; ++i) { /* ... */ }  // ✓
for (int i = 0; i < n; i++) { /* ... */ }  // 不推荐

// 即使只有一行也必须用大括号
if (cond) {
    DoSomething();               // ✓
}
if (cond) DoSomething();         // ✗
```

### 9.8 预处理器指令

```cpp
// # 始终在行首
#if defined(ENABLE_FEATURE)
#  if defined(PLATFORM_WINDOWS)
    // ...
#  endif  // PLATFORM_WINDOWS
#endif  // ENABLE_FEATURE
```

---

## 10. 项目配置

### 10.1 .clang-format

```yaml
BasedOnStyle: Google
IndentWidth: 4
TabWidth: 4
UseTab: Never
ColumnLimit: 120

AccessModifierOffset: -4
AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false
AlwaysBreakTemplateDeclarations: Yes
BreakBeforeBraces: Attach
BreakConstructorInitializers: BeforeColon

NamespaceIndentation: None
IndentCaseLabels: false
IndentPPDirectives: None

IncludeBlocks: Regroup
IncludeCategories:
  - Regex: '^".*"'
    Priority: 1
  - Regex: '^<[^.]'
    Priority: 2
  - Regex: '^<.*\.h>'
    Priority: 3

SortIncludes: true

PointerAlignment: Left
SpaceAfterCStyleCast: true
SpacesInAngles: false
SpacesInContainerLiterals: false
DerivePointerAlignment: false
```

### 10.2 .editorconfig

```ini
root = true

[*]
indent_style = space
indent_size = 4
end_of_line = lf
charset = utf-8
trim_trailing_whitespace = true
insert_final_newline = true

[*.{cc,cpp,h,hpp}]
indent_size = 4

[CMakeLists.txt]
indent_size = 4

[*.{yml,yaml}]
indent_size = 2

[*.md]
trim_trailing_whitespace = false
```

---

## 11. 代码审查检查清单

### 风格审查

- [ ] **缩进**：全 4 空格，无 Tab？
- [ ] **命名**：类 PascalCase，函数 PascalCase，变量 snake_case_，常量 kPascalCase？
- [ ] **头文件**：自包含？`#pragma once` / guard 到位？Include 顺序正确？
- [ ] **按需包含**：每个符号都有直接 `#include`？
- [ ] **构造/析构**：单参 `explicit`？基类析构 `virtual`？
- [ ] **Rule of Five**：拷贝/移动/析构明确？
- [ ] **智能指针**：`make_unique` / `make_shared`？无裸 new/delete/malloc？
- [ ] **const 正确性**：成员函数、参数、变量尽可能 const？
- [ ] **override**：重写虚函数都加 `override`？
- [ ] **nullptr**：替代 NULL/0？
- [ ] **auto**：仅在类型显而易见时使用？
- [ ] **类型转换**：C++ 命名转换，无 C 风格？
- [ ] **大括号**：所有 if/for/while 都用大括号？
- [ ] **行宽**：≤120 字符？
- [ ] **return**：无不必要括号？
- [ ] **宏**：可改用 constexpr/inline/模板？

### 安全审查

- [ ] **RAII**：所有资源由对象生命周期管理？
- [ ] **资源泄漏**：分配的资源都有释放？
- [ ] **空指针**：指针参数和返回值判空？
- [ ] **越界检查**：数组/容器访问在边界内？
- [ ] **整数溢出**：关键运算有溢出检查？
- [ ] **除 0 检查**：除法/取模检查除数为 0？
- [ ] **异常安全**：析构 noexcept？移动 noexcept？
- [ ] **异常传播**：按引用捕获？未抛指针？
- [ ] **宏安全**：表达式/参数加括号？多语句 do-while(0)？
- [ ] **危险函数**：无 strcpy/strcat/sprintf/gets？
- [ ] **VLA**：无变长数组？
- [ ] **C 风格转换**：无 C 风格/函数风格转换？
- [ ] **敏感数据**：未写入代码或日志？
- [ ] **并发安全**：共享数据有同步？
- [ ] **未定义行为**：无不依赖未定义/未声明/实现定义行为？

### C++23 专项

- [ ] **错误处理**：是否考虑用 `std::expected` 替代异常或错误码？
- [ ] **格式化**：是否用 `std::print` 替代 `printf` / `cout`？
- [ ] **Ranges**：管道式操作是否用 `std::ranges::to` 收集结果？
- [ ] **类型双关**：是否用 `std::bit_cast` 替代 `reinterpret_cast`？
- [ ] **多维访问**：是否用 `std::mdspan` 替代裸多维数组？

---

## 12. 快速参考卡片

```
┌───────────────────────────────────────────────────────────────┐
│            C++ Code Style Quick Reference                      │
│       C++17 / C++20 / C++23 — 安全现代 C++ 规范                │
├────────────────┬──────────────────────────────────────────────┤
│ 缩进           │ 4 空格，不用 Tab                              │
│ 行宽           │ ≤ 120 字符                                    │
│ 源文件         │ .cc 或 .cpp                                   │
│ 类名           │ PascalCase        │ HttpClient               │
│ 函数名         │ PascalCase        │ GetName()                │
│ 变量名         │ snake_case        │ user_count               │
│ 成员变量       │ snake_case_       │ count_                   │
│ 常量           │ kPascalCase       │ kMaxSize                 │
│ 枚举值         │ kPascalCase       │ kRed                     │
│ 宏             │ UPPER_SNAKE_CASE  │ MY_PROJECT_BUFFER_SIZE   │
│ 命名空间       │ snake_case        │ my_project               │
│ 文件名         │ snake_case        │ http_client.h            │
│ 头文件保护     │ #pragma once 或 guard                        │
│ nullptr        │ 禁止 NULL / 0 表示空指针                      │
│ 大括号         │ 左括号不换行（Attach）                         │
│ 智能指针       │ make_unique / make_shared                     │
│ 单参构造       │ explicit                                      │
│ 虚析构         │ 继承时必须                                    │
│ override       │ 必须                                          │
│ const          │ 尽可能                                        │
│ enum class     │ 强制使用                                      │
│ 类型转换       │ static_cast / dynamic_cast                    │
│ 异常           │ 按引用捕获；析构/移动 noexcept                 │
│ 错误处理       │ C++23 推荐 std::expected                      │
│ 类型双关       │ std::bit_cast (C++20)                         │
│ RAII           │ 所有资源由对象管理                            │
│ 不安全特性     │ VLA / C 转换 / malloc / strcpy 等全部禁用     │
└────────────────┴──────────────────────────────────────────────┘
```

---

## 13. 参考资源

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — Bjarne Stroustrup & Herb Sutter 主编
- [CppCon / C++Now 现代 C++ 最佳实践演讲](https://github.com/CppCon)
- [cppreference.com](https://en.cppreference.com/w/) — C++ 标准库权威文档
